#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include "formats/CharacterDef.h"

// Mock XML data (simulating CharacterDef.xml)
const std::string MockCharacterDef = R"(
<?xml version="1.0" encoding="utf-8"?>
<CharacterDef>
	<Script>NPC_Mock</Script>
	<BodyModel>amBodyMock</BodyModel>
	<BodyDM>_white</BodyDM>
	<HeadModel>amHeadHatMock</HeadModel>
	<HairIndex>0</HairIndex>
	<HatIndex>0</HatIndex>
	<EyesModel>amEyesMock</EyesModel>
	<MouthModel>amMouthMock</MouthModel>
	<SkinToneIndex>0</SkinToneIndex>
	<MiscOpIndex>0</MiscOpIndex>
	<MiscModIndex>0</MiscModIndex>
	<VoxGroup>F</VoxGroup>
	<PitchAdjustment>0.85</PitchAdjustment>
	<Rig>skeleton-rig</Rig>
	<CollisionInfo>0 1 0 0.5 2 0.5</CollisionInfo>
	<Clips>
		<eIDLE2>a-idle-thighSlap</eIDLE2>
		<eIDLE3>a-idle-wave</eIDLE3>
	</Clips>
	<LuaData>
		<!-- Lua data is currently not being tested -->
	</LuaData>
	<Influences>
		<Interest type="Food">5</Interest>
		<Interest type="SciFi">-1</Interest>
		<Interest type="Fun">2</Interest>
	</Influences>
</CharacterDef>
)";

// Test case for CharacterDef file reading
TEST(CharacterDefTest, ReadFromStream) {
    // Simulating file input using istringstream
    std::istringstream file(MockCharacterDef);

    // Ensure stream is valid
    ASSERT_TRUE(file.good()) << "Mock file stream failed to open";

    void *buffer = nullptr;
    size_t size = 0;

    // Get size of mock data
    file.seekg(0, std::ios::end);
    size = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ios::beg);

    // Allocate memory
    buffer = new char[size];
    file.read(static_cast<char *>(buffer), size);

    // Ensure the data is read correctly
    ASSERT_EQ(file.gcount(), static_cast<std::streamsize>(size)) << "File read size mismatch";

    // Create CharacterDef instance and call Read method
    CharacterDef characterDef;
    CharacterDef::Read(characterDef, buffer, size);

    // Validate that CharacterDef correctly parsed the data (assuming GetName() method exists)
    EXPECT_EQ(characterDef.Script, "NPC_Mock");
    EXPECT_EQ(characterDef.BodyModel, "amBodyMock");
    EXPECT_EQ(characterDef.BodyDM, "_white");
    EXPECT_EQ(characterDef.HeadModel, "amHeadHatMock");
    EXPECT_EQ(characterDef.HairIndex, 0);
    EXPECT_EQ(characterDef.HatIndex, 0);
    EXPECT_EQ(characterDef.EyesModel, "amEyesMock");
    EXPECT_EQ(characterDef.MouthModel, "amMouthMock");
    EXPECT_EQ(characterDef.SkinToneIndex, 0);
    EXPECT_EQ(characterDef.MiscOpIndex, 0);
    EXPECT_EQ(characterDef.MiscModIndex, 0);

    // TODO: Add missing fields here
    
    // Cleanup allocated memory
    delete[] static_cast<char *>(buffer);
}
