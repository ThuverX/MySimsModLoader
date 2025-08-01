#include "Material.h"

void Material::Read(Material &instance, EA::IO::IStream *pStream) {
    SKIP(pStream, 4); // 1
    SKIP(pStream, 4); // 1
    SKIP(pStream, 4); // 0
    SKIP(pStream, 4); // 0
    SKIP(pStream, 4); // 1

    EA::ResourceMan::Key::Read(instance.mSelf, pStream);

    SKIP(pStream, 4); // Header Size
    SKIP(pStream, 4); // Total Size

    SKIP(pStream, 4); // MATD
    SKIP(pStream, 4); // MATD version

    READ(pStream, instance.mMaterialId);
    READ(pStream, instance.mShaderId);

    SKIP(pStream, 4); // MTRL size

    const size_t kMtrlOffset = OFFSET(pStream);

    SKIP(pStream, 4); // MTRL
    SKIP(pStream, 4); // MTRL version

    SKIP(pStream, 4); // Data size
    READ_VALUE(pStream, uint32_t, numParams);

    for (uint32_t i = 0; i < numParams; i++) {
        MaterialParameter parameter{};
        MaterialParameter::Read(parameter, kMtrlOffset, pStream);

        instance.mParameters.push_back(parameter);
    }
}

void Material::Write(EA::IO::IStream *pStream) {
    WRITE_VALUE(pStream, uint32_t, 1);
    WRITE_VALUE(pStream, uint32_t, 1);
    WRITE_VALUE(pStream, uint32_t, 0);
    WRITE_VALUE(pStream, uint32_t, 0);
    WRITE_VALUE(pStream, uint32_t, 1);

    mSelf.Write(pStream);

    WRITE_VALUE(pStream, uint32_t, OFFSET(pStream) + 8);
    CREATE_HOLE(pStream, totalSize, uint32_t);
    const size_t kTotalOffset = OFFSET(pStream);

    WRITE_STRING(pStream, "MATD");
    WRITE_VALUE(pStream, uint32_t, 1);

    WRITE(pStream, mMaterialId);
    WRITE(pStream, mShaderId);

    CREATE_HOLE(pStream, mtrlSize, uint32_t);
    const size_t kMtrlOffset = OFFSET(pStream);

    WRITE_STRING(pStream, "MTRL");
    WRITE_VALUE(pStream, uint32_t, 0);

    CREATE_HOLE(pStream, dataSize, uint32_t);

    const uint32_t kNumParams = mParameters.size();
    WRITE_VALUE(pStream, uint32_t, kNumParams);

    uint32_t rollingOffset = OFFSET(pStream) + 16 * kNumParams - kMtrlOffset;

    for (auto &parameter: mParameters) {
        parameter.mOffset = rollingOffset;
        rollingOffset += parameter.GetBodySize();
        parameter.WriteHeader(pStream);
    }

    const size_t kDataOffset = OFFSET(pStream);

    for (auto &parameter: mParameters) {
        parameter.WriteBody(pStream);
    }

    const size_t kEndOffset = OFFSET(pStream);

    FILL_HOLE(pStream, dataSize, kEndOffset - kDataOffset);
    FILL_HOLE(pStream, totalSize, kEndOffset - kTotalOffset);
    FILL_HOLE(pStream, mtrlSize, kEndOffset - kMtrlOffset);
}
