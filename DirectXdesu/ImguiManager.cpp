#include "ImguiManager.h"

void ImguiManager::Init()
{
    HRESULT result;

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    desc.NumDescriptors = 1;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    result = dx->GetDev()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descRipterHeap));
    assert(SUCCEEDED(result));

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(win->GetHWND());
    ImGui_ImplDX12_Init(dx->GetDev(), dx->GetBackBufferSize(),
        DXGI_FORMAT_R8G8B8A8_UNORM, descRipterHeap.Get(),
        descRipterHeap->GetCPUDescriptorHandleForHeapStart(),
        descRipterHeap->GetGPUDescriptorHandleForHeapStart());
}

void ImguiManager::Finalize()
{
    // Cleanup
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    descRipterHeap.Reset();
}

void ImguiManager::Update()
{
   
}

void ImguiManager::Begin()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImguiManager::Draw()
{
    ID3D12DescriptorHeap* tmpDesc[] = { descRipterHeap.Get() };
    dx->GetCmdlist()->SetDescriptorHeaps(_countof(tmpDesc), tmpDesc);
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dx->GetCmdlist());
}

void ImguiManager::End()
{
    // Rendering
    ImGui::Render();
}
