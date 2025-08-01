//
// Created by exozg on 01/06/2025.
//

#include "SkintoneVariantGenerator.h"

#include "../../core/assets/Assets.h"
#include "../../core/hash/FNV.h"
#include "../../core/util/StreamUtil.h"
#include "../../EA/IO/MemoryStream.h"

#include <comdef.h>

bool SkintoneVariantGenerator::CreateImage(const std::vector<uint8_t> &kBuffer, DirectX::ScratchImage &image) {
    DirectX::ScratchImage loadedImage;

    if (FAILED(DirectX::LoadFromDDSMemory(
        kBuffer.data(), kBuffer.size(), DirectX::DDS_FLAGS_NONE, nullptr, loadedImage
    ))) {
        return false;
    }

    const auto kMetaData = loadedImage.GetMetadata();

    if (DirectX::IsCompressed(kMetaData.format)) {
        if (FAILED(DirectX::Decompress(
            loadedImage.GetImages(), loadedImage.GetImageCount(), kMetaData,
            DXGI_FORMAT_R8G8B8A8_UNORM, image
        ))) { return false; }
    } else if (kMetaData.format != DXGI_FORMAT_R8G8B8A8_UNORM) {
        if (FAILED(DirectX::Convert(
            loadedImage.GetImages(), loadedImage.GetImageCount(), kMetaData,
            DXGI_FORMAT_R8G8B8A8_UNORM, DirectX::TEX_FILTER_LINEAR, 0.0F, image
        ))) { return false; }
    } else {
        image = std::move(loadedImage);
    }

    return true;
}

DirectX::XMVECTOR ColorFromUINT(const uint32_t kDxgiColor) {
    return DirectX::XMVectorSet(
        (kDxgiColor & 0xFF) / 255.0F,
        ((kDxgiColor >> 8) & 0xFF) / 255.0F,
        ((kDxgiColor >> 16) & 0xFF) / 255.0F,
        ((kDxgiColor >> 24) & 0xFF) / 255.0F
    );
}

uint32_t ColorToUINT(const DirectX::XMVECTOR kColor) {
    const uint32_t r = std::min(255U, static_cast<uint32_t>(lround(DirectX::XMVectorGetX(kColor) * 255.0F)));
    const uint32_t g = std::min(255U, static_cast<uint32_t>(lround(DirectX::XMVectorGetY(kColor) * 255.0F)));
    const uint32_t b = std::min(255U, static_cast<uint32_t>(lround(DirectX::XMVectorGetZ(kColor) * 255.0F)));
    const uint32_t a = std::min(255U, static_cast<uint32_t>(lround(DirectX::XMVectorGetW(kColor) * 255.0F)));
    return (a << 24) | (b << 16) | (g << 8) | r;
}

bool SkintoneVariantGenerator::Generate(const std::string &kName, EA::ResourceMan::IRecord *pBaseLayer,
                                        EA::ResourceMan::IRecord *pMaskLayer) {
    MSML_LOG_DEBUG("Generating skin tone textures for %s", kName.c_str());

    const auto kBaseLayerBuffer = Msml::Core::Util::StreamUtil::ReadBytes(pBaseLayer->GetStream());
    const auto kMaskLayerBuffer = Msml::Core::Util::StreamUtil::ReadBytes(pMaskLayer->GetStream());

    DirectX::ScratchImage baseImage;
    DirectX::ScratchImage maskImage;
    if (!CreateImage(kBaseLayerBuffer, baseImage)) { return false; }
    if (!CreateImage(kMaskLayerBuffer, maskImage)) { return false; }

    const DirectX::Image *base = baseImage.GetImage(0, 0, 0);
    const DirectX::Image *mask = maskImage.GetImage(0, 0, 0);
    size_t width = base->width;
    size_t height = base->height;

    for (int i = 0; i < kVariantCount; i++) {
        Vector4 variant = kVariants[i];

        DirectX::ScratchImage scratchResult;
        if (FAILED(scratchResult.Initialize2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 1))) {
            continue;
        }
        const DirectX::Image *dst = scratchResult.GetImage(0, 0, 0);

        for (size_t y = 0; y < height; ++y) {
            auto *out = reinterpret_cast<uint32_t *>(dst->pixels + y * dst->rowPitch);
            const auto *baseRow = reinterpret_cast<const uint32_t *>(base->pixels + (y * base->rowPitch));

            auto maskY = static_cast<size_t>((static_cast<float>(y) / height) * mask->height);
            maskY = std::min(maskY, mask->height - 1);
            const auto *maskRow = reinterpret_cast<const uint32_t *>(mask->pixels + (maskY * mask->rowPitch));

            for (size_t x = 0; x < width; ++x) {
                auto maskX = static_cast<size_t>((static_cast<float>(x) / width) * mask->width);
                maskX = std::min(maskX, mask->width - 1);

                DirectX::XMVECTOR diffuseColor = ColorFromUINT(baseRow[x]);
                DirectX::XMVECTOR skinToneMask = ColorFromUINT(maskRow[maskX]);
                DirectX::XMVECTOR skinColor = DirectX::XMVectorSet(
                    variant.x / 255.0F, variant.y / 255.0F, variant.z / 255.0F, 1.0F);

                DirectX::XMVECTOR result = DirectX::XMVectorLerp(diffuseColor,
                                                                 DirectX::XMVectorMultiply(skinToneMask, skinColor),
                                                                 DirectX::XMVectorGetW(skinToneMask));

                // DirectX::XMVectorSetW(result, 1); // no transparency

                out[x] = ColorToUINT(result);
            }
        }

        DirectX::Blob blob;
        if (FAILED(DirectX::SaveToDDSMemory(*dst, DirectX::DDS_FLAGS_NONE, blob))) {
            return false;
        }

        const std::string kFileNameWithSkin = kName + "_s" + std::to_string(i) + ".dds";

        auto *const kAsset = new Msml::Core::Asset(Msml::Core::Asset::GetKey(kFileNameWithSkin),
                                                   Msml::Core::AssetType::kBuffer);
        kAsset->mBuffer.assign(
            blob.GetConstBufferPointer(),
            blob.GetConstBufferPointer() + blob.GetBufferSize()
        );

        Msml::Core::Assets::GetInstance().RegisterAsset(kAsset);
    }

    return true;
}
