//
// Created by exozg on 01/06/2025.
//

#include "SkintoneVariantGenerator.h"

#include "../../core/assets/Assets.h"
#include "../../core/hash/FNV.h"
#include "../../core/util/StreamUtil.h"
#include "../../EA/IO/MemoryStream.h"

#include <comdef.h>

bool SkintoneVariantGenerator::CreateImage(const std::vector<uint8_t>& buffer, DirectX::ScratchImage& finalImage) {
    DirectX::ScratchImage loadedImage;

    HRESULT hr = DirectX::LoadFromDDSMemory(
        buffer.data(), buffer.size(), DirectX::DDS_FLAGS_NONE, nullptr, loadedImage
    );
    if (FAILED(hr)) return false;

    const auto metadata = loadedImage.GetMetadata();

    if (DirectX::IsCompressed(metadata.format)) {
        hr = DirectX::Decompress(
            loadedImage.GetImages(), loadedImage.GetImageCount(), metadata,
            DXGI_FORMAT_R8G8B8A8_UNORM, finalImage
        );
        if (FAILED(hr)) return false;
    } else if (metadata.format != DXGI_FORMAT_R8G8B8A8_UNORM) {
        hr = DirectX::Convert(
            loadedImage.GetImages(), loadedImage.GetImageCount(), metadata,
            DXGI_FORMAT_R8G8B8A8_UNORM, DirectX::TEX_FILTER_LINEAR, 0.0f, finalImage
        );
        if (FAILED(hr)) return false;
    } else {
        finalImage = std::move(loadedImage);
    }

    return true;
}

DirectX::XMVECTOR ColorFromUINT(uint32_t dxgiColor) {
    return DirectX::XMVectorSet(
        ( dxgiColor        & 0xFF) / 255.0f,       // Red   = bits 0–7
        ((dxgiColor >> 8)  & 0xFF) / 255.0f,       // Green = bits 8–15
        ((dxgiColor >> 16) & 0xFF) / 255.0f,       // Blue  = bits 16–23
        ((dxgiColor >> 24) & 0xFF) / 255.0f        // Alpha = bits 24–31
    );
}

uint32_t ColorToUINT(const DirectX::XMVECTOR color) {
    const uint32_t r = std::min(255u, static_cast<uint32_t>(DirectX::XMVectorGetX(color) * 255.0f + 0.5f));
    const uint32_t g = std::min(255u, static_cast<uint32_t>(DirectX::XMVectorGetY(color) * 255.0f + 0.5f));
    const uint32_t b = std::min(255u, static_cast<uint32_t>(DirectX::XMVectorGetZ(color) * 255.0f + 0.5f));
    const uint32_t a = std::min(255u, static_cast<uint32_t>(DirectX::XMVectorGetW(color) * 255.0f + 0.5f));
    return (a << 24) | (b << 16) | (g << 8) | r;
}

bool SkintoneVariantGenerator::Generate(const std::string &name, EA::ResourceMan::IRecord *base_layer,
                                        EA::ResourceMan::IRecord *mask_layer) {
    MSML_LOG_DEBUG("Generating skin tone textures for %s", name.c_str());

    const auto base_layer_buffer = msml::core::util::StreamUtil::ReadBytes(base_layer->GetStream());
    const auto mask_layer_buffer = msml::core::util::StreamUtil::ReadBytes(mask_layer->GetStream());

    DirectX::ScratchImage baseImage, maskImage;
    if (!CreateImage(base_layer_buffer, baseImage)) return false;
    if (!CreateImage(mask_layer_buffer, maskImage)) return false;

    const DirectX::Image *base = baseImage.GetImage(0, 0, 0);
    const DirectX::Image *mask = maskImage.GetImage(0, 0, 0);
    size_t width = base->width, height = base->height;

    int skin_id = 0;
    for (int i = 0; i < VARIANT_COUNT; i++) {
        Vector4 variant = variants[i];

        DirectX::ScratchImage scratchResult;
        scratchResult.Initialize2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 1);
        const DirectX::Image *dst = scratchResult.GetImage(0, 0, 0);

        for (size_t y = 0; y < height; ++y) {
            auto *out = reinterpret_cast<uint32_t *>(dst->pixels + y * dst->rowPitch);
            const auto *baseRow = reinterpret_cast<const uint32_t *>(base->pixels + y * base->rowPitch);

            auto maskY = static_cast<size_t>((static_cast<float>(y) / height) * mask->height);
            maskY = std::min(maskY, mask->height - 1);
            const auto *maskRow = reinterpret_cast<const uint32_t *>(mask->pixels + maskY * mask->rowPitch);

            for (size_t x = 0; x < width; ++x) {
                size_t maskX = static_cast<size_t>((static_cast<float>(x) / width) * mask->width);
                maskX = std::min(maskX, mask->width - 1);

                DirectX::XMVECTOR diffuseColor = ColorFromUINT(baseRow[x]);
                DirectX::XMVECTOR skinToneMask = ColorFromUINT(maskRow[maskX]);
                DirectX::XMVECTOR skinColor = DirectX::XMVectorSet(
                    variant.x / 255.0f, variant.y / 255.0f, variant.z / 255.0f, 1.0f);

                DirectX::XMVECTOR result = DirectX::XMVectorLerp(diffuseColor, DirectX::XMVectorMultiply(skinToneMask, skinColor), DirectX::XMVectorGetW(skinToneMask));

                // DirectX::XMVectorSetW(result, 1); // no transparency

                out[x] = ColorToUINT(result);
            }
        }

        DirectX::Blob blob;
        if (FAILED(DirectX::SaveToDDSMemory(*dst, DirectX::DDS_FLAGS_NONE, blob)))
            return false;

        const std::string file_name_with_skin = name + "_s" + std::to_string(skin_id) + ".dds";

        const auto asset = new msml::core::assets::Asset(msml::core::assets::Asset::GetKey(file_name_with_skin), msml::core::assets::BUFFER);
        asset->buffer.assign(
            static_cast<const uint8_t*>(blob.GetBufferPointer()),
            static_cast<const uint8_t*>(blob.GetBufferPointer()) + blob.GetBufferSize()
        );

        msml::core::Assets::GetInstance().RegisterAsset(asset);

        skin_id++;
    }

    return true;
}
