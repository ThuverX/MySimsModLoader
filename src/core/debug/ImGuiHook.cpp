//
// Created by exozg on 13/06/2025.
//

#include "ImGuiHook.h"

#include "DebugUI.h"


#include "../../EA/GraphicDevice.h"
#include "../signatures/sigdef.h"

#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Msml::Core {

    static Cmd* pCmd;
    static Queue* pImguiQueue;
    static CmdPool* pCmdPool;
    static ID3D12DescriptorHeap* pImGuiHeap;
    static bool mIsInitialized;
    static WNDPROC ogWndProc;

    void AllocateDescriptor(ImGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_desc_handle) {
        *out_cpu_desc_handle = pImGuiHeap->GetCPUDescriptorHandleForHeapStart();
        *out_gpu_desc_handle = pImGuiHeap->GetGPUDescriptorHandleForHeapStart();
    }

    void FreeDescriptor(ImGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_desc_handle) {
    }


    LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (DebugUI::GetInstance().mIsVisible && (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam) != 0)) {
            return TRUE;
        }
        return CallWindowProc(ogWndProc, hwnd, uMsg, wParam, lParam);
    }

    void HkPresent(Queue *pQueue, const QueuePresentDesc *pDesc) {
        const auto *graphicsDevice = TSS::Graphics::GetDevice();
        auto *pRenderer = graphicsDevice->pRenderer;
        auto *device = graphicsDevice->pRenderer->mDx.pDevice;

        if (!mIsInitialized) {
            ImGui::CreateContext();

            ImGuiIO &io = ImGui::GetIO();
            io.WantCaptureMouse = true;
            io.WantTextInput = true;
            io.WantCaptureKeyboard = true;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

            ImGui_ImplWin32_Init(graphicsDevice->hwnd);
            ogWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(graphicsDevice->hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc)));

            D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
            heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            heapDesc.NumDescriptors = pDesc->pSwapChain->mImageCount;
            heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

            if (FAILED(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&pImGuiHeap)))) {
                MSML_LOG_ERROR("Failed to create descriptor heap");
                queuePresentHook.Original(pQueue, pDesc);
                return;
            }

            ImGui_ImplDX12_InitInfo initInfo = {};
            initInfo.Device = device;
            initInfo.NumFramesInFlight = pDesc->pSwapChain->mImageCount;
            initInfo.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
            initInfo.SrvDescriptorHeap = pImGuiHeap;
            initInfo.SrvDescriptorAllocFn = &AllocateDescriptor;
            initInfo.SrvDescriptorFreeFn = &FreeDescriptor;
            initInfo.CommandQueue = pQueue->mDx.pQueue;

            QueueDesc queueDesc = {};
            queueDesc.mType = QUEUE_TYPE_GRAPHICS;
            queueDesc.mFlag = QUEUE_FLAG_NONE;
            queueDesc.mNodeIndex = 0;

            addQueue(pRenderer, &queueDesc, &pImguiQueue);

            if (pImguiQueue == nullptr) {
                MSML_LOG_ERROR("Failed to create queue");
                queuePresentHook.Original(pQueue, pDesc);
                return;
            }

            initInfo.CommandQueue = pImguiQueue->mDx.pQueue;

            ImGui_ImplDX12_Init(&initInfo);
            ImGui_ImplDX12_CreateDeviceObjects();

            CmdPoolDesc poolDesc = {};
            poolDesc.pQueue = pImguiQueue;
            addCmdPool(pRenderer, &poolDesc, &pCmdPool);

            CmdDesc cmdDesc = {};
            cmdDesc.pPool = pCmdPool;
            addCmd(pRenderer, &cmdDesc, &pCmd);

            DebugUI::GetInstance().Init();

            mIsInitialized = true;
        }

        ImGui_ImplDX12_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        DebugUI::GetInstance().Draw();

        ImGui::Render();
        ImGui::UpdatePlatformWindows();

        resetCmdPool(pRenderer, pCmdPool);
        beginCmd(pCmd);

        const uint32_t kFrameIdx = pDesc->pSwapChain->mDx.pSwapChain->GetCurrentBackBufferIndex();
        RenderTarget* pRT = pDesc->pSwapChain->ppRenderTargets[kFrameIdx];

        BindRenderTargetDesc renderTargetDesc = {};
        renderTargetDesc.pRenderTarget = pRT;
        renderTargetDesc.mLoadAction = LOAD_ACTION_LOAD;

        BindRenderTargetsDesc bindDesc = {};
        bindDesc.mRenderTargetCount = 1;
        bindDesc.mRenderTargets[0] = renderTargetDesc;

        cmdBindRenderTargets(pCmd, &bindDesc);

        cmdSetViewport(pCmd, 0.0F, 0.0F, static_cast<float>(pRT->mWidth), static_cast<float>(pRT->mHeight), 0.0f, 1.0f);
        cmdSetScissor(pCmd, 0, 0, pRT->mWidth, pRT->mHeight);

        ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), pCmd->mDx.pCmdList);

        endCmd(pCmd);

        ID3D12CommandList* ppCommandLists[] = { pCmd->mDx.pCmdList };
        pQueue->mDx.pQueue->ExecuteCommandLists(1, ppCommandLists);

        queuePresentHook.Original(pQueue, pDesc);
    }

    void ImGuiHook::Install() {
        // WriteLogHook.Install(&writeLogHooked);
        queuePresentHook.Install(&HkPresent);
    }
}
