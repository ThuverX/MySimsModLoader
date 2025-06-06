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
    const auto stream = new EA::IO::FileStream(TEST_ASSETS_DIR "0x4d114292!0x000000007dea5de1.Material");
    stream->AddRef();
    Material::Read(material, stream);

    ASSERT_EQ(material.self.instance, 0x7DEA5DE1);
    ASSERT_EQ(material.self.type, 0x1D0E75D);
    ASSERT_EQ(material.self.group, 0x4D114292);

    ASSERT_EQ(material.headerSize, 44);
    ASSERT_EQ(material.totalSize, 312);

    ASSERT_EQ(material.materialId, 0x7DEA5DE1);
    ASSERT_EQ(material.shaderId, 0x94773578);

    ASSERT_EQ(material.mtrlSize, 292);

    ASSERT_EQ(material.dataSize, 100);
    ASSERT_EQ(material.numParams, 11);
    ASSERT_EQ(material.parameters.size(), 11); {
        ASSERT_EQ(material.parameters[0].name, 0xDAA9532D);
        ASSERT_EQ(material.parameters[0].type, 1);
        ASSERT_EQ(material.parameters[0].valueFieldCount, 1);
        ASSERT_EQ(material.parameters[0].offset, 192);
        ASSERT_FLOAT_EQ(material.parameters[0].colorValue.x, 0);
    } {
        ASSERT_EQ(material.parameters[1].name, 0x3BD441A0);
        ASSERT_EQ(material.parameters[1].type, 1);
        ASSERT_EQ(material.parameters[1].valueFieldCount, 3);
        ASSERT_EQ(material.parameters[1].offset, 196);
        ASSERT_FLOAT_EQ(material.parameters[1].colorValue.x, 0);
        ASSERT_FLOAT_EQ(material.parameters[1].colorValue.y, 0);
        ASSERT_FLOAT_EQ(material.parameters[1].colorValue.z, 0);
    } {
        ASSERT_EQ(material.parameters[2].name, 0x988403F9);
        ASSERT_EQ(material.parameters[2].type, 1);
        ASSERT_EQ(material.parameters[2].valueFieldCount, 3);
        ASSERT_EQ(material.parameters[2].offset, 208);
        ASSERT_FLOAT_EQ(material.parameters[2].colorValue.x, 0);
        ASSERT_FLOAT_EQ(material.parameters[2].colorValue.y, 0);
        ASSERT_FLOAT_EQ(material.parameters[2].colorValue.z, 0);
    } {
        ASSERT_EQ(material.parameters[3].name, 0x5D22FD3);
        ASSERT_EQ(material.parameters[3].type, 1);
        ASSERT_EQ(material.parameters[3].valueFieldCount, 1);
        ASSERT_EQ(material.parameters[3].offset, 220);
        ASSERT_FLOAT_EQ(material.parameters[3].colorValue.x, 1);
    } {
        ASSERT_EQ(material.parameters[4].name, 0xF46B90AE);
        ASSERT_EQ(material.parameters[4].type, 2);
        ASSERT_EQ(material.parameters[4].valueFieldCount, 1);
        ASSERT_EQ(material.parameters[4].offset, 224);
        ASSERT_EQ(material.parameters[4].intValue, 0);
    } {
        ASSERT_EQ(material.parameters[5].name, 0x29BCDD1F);
        ASSERT_EQ(material.parameters[5].type, 1);
        ASSERT_EQ(material.parameters[5].valueFieldCount, 1);
        ASSERT_EQ(material.parameters[5].offset, 228);
        ASSERT_FLOAT_EQ(material.parameters[5].colorValue.x, 1);
    } {
        ASSERT_EQ(material.parameters[6].name, 0x73C9923E);
        ASSERT_EQ(material.parameters[6].type, 1);
        ASSERT_EQ(material.parameters[6].valueFieldCount, 3);
        ASSERT_EQ(material.parameters[6].offset, 232);
        ASSERT_FLOAT_EQ(material.parameters[6].colorValue.x, 1);
        ASSERT_FLOAT_EQ(material.parameters[6].colorValue.y, 1);
        ASSERT_FLOAT_EQ(material.parameters[6].colorValue.z, 1);
    } {
        ASSERT_EQ(material.parameters[7].name, 0x4A5DAA3);
        ASSERT_EQ(material.parameters[7].type, 1);
        ASSERT_EQ(material.parameters[7].valueFieldCount, 3);
        ASSERT_EQ(material.parameters[7].offset, 244);
        ASSERT_FLOAT_EQ(material.parameters[7].colorValue.x, 0);
        ASSERT_FLOAT_EQ(material.parameters[7].colorValue.y, 0);
        ASSERT_FLOAT_EQ(material.parameters[7].colorValue.z, 0);
    } {
        ASSERT_EQ(material.parameters[8].name, 0x76F88689);
        ASSERT_EQ(material.parameters[8].type, 2);
        ASSERT_EQ(material.parameters[8].valueFieldCount, 1);
        ASSERT_EQ(material.parameters[8].offset, 256);
        ASSERT_EQ(material.parameters[8].intValue, 1);
    } {
        ASSERT_EQ(material.parameters[9].name, 0x7FEE2D1A);
        ASSERT_EQ(material.parameters[9].type, 1);
        ASSERT_EQ(material.parameters[9].valueFieldCount, 4);
        ASSERT_EQ(material.parameters[9].offset, 260);
        ASSERT_FLOAT_EQ(material.parameters[9].colorValue.x, 1);
        ASSERT_FLOAT_EQ(material.parameters[9].colorValue.y, 1);
        ASSERT_FLOAT_EQ(material.parameters[9].colorValue.z, 1);
        ASSERT_FLOAT_EQ(material.parameters[9].colorValue.w, 1);
    } {
        ASSERT_EQ(material.parameters[10].name, 0x6CC0FD85);
        ASSERT_EQ(material.parameters[10].type, 4);
        ASSERT_EQ(material.parameters[10].valueFieldCount, 4);
        ASSERT_EQ(material.parameters[10].offset, 276);
        ASSERT_EQ(material.parameters[10].keyValue.instance, 0x7DEA5DE1);
        ASSERT_EQ(material.parameters[10].keyValue.type, 0xB2D882);
        ASSERT_EQ(material.parameters[10].keyValue.group, 0x00);
    }

    stream->Close();
    stream->Release();
}

TEST(Material, Write) {
    Material material;
    const auto readStream = new EA::IO::FileStream(TEST_ASSETS_DIR "0x4d114292!0x000000007dea5de1.Material");
    readStream->AddRef();
    const auto writeStream = new EA::IO::MemoryStream;
    writeStream->AddRef();
    Material::Read(material, readStream);
    material.Write(writeStream);

    const auto inBuffer =  msml::core::util::StreamUtil::ReadBytes(readStream);
    const auto outBuffer =  msml::core::util::StreamUtil::ReadBytes(writeStream);

    readStream->Close();
    readStream->Release();
    writeStream->Close();
    writeStream->Release();

    ASSERT_EQ(inBuffer.size(), outBuffer.size());
    EXPECT_THAT(inBuffer, ::testing::ElementsAreArray(outBuffer));
}
