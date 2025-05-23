cmake_minimum_required(VERSION 3.10)
project(MySimsModLoader)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)

# Disable sigmatch tests to speed up compilation
set(SIGMATCH_BUILD_TESTS OFF CACHE BOOL "Disable sigmatch tests" FORCE)

add_subdirectory(libs/minhook)
add_subdirectory(libs/sigmatch)
add_subdirectory(libs/pugixml)

set(SOURCES
        src/Main.cpp
        src/core/modloader/ModLoader.cpp
        src/core/modloader/ModLoader.h
        src/core/system/Logger.cpp
        src/core/system/Logger.h
        src/core/system/Console.cpp
        src/core/system/Console.h
        src/core/assets/Assets.cpp
        src/core/assets/Assets.h
        src/core/modloader/Mods.cpp
        src/core/modloader/Mods.h
        src/core/hooks/Hooks.cpp
        src/core/hooks/Hooks.h
        src/core/signatures/Signatures.cpp
        src/core/signatures/Signatures.h
        src/core/modloader/Mod.cpp
        src/core/modloader/Mod.h
        src/core/assets/Asset.cpp
        src/core/assets/Asset.h
        src/core/hash/FNV.cpp
        src/core/hash/FNV.h
        src/include/lua.h
        src/core/signatures/sigdef.h
        src/core/hooks/LuaHook.cpp
        src/core/hooks/LuaHook.h
        src/include/hash_sha256.h
        src/core/resource/IdResolver.cpp
        src/core/resource/IdResolver.h
        src/core/hooks/Config.h
        src/formats/materials/Material.h
        src/formats/materials/Material.cpp
        src/formats/xml/CharacterDef.h
        src/formats/xml/CharacterDef.cpp
        src/formats/xml/ObjectDef.h
        src/formats/xml/ObjectDef.cpp
        src/formats/xml/World.h
        src/formats/xml/World.cpp
        src/core/system/CrashHandler.cpp
        src/core/system/CrashHandler.h
        src/core/resource/CustomDatabase.cpp
        src/core/resource/CustomDatabase.h
        src/EA/io/FileStream.cpp
        src/EA/io/FileStream.h
        src/EA/io/IStream.h
        src/tweakers/Swarm/SwarmTweaker.h
        src/tweakers/Swarm/SwarmTweaker.cpp
        src/EA/io/MemoryStream.cpp
        src/EA/io/MemoryStream.h
        src/tweakers/Tweakers.h
        src/EA/ResourceMan/KeyFilter.h
        src/EA/ResourceMan/RecordInfo.h
        src/core/resource/CustomRecord.cpp
        src/core/resource/CustomRecord.h
        src/EA/ResourceMan/IDatabase.h
        src/EA/ResourceMan/IRecord.h
        src/EA/IO/Constants.h
        src/EA/IO/SubFileStream.cpp
        src/EA/IO/SubFileStream.h
        src/tweakers/Clothing/ClothingTweaker.cpp
        src/tweakers/Clothing/ClothingTweaker.h
        src/EA/ResourceMan/ResourceKey.cpp
        src/formats/materials/MaterialParameter.cpp
        src/include/refpack.h
        src/core/hooks/ArgscriptHook.cpp
        src/core/hooks/ArgscriptHook.h
        src/formats/materials/MaterialSet.cpp
        src/formats/materials/MaterialSet.h
        src/formats/builders/MaterialBuilder.cpp
        src/formats/builders/MaterialBuilder.h
        src/formats/builders/MaterialSetBuilder.cpp
        src/formats/builders/MaterialSetBuilder.h
)

if (CMAKE_GENERATOR_PLATFORM STREQUAL "x64")
    list(APPEND SOURCES
            src/platform/w64/wsock.def
            src/platform/w64/wsock.h)

else ()
    list(APPEND SOURCES
            src/platform/w32/dsound.def
            src/platform/w32/dsound.h)
endif ()

add_library(MySimsModLoader SHARED ${SOURCES})

add_custom_target(version
        ${CMAKE_COMMAND} -D SRC=${CMAKE_SOURCE_DIR}/src/Version.h.in
        -D DST=${CMAKE_SOURCE_DIR}/src/Version.h
        -P ${CMAKE_SOURCE_DIR}/GenerateVersion.cmake
)

add_dependencies(MySimsModLoader version)

add_compile_definitions(CHAR8_T_DEFINED)

add_definitions(-D_CHAR16T)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_SCL_SECURE_NO_WARNINGS)
add_definitions(-DEASTL_OPENSOURCE=1)

add_definitions(-DPROJECT_ROOT="${CMAKE_SOURCE_DIR}")

target_include_directories(MySimsModLoader PRIVATE
        libs/eastl/include
        libs/eastl/test/packages/EABase/include/Common
)

target_include_directories(MySimsModLoader PRIVATE libs/pugixml/src)
target_include_directories(MySimsModLoader PRIVATE libs/minhook/include)
target_include_directories(MySimsModLoader PRIVATE libs/sigmatch/include)

target_link_libraries(MySimsModLoader PRIVATE minhook)
target_link_libraries(MySimsModLoader PRIVATE pugixml-static)

set_target_properties(MySimsModLoader PROPERTIES OUTPUT_NAME "dsound")

if (CMAKE_GENERATOR_PLATFORM STREQUAL "x64")
    set_target_properties(MySimsModLoader PROPERTIES OUTPUT_NAME "WSOCK32")
endif ()

if (BUILD_TESTS)
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    # As per documentation: http://google.github.io/googletest/quickstart-cmake.html
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(libs/googletest)

    enable_testing()

    add_executable(Tests
            # Tests
            tests/CharacterDefTest.cpp
            tests/MaterialTest.cpp
            tests/ObjectDefTest.cpp
            # Format
            src/formats/xml/CharacterDef.h
            src/formats/xml/CharacterDef.cpp
            src/formats/xml/ObjectDef.h
            src/formats/xml/ObjectDef.cpp
            src/formats/materials/Material.h
            src/formats/materials/Material.cpp
            src/formats/materials/MaterialParameter.h
            src/formats/materials/MaterialParameter.cpp
            src/formats/materials/MaterialSet.h
            src/formats/materials/MaterialSet.cpp
            # EA
            src/EA/IO/MemoryStream.h
            src/EA/IO/MemoryStream.cpp
            src/EA/IO/FileStream.h
            src/EA/IO/FileStream.cpp
            src/EA/ResourceMan/ResourceKey.h
            src/EA/ResourceMan/ResourceKey.cpp
            # Utilities
            src/core/system/Logger.h
            src/core/system/Logger.cpp
            tests/MaterialSetTest.cpp)

    target_include_directories(Tests PRIVATE
            libs/eastl/include
            libs/eastl/test/packages/EABase/include/Common
    )

    # Set a define to indicate that we are running unit tests
    target_compile_definitions(Tests PRIVATE -DUNIT_TESTING)

    target_include_directories(Tests PRIVATE libs/pugixml/src)
    target_include_directories(Tests PRIVATE src)

    target_compile_definitions(Tests PRIVATE
            TEST_ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/assets/"
    )

    target_link_libraries(Tests PRIVATE
            GTest::gmock
            GTest::gtest_main
            pugixml-static)

    include(GoogleTest)
    gtest_discover_tests(Tests)
endif ()