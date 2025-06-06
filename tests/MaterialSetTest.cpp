﻿//
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

    const auto stream = new EA::IO::FileStream(TEST_ASSETS_DIR "0xdd91919d!0x00000000db272b16.MaterialSet");
    stream->AddRef();
    MaterialSet::Read(materialSet, stream);

    ASSERT_EQ(materialSet.numMaterials, 5);
    ASSERT_EQ(materialSet.materials.size(), 5);

    ASSERT_EQ(materialSet.self.instance, 0xDB272B16);
    ASSERT_EQ(materialSet.self.type, 0x2019972);
    ASSERT_EQ(materialSet.self.group, 0xDD91919D);

    {
        ASSERT_EQ(materialSet.materials[0].instance, 0xDFFF58C4);
        ASSERT_EQ(materialSet.materials[0].type, 0x1D0E75D);
        ASSERT_EQ(materialSet.materials[0].group, 0xDB272B16);
    }

    {
        ASSERT_EQ(materialSet.materials[1].instance, 0x4F4D01B1);
        ASSERT_EQ(materialSet.materials[1].type, 0x1D0E75D);
        ASSERT_EQ(materialSet.materials[1].group, 0xDB272B16);
    }

    {
        ASSERT_EQ(materialSet.materials[2].instance, 0x71B2317C);
        ASSERT_EQ(materialSet.materials[2].type, 0x1D0E75D);
        ASSERT_EQ(materialSet.materials[2].group, 0xDB272B16);
    }

    {
        ASSERT_EQ(materialSet.materials[3].instance, 0xEE3C68FD);
        ASSERT_EQ(materialSet.materials[3].type, 0x1D0E75D);
        ASSERT_EQ(materialSet.materials[3].group, 0xDB272B16);
    }

    {
        ASSERT_EQ(materialSet.materials[4].instance, 0x8EC04A9);
        ASSERT_EQ(materialSet.materials[4].type, 0x1D0E75D);
        ASSERT_EQ(materialSet.materials[4].group, 0xDB272B16);
    }

    ASSERT_EQ(materialSet.headerSize, 124);
    ASSERT_EQ(materialSet.mtstSize, 40);

    ASSERT_EQ(materialSet.mtst.name, 0xDD91919D);
    ASSERT_EQ(materialSet.mtst.index, 0x30000001);
    ASSERT_EQ(materialSet.mtst.count, 5);

    ASSERT_EQ(materialSet.mtst.indices[0], 805306370);
    ASSERT_EQ(materialSet.mtst.indices[1], 805306371);
    ASSERT_EQ(materialSet.mtst.indices[2], 805306369);
    ASSERT_EQ(materialSet.mtst.indices[3], 805306372);
    ASSERT_EQ(materialSet.mtst.indices[4], 805306373);
}

TEST(MaterialSet, Write) {
    MaterialSet materialSet;
    const auto readStream = new EA::IO::FileStream(TEST_ASSETS_DIR "0xdd91919d!0x00000000db272b16.MaterialSet");
    readStream->AddRef();
    const auto writeStream = new EA::IO::MemoryStream;
    writeStream->AddRef();
    MaterialSet::Read(materialSet, readStream);
    materialSet.Write(writeStream);

    const auto inBuffer =  msml::core::util::StreamUtil::ReadBytes(readStream);
    const auto outBuffer =  msml::core::util::StreamUtil::ReadBytes(writeStream);

    readStream->Close();
    readStream->Release();
    writeStream->Close();
    writeStream->Release();

    ASSERT_EQ(inBuffer.size(), outBuffer.size());
    EXPECT_THAT(inBuffer, ::testing::ElementsAreArray(outBuffer));
}