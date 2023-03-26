#include "ImguiManager.h"
#include "KDescriptorHeap.h"

void ImguiManager::Init()
{
    //ImGui�̃R���e�L�X�g�𐶐�
    ImGui::CreateContext();

    //ImGui�̃X�^�C����ݒ�
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(win->GetHWND());

    KDescriptorHeap::DescriptorHeapViewHandle handl = dx->GetSRVDescriptorHeap()->AddSRV();

    ImGui_ImplDX12_Init(
        dx->GetDev(),
        static_cast<int>(dx->GetBackBufferCount()),
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, dx->GetSRVDescriptorHeap()->GetHeap(),
        handl.cpuHandle,
        handl.gpuHandle
    );

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("Resources/Fonts/NotoSansJP-Bold.otf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
}

void ImguiManager::Finalize()
{
    // Cleanup
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
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
    ID3D12GraphicsCommandList* commandlist = dx->GetCmdlist();

    //�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
    ID3D12DescriptorHeap* ppHeaps[] = { dx->GetSRVDescriptorHeap()->GetHeap() };
    commandlist->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    //�`��R�}���h�𔭍s
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandlist);
}

void ImguiManager::End()
{
    // Rendering
    ImGui::Render();
}