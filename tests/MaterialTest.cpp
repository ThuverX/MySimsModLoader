#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include "formats/materials/Material.h"

#include <cstdint>
#include <fstream>

#include "../libs/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "core/util/StreamUtil.h"
#include "EA/IO/MemoryStream.h"

TEST(Material, Read) {
    Material material;
    auto *const kStream = new EA::IO::FileStream(TEST_ASSETS_DIR "0x4d114292!0x000000007dea5de1.Material");
    kStream->AddRef();
    Material::Read(material, kStream);

    ASSERT_EQ(material.mSelf.mInstance, 0x7DEA5DE1);
    ASSERT_EQ(material.mSelf.mType, 0x1D0E75D);
    ASSERT_EQ(material.mSelf.mGroup, 0x4D114292);

    ASSERT_EQ(material.mMaterialId, 0x7DEA5DE1);
    ASSERT_EQ(material.mShaderId, 0x94773578);

    ASSERT_EQ(material.mParameters.size(), 11); {
        ASSERT_EQ(material.mParameters[0].mName, 0xDAA9532D);
        ASSERT_EQ(material.mParameters[0].mType, 1);
        ASSERT_EQ(material.mParameters[0].mValueFieldCount, 1);
        ASSERT_EQ(material.mParameters[0].mOffset, 192);
        ASSERT_FLOAT_EQ(material.mParameters[0].mColorValue.x, 0);
    } {
        ASSERT_EQ(material.mParameters[1].mName, 0x3BD441A0);
        ASSERT_EQ(material.mParameters[1].mType, 1);
        ASSERT_EQ(material.mParameters[1].mValueFieldCount, 3);
        ASSERT_EQ(material.mParameters[1].mOffset, 196);
        ASSERT_FLOAT_EQ(material.mParameters[1].mColorValue.x, 0);
        ASSERT_FLOAT_EQ(material.mParameters[1].mColorValue.y, 0);
        ASSERT_FLOAT_EQ(material.mParameters[1].mColorValue.z, 0);
    } {
        ASSERT_EQ(material.mParameters[2].mName, 0x988403F9);
        ASSERT_EQ(material.mParameters[2].mType, 1);
        ASSERT_EQ(material.mParameters[2].mValueFieldCount, 3);
        ASSERT_EQ(material.mParameters[2].mOffset, 208);
        ASSERT_FLOAT_EQ(material.mParameters[2].mColorValue.x, 0);
        ASSERT_FLOAT_EQ(material.mParameters[2].mColorValue.y, 0);
        ASSERT_FLOAT_EQ(material.mParameters[2].mColorValue.z, 0);
    } {
        ASSERT_EQ(material.mParameters[3].mName, 0x5D22FD3);
        ASSERT_EQ(material.mParameters[3].mType, 1);
        ASSERT_EQ(material.mParameters[3].mValueFieldCount, 1);
        ASSERT_EQ(material.mParameters[3].mOffset, 220);
        ASSERT_FLOAT_EQ(material.mParameters[3].mColorValue.x, 1);
    } {
        ASSERT_EQ(material.mParameters[4].mName, 0xF46B90AE);
        ASSERT_EQ(material.mParameters[4].mType, 2);
        ASSERT_EQ(material.mParameters[4].mValueFieldCount, 1);
        ASSERT_EQ(material.mParameters[4].mOffset, 224);
        ASSERT_EQ(material.mParameters[4].mIntValue, 0);
    } {
        ASSERT_EQ(material.mParameters[5].mName, 0x29BCDD1F);
        ASSERT_EQ(material.mParameters[5].mType, 1);
        ASSERT_EQ(material.mParameters[5].mValueFieldCount, 1);
        ASSERT_EQ(material.mParameters[5].mOffset, 228);
        ASSERT_FLOAT_EQ(material.mParameters[5].mColorValue.x, 1);
    } {
        ASSERT_EQ(material.mParameters[6].mName, 0x73C9923E);
        ASSERT_EQ(material.mParameters[6].mType, 1);
        ASSERT_EQ(material.mParameters[6].mValueFieldCount, 3);
        ASSERT_EQ(material.mParameters[6].mOffset, 232);
        ASSERT_FLOAT_EQ(material.mParameters[6].mColorValue.x, 1);
        ASSERT_FLOAT_EQ(material.mParameters[6].mColorValue.y, 1);
        ASSERT_FLOAT_EQ(material.mParameters[6].mColorValue.z, 1);
    } {
        ASSERT_EQ(material.mParameters[7].mName, 0x4A5DAA3);
        ASSERT_EQ(material.mParameters[7].mType, 1);
        ASSERT_EQ(material.mParameters[7].mValueFieldCount, 3);
        ASSERT_EQ(material.mParameters[7].mOffset, 244);
        ASSERT_FLOAT_EQ(material.mParameters[7].mColorValue.x, 0);
        ASSERT_FLOAT_EQ(material.mParameters[7].mColorValue.y, 0);
        ASSERT_FLOAT_EQ(material.mParameters[7].mColorValue.z, 0);
    } {
        ASSERT_EQ(material.mParameters[8].mName, 0x76F88689);
        ASSERT_EQ(material.mParameters[8].mType, 2);
        ASSERT_EQ(material.mParameters[8].mValueFieldCount, 1);
        ASSERT_EQ(material.mParameters[8].mOffset, 256);
        ASSERT_EQ(material.mParameters[8].mIntValue, 1);
    } {
        ASSERT_EQ(material.mParameters[9].mName, 0x7FEE2D1A);
        ASSERT_EQ(material.mParameters[9].mType, 1);
        ASSERT_EQ(material.mParameters[9].mValueFieldCount, 4);
        ASSERT_EQ(material.mParameters[9].mOffset, 260);
        ASSERT_FLOAT_EQ(material.mParameters[9].mColorValue.x, 1);
        ASSERT_FLOAT_EQ(material.mParameters[9].mColorValue.y, 1);
        ASSERT_FLOAT_EQ(material.mParameters[9].mColorValue.z, 1);
        ASSERT_FLOAT_EQ(material.mParameters[9].mColorValue.w, 1);
    } {
        ASSERT_EQ(material.mParameters[10].mName, 0x6CC0FD85);
        ASSERT_EQ(material.mParameters[10].mType, 4);
        ASSERT_EQ(material.mParameters[10].mValueFieldCount, 4);
        ASSERT_EQ(material.mParameters[10].mOffset, 276);
        ASSERT_EQ(material.mParameters[10].mKeyValue.mInstance, 0x7DEA5DE1);
        ASSERT_EQ(material.mParameters[10].mKeyValue.mType, 0xB2D882);
        ASSERT_EQ(material.mParameters[10].mKeyValue.mGroup, 0x00);
    }

    kStream->Close();
    kStream->Release();
}

TEST(Material, Write) {
    Material material;
    auto *const kReadStream = new EA::IO::FileStream(TEST_ASSETS_DIR "0x4d114292!0x000000007dea5de1.Material");
    kReadStream->AddRef();
    auto *const kWriteStream = new EA::IO::MemoryStream;
    kWriteStream->AddRef();
    Material::Read(material, kReadStream);
    material.Write(kWriteStream);

    const auto kInBuffer = Msml::Core::Util::StreamUtil::ReadBytes(kReadStream);
    const auto kOutBuffer =Msml::Core::Util::StreamUtil::ReadBytes(kWriteStream);

    kReadStream->Close();
    kReadStream->Release();
    kWriteStream->Close();
    kWriteStream->Release();

    ASSERT_EQ(kInBuffer.size(), kOutBuffer.size());
    EXPECT_THAT(kInBuffer, ::testing::ElementsAreArray(kOutBuffer));
}
