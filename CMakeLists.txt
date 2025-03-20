cmake_minimum_required(VERSION 3.10)
project(MySimsModLoader)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable sigmatch tests to speed up compilation
set(SIGMATCH_BUILD_TESTS OFF CACHE BOOL "Disable sigmatch tests" FORCE)

add_subdirectory(libs/minhook)
add_subdirectory(libs/sigmatch)
add_subdirectory(libs/pugixml)

set(SOURCES
        src/Main.cpp
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
        src/hooks/LuaHook.cpp
        src/hooks/LuaHook.h
        src/include/hash_sha256.h
        src/hashes/Hashes.cpp
        src/hashes/Hashes.h
        src/tweakers/SwarmTweaker/SwarmTweaker.cpp
        src/tweakers/SwarmTweaker/SwarmTweaker.h
        src/tweakers/Tweaker.cpp
        src/tweakers/Tweaker.h
        src/Common.h
)

if(CMAKE_GENERATOR_PLATFORM STREQUAL "x64")
    list(APPEND SOURCES
            src/platform/w64/wsock.def
            src/platform/w64/wsock.h)

else ()
    list(APPEND SOURCES
            src/platform/w32/dsound.h)
endif ()

add_library(MySimsModLoader SHARED ${SOURCES})

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

if(CMAKE_GENERATOR_PLATFORM STREQUAL "x64")
    set_target_properties(MySimsModLoader PROPERTIES OUTPUT_NAME "WSOCK32")
endif()