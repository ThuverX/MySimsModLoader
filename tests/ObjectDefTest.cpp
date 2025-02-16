//INCOMPLETE

#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include "formats/ObjectDef.h"

// Mock XML data (simulating ObjectDef.xml)
const std::string MockObjectDef = R"(<?xml version="1.0" encoding="utf-8" ?>
<ObjectDef>
    <Model>flairSymbolMock</Model>
    <Rig>essenceSymbolRig-rig</Rig>
    <Script>Essence</Script>
    <HavokInfo>2</HavokInfo> <!--  1=Drop by gravity, 2=start with static --> 
    <NoFootPrint>1</NoFootPrint>  <!--  no footprint   --> 
    <RotateByHavok>0</RotateByHavok>  <!--  Havok update rotation  --> 
    <HavokLinearDamping>2.0f</HavokLinearDamping>
    <Shadow>0</Shadow>

    <EssenceDef>
        <Name>Mock</Name>
        <FlairModel>flairMock</FlairModel>
        <EssenceMaterial>matSetMock</EssenceMaterial>
        <Plantable>1</Plantable> <!-- 0 = Not Plantable, 1 = Gardens, 2 = Prospecting, 3 = Water Harvesting -->
        <Icon>uitexture-essence-flair-mock</Icon>
        <PaletteIcon>uitexture-essence-paint-mock</PaletteIcon>
    </EssenceDef>
    <Influences>
        <Interest type="Food">1.428</Interest>
    </Influences>
</ObjectDef>
)";


// Test case for ObjectDef file reading
TEST(ObjectDefTest, ReadFromStream) {
    // Simulating file input using istringstream
    std::istringstream file(MockObjectDef);

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

    // Create ObjectDef instance and call Read method
    ObjectDef objectDef;
    ObjectDef::Read(objectDef, buffer, size);

    // Validate that CharacterDef correctly parsed the data (assuming GetName() method exists)
    EXPECT_EQ(objectDef.Model, "flairSymbolMock");
    EXPECT_EQ(objectDef.Rig, "essenceSymbolRig-rig");
    EXPECT_EQ(objectDef.Script, "Essence");
    //EXPECT_EQ(objectDef.HavokInfo, "2");
    EXPECT_EQ(objectDef.NoFootPrint, "1");
    //EXPECT_EQ(objectDef.RotateByHavok, "0");
    //EXPECT_EQ(objectDef.HavokLinearDamping, "2.0f");
    //EXPECT_EQ(objectDef.Shadow, "0");

    //Commented lines do not exist in original file format?

/*

    // Validate Clips
    EXPECT_EQ(characterDef.Clips.size(), 2);
    EXPECT_EQ(characterDef.Clips.at("eIDLE2"), "a-idle-thighSlap");
    EXPECT_EQ(characterDef.Clips.at("eIDLE3"), "a-idle-wave");

    // Validate Influences
    EXPECT_EQ(characterDef.Influences.Interests.size(), 3);
    EXPECT_EQ(characterDef.Influences.Interests[0].Type, "Food");
    EXPECT_EQ(characterDef.Influences.Interests[0].Value, 5);
    EXPECT_EQ(characterDef.Influences.Interests[1].Type, "SciFi");
    EXPECT_EQ(characterDef.Influences.Interests[1].Value, -1);
    EXPECT_EQ(characterDef.Influences.Interests[2].Type, "Fun");
    EXPECT_EQ(characterDef.Influences.Interests[2].Value, 2);*/

    // Cleanup allocated memory
    delete[] static_cast<char *>(buffer);
}