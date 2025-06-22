//
// Created by exozg on 23/05/2025.
//

#include <fstream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../libs/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "core/util/StreamUtil.h"
#include "EA/IO/MemoryStream.h"
#include "formats/materials/MaterialSet.h"


TEST(MaterialSet, Read) {
    MaterialSet materialSet;

    auto *const stream = new EA::IO::FileStream(TEST_ASSETS_DIR "0xdd91919d!0x00000000db272b16.mMaterialset");
    stream->AddRef();
    MaterialSet::Read(materialSet, stream);

    ASSERT_EQ(materialSet.mMaterials.size(), 5);

    ASSERT_EQ(materialSet.mSelf.mInstance, 0xDB272B16);
    ASSERT_EQ(materialSet.mSelf.mType, 0x2019972);
    ASSERT_EQ(materialSet.mSelf.mGroup, 0xDD91919D);

    {
        ASSERT_EQ(materialSet.mMaterials[0].mInstance, 0xDFFF58C4);
        ASSERT_EQ(materialSet.mMaterials[0].mType, 0x1D0E75D);
        ASSERT_EQ(materialSet.mMaterials[0].mGroup, 0xDB272B16);
    }

    {
        ASSERT_EQ(materialSet.mMaterials[1].mInstance, 0x4F4D01B1);
        ASSERT_EQ(materialSet.mMaterials[1].mType, 0x1D0E75D);
        ASSERT_EQ(materialSet.mMaterials[1].mGroup, 0xDB272B16);
    }

    {
        ASSERT_EQ(materialSet.mMaterials[2].mInstance, 0x71B2317C);
        ASSERT_EQ(materialSet.mMaterials[2].mType, 0x1D0E75D);
        ASSERT_EQ(materialSet.mMaterials[2].mGroup, 0xDB272B16);
    }

    {
        ASSERT_EQ(materialSet.mMaterials[3].mInstance, 0xEE3C68FD);
        ASSERT_EQ(materialSet.mMaterials[3].mType, 0x1D0E75D);
        ASSERT_EQ(materialSet.mMaterials[3].mGroup, 0xDB272B16);
    }

    {
        ASSERT_EQ(materialSet.mMaterials[4].mInstance, 0x8EC04A9);
        ASSERT_EQ(materialSet.mMaterials[4].mType, 0x1D0E75D);
        ASSERT_EQ(materialSet.mMaterials[4].mGroup, 0xDB272B16);
    }

    ASSERT_EQ(materialSet.mMtst.mName, 0xDD91919D);
    ASSERT_EQ(materialSet.mMtst.mIndex, 0x30000001);
    ASSERT_EQ(materialSet.mMtst.mIndices.size(), 5);

    ASSERT_EQ(materialSet.mMtst.mIndices[0], 805306370);
    ASSERT_EQ(materialSet.mMtst.mIndices[1], 805306371);
    ASSERT_EQ(materialSet.mMtst.mIndices[2], 805306369);
    ASSERT_EQ(materialSet.mMtst.mIndices[3], 805306372);
    ASSERT_EQ(materialSet.mMtst.mIndices[4], 805306373);
}

TEST(MaterialSet, Write) {
    MaterialSet materialSet;
    auto *const kReadStream = new EA::IO::FileStream(TEST_ASSETS_DIR "0xdd91919d!0x00000000db272b16.mMaterialset");
    kReadStream->AddRef();
    auto *const kWriteStream = new EA::IO::MemoryStream;
    kWriteStream->AddRef();
    MaterialSet::Read(materialSet, kReadStream);
    materialSet.Write(kWriteStream);

    const auto kInBuffer = Msml::Core::Util::StreamUtil::ReadBytes(kReadStream);
    const auto kOutBuffer = Msml::Core::Util::StreamUtil::ReadBytes(kWriteStream);

    kReadStream->Close();
    kReadStream->Release();
    kWriteStream->Close();
    kWriteStream->Release();

    ASSERT_EQ(kInBuffer.size(), kOutBuffer.size());
    EXPECT_THAT(kInBuffer, ::testing::ElementsAreArray(kOutBuffer));
}