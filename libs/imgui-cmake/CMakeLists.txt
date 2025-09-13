cmake_minimum_required(VERSION 3.14)
project(imgui_dx12_win32)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(IMGUI_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../imgui)

set(IMGUI_SRC
        ${IMGUI_SOURCE_DIR}/imgui.cpp
        ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
        ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
        ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
        ${IMGUI_SOURCE_DIR}/imgui_demo.cpp
        ${IMGUI_SOURCE_DIR}/backends/imgui_impl_win32.cpp
        ${IMGUI_SOURCE_DIR}/backends/imgui_impl_dx12.cpp
)

add_library(imgui STATIC
        ${IMGUI_SRC}
)

target_include_directories(imgui PUBLIC
        ${IMGUI_SOURCE_DIR}
        ${IMGUI_SOURCE_DIR}/backends
)

if (WIN32)
    target_link_libraries(imgui
            PUBLIC
            d3d12
            dxgi
            dxguid
            user32
            gdi32
    )
endif()