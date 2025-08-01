#pragma once
#include "Graphics/Interfaces/IGraphics.h"

using undefined = char;

#pragma pack(push, 1)

struct Surface {
};

struct GraphicDevice {
    undefined _pad0[328];
    undefined some_mutex4;
    undefined _pad1[111];
    undefined some_mutex2;
    undefined _pad2[183];
    Renderer *pRenderer;
    Queue *pQueue;
    HWND hwnd;
    undefined _pad3[16];
    Semaphore semaphore[2];
    undefined _pad4[168];
    CmdPool *pCmdPool;
    undefined _pad5[6672];
    CmdPool *cmdPool;
    Cmd **pCmdList2;
    undefined _pad6[16];
    Cmd *pCmdList;
    undefined _pad7[1316];
    int window_width;
    int window_height;
    undefined _pad8[124];
    bool is_still_drawing;
    undefined field8947_0x2359;
    undefined field8948_0x235a;
    undefined field8949_0x235b;
    undefined field8950_0x235c;
    undefined field8951_0x235d;
    undefined field8952_0x235e;
    undefined field8953_0x235f;
    Surface *main_depth_buffer_surface_target;
    Surface *msaa_back_buffer;
    Surface *main_depth_buffer_render_target;
    undefined _pad9[824];
    undefined some_mutex3;
    undefined _pad10[79];
    undefined some_mutex1;
    undefined _pad11[327];
    int window_width1;
    int window_height1;
};

#pragma pack(pop)
