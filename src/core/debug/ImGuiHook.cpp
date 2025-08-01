//
// Created by exozg on 13/06/2025.
//

#include "ImGuiHook.h"

#include "DebugUI.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include "../../EA/GraphicDevice.h"

bool ShowMenu = false;
bool ImGui_Initialised = false;

namespace Process {
    HWND Hwnd;
    WNDPROC WndProc;
}

namespace DirectX12Interface {
    ID3D12Device *Device = nullptr;
    ID3D12DescriptorHeap *DescriptorHeapBackBuffers;
    ID3D12DescriptorHeap *DescriptorHeapImGuiRender;
    ID3D12GraphicsCommandList *CommandList;

    struct _FrameContext {
        ID3D12CommandAllocator *CommandAllocator;
        ID3D12Resource *Resource;
        D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle;
    };

    int BuffersCounts = -1;
    _FrameContext *FrameContext;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (ShowMenu && ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) {
        return true;
    }
    return CallWindowProc(Process::WndProc, hwnd, uMsg, wParam, lParam);
}

void DescriptorAlloc(ImGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_desc_handle)
{
    *out_cpu_desc_handle = DirectX12Interface::DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart();
    *out_gpu_desc_handle = DirectX12Interface::DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart();
}

void DescriptorFree(ImGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_desc_handle) {

}

// TODO: Check if we're only doing this on one of the frames...
void hkPresent(Queue *pQueue, const QueuePresentDesc *pDesc) {
    if (!Process::Hwnd) {
        if (auto device = TSS::Graphics::GetDevice()) {
            Process::Hwnd = device->hwnd;
            DirectX12Interface::Device = device->pRenderer->mDx.pDevice;
        }
    }

    if (!Process::Hwnd) { return d3d12_queuePresentHook.Original(pQueue, pDesc); }

    if (!DirectX12Interface::Device) { return d3d12_queuePresentHook.Original(pQueue, pDesc); }

    auto pSwapChain = pDesc->pSwapChain->mDx.pSwapChain;
    if (!pSwapChain) { return d3d12_queuePresentHook.Original(pQueue, pDesc); }
    auto commandQueue = pQueue->mDx.pQueue;
    if (!commandQueue) { return d3d12_queuePresentHook.Original(pQueue, pDesc); }

    if (!ImGui_Initialised) {
        ImGui::CreateContext();

        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        DirectX12Interface::BuffersCounts = pDesc->pSwapChain->mImageCount;
        DirectX12Interface::FrameContext = new DirectX12Interface::_FrameContext[DirectX12Interface::BuffersCounts];

        D3D12_DESCRIPTOR_HEAP_DESC DescriptorImGuiRender = {};
        DescriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        DescriptorImGuiRender.NumDescriptors = DirectX12Interface::BuffersCounts;
        DescriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

        if (DirectX12Interface::Device->CreateDescriptorHeap(&DescriptorImGuiRender,
                                                             IID_PPV_ARGS(
                                                                 &DirectX12Interface::DescriptorHeapImGuiRender)) !=
            S_OK)
            return d3d12_queuePresentHook.Original(pQueue, pDesc);

        ID3D12CommandAllocator *Allocator;
        if (DirectX12Interface::Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator))
            != S_OK)
            return d3d12_queuePresentHook.Original(pQueue, pDesc);

        for (size_t i = 0; i < DirectX12Interface::BuffersCounts; i++) {
            DirectX12Interface::FrameContext[i].CommandAllocator = Allocator;
        }

        if (DirectX12Interface::Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator, NULL,
                                                          IID_PPV_ARGS(&DirectX12Interface::CommandList)) != S_OK ||
            DirectX12Interface::CommandList->Close() != S_OK)
            return d3d12_queuePresentHook.Original(pQueue, pDesc);

        D3D12_DESCRIPTOR_HEAP_DESC DescriptorBackBuffers;
        DescriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        DescriptorBackBuffers.NumDescriptors = DirectX12Interface::BuffersCounts;
        DescriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        DescriptorBackBuffers.NodeMask = 1;

        if (DirectX12Interface::Device->CreateDescriptorHeap(&DescriptorBackBuffers,
                                                             IID_PPV_ARGS(
                                                                 &DirectX12Interface::DescriptorHeapBackBuffers)) !=
            S_OK)
            return d3d12_queuePresentHook.Original(pQueue, pDesc);

        const auto RTVDescriptorSize = DirectX12Interface::Device->GetDescriptorHandleIncrementSize(
            D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = DirectX12Interface::DescriptorHeapBackBuffers->
                GetCPUDescriptorHandleForHeapStart();

        for (size_t i = 0; i < DirectX12Interface::BuffersCounts; i++) {
            ID3D12Resource *pBackBuffer = nullptr;
            DirectX12Interface::FrameContext[i].DescriptorHandle = RTVHandle;
            pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
            DirectX12Interface::Device->CreateRenderTargetView(pBackBuffer, nullptr, RTVHandle);
            DirectX12Interface::FrameContext[i].Resource = pBackBuffer;
            RTVHandle.ptr += RTVDescriptorSize;
        }

        ImGui_ImplWin32_Init(Process::Hwnd);

        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.NodeMask = 1;


        ImGui_ImplDX12_InitInfo init_info = {};
        init_info.Device = DirectX12Interface::Device;
        init_info.NumFramesInFlight = DirectX12Interface::BuffersCounts;
        init_info.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
        init_info.SrvDescriptorHeap = DirectX12Interface::DescriptorHeapImGuiRender;
        init_info.SrvDescriptorAllocFn = &DescriptorAlloc;
        init_info.SrvDescriptorFreeFn = &DescriptorFree;

        HRESULT hr = DirectX12Interface::Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&init_info.CommandQueue));
        IM_ASSERT(SUCCEEDED(hr));

        ImGui_ImplDX12_Init(&init_info);
        // ImGui_ImplDX12_Init(DirectX12Interface::Device, DirectX12Interface::BuffersCounts, DXGI_FORMAT_R8G8B8A8_UNORM,
        //                     DirectX12Interface::DescriptorHeapImGuiRender,
        //                     DirectX12Interface::DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(),
        //                     DirectX12Interface::DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());
        ImGui_ImplDX12_CreateDeviceObjects();
        Process::WndProc = (WNDPROC) SetWindowLongPtr(Process::Hwnd, GWLP_WNDPROC, (__int3264) (LONG_PTR) WndProc);
        ImGui_Initialised = true;

        Msml::Core::DebugUI::GetInstance().Init();
    }


    if (GetAsyncKeyState(VK_F10) & 1) ShowMenu = !ShowMenu;

    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::GetIO().MouseDrawCursor = ShowMenu;
    SetCursorLock(ShowMenu);
    if (ShowMenu == true) {
        Msml::Core::DebugUI::GetInstance().Draw();
    }
    ImGui::EndFrame();

    DirectX12Interface::_FrameContext &CurrentFrameContext = DirectX12Interface::FrameContext[pSwapChain->
        GetCurrentBackBufferIndex()];
    CurrentFrameContext.CommandAllocator->Reset();

    D3D12_RESOURCE_BARRIER Barrier;
    Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    Barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    Barrier.Transition.pResource = CurrentFrameContext.Resource;
    Barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

    DirectX12Interface::CommandList->Reset(CurrentFrameContext.CommandAllocator, nullptr);
    DirectX12Interface::CommandList->ResourceBarrier(1, &Barrier);
    DirectX12Interface::CommandList->OMSetRenderTargets(1, &CurrentFrameContext.DescriptorHandle, FALSE, nullptr);
    DirectX12Interface::CommandList->SetDescriptorHeaps(1, &DirectX12Interface::DescriptorHeapImGuiRender);

    ImGui::Render();
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectX12Interface::CommandList);
    Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    DirectX12Interface::CommandList->ResourceBarrier(1, &Barrier);
    DirectX12Interface::CommandList->Close();
    commandQueue->
            ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList * const*>(&DirectX12Interface::CommandList));

    return d3d12_queuePresentHook.Original(pQueue, pDesc);
}
