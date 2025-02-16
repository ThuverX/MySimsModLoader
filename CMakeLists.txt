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

add_library(MySimsModLoader SHARED src/Main.cpp
        src/dsound.h
        src/modloader/ModLoader.cpp
        src/modloader/ModLoader.h
        src/util/Logger.cpp
        src/util/Logger.h
        src/util/Console.cpp
        src/util/Console.h
        src/assets/Assets.cpp
        src/assets/Assets.h
        src/mods/Mods.cpp
        src/mods/Mods.h
        src/Hooks/Hooks.cpp
        src/Hooks/Hooks.h
        src/signatures/Signatures.cpp
        src/signatures/Signatures.h
        src/mods/Mod.cpp
        src/mods/Mod.h
        src/util/File.cpp
        src/util/File.h
        src/assets/Asset.cpp
        src/assets/Asset.h
        src/util/FNV.cpp
        src/util/FNV.h
        src/signatures/lua.h
        src/signatures/sigdef.h
        # Format
        src/formats/CharacterDef.h
        src/formats/CharacterDef.cpp
        src/formats/World.h
        src/formats/World.cpp
)

add_custom_target(version
        ${CMAKE_COMMAND} -D SRC=${CMAKE_SOURCE_DIR}/src/Version.h.in
        -D DST=${CMAKE_SOURCE_DIR}/src/Version.h
        -P ${CMAKE_SOURCE_DIR}/GenerateVersion.cmake
)

add_dependencies(MySimsModLoader version)

target_include_directories(MySimsModLoader PRIVATE libs/pugixml/src)
target_include_directories(MySimsModLoader PRIVATE libs/minhook/include)
target_include_directories(MySimsModLoader PRIVATE libs/sigmatch/include)

target_link_libraries(MySimsModLoader PRIVATE minhook)
target_link_libraries(MySimsModLoader PRIVATE pugixml-static)

set_target_properties(MySimsModLoader PROPERTIES OUTPUT_NAME "dsound")

if (BUILD_TESTS)
        # For Windows: Prevent overriding the parent project's compiler/linker settings
        # As per documentation: http://google.github.io/googletest/quickstart-cmake.html
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        add_subdirectory(libs/googletest)

        enable_testing()

        add_executable(Tests
                # Tests
                tests/CharacterDefTest.cpp
                # Format
                src/formats/CharacterDef.h
                src/formats/CharacterDef.cpp
                # Utilities
                src/util/Logger.h
                src/util/Logger.cpp)

        # Set a define to indicate that we are running unit tests
        target_compile_definitions(Tests PRIVATE -DUNIT_TESTING)

        target_include_directories(Tests PRIVATE libs/pugixml/src)
        target_include_directories(Tests PRIVATE src)

        target_link_libraries(Tests PRIVATE GTest::gtest_main pugixml-static)

        include(GoogleTest)
        gtest_discover_tests(Tests)
endif()
