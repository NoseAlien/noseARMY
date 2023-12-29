#include "ADXImGuiManager.h"
#include "ADXCommon.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

void ADXImGuiManager::Initialize(ADXWindow* window)
{
	//ImGuiのコンテキストを生成
	ImGui::CreateContext();
	//ImGuiのスタイルを設定
	ImGui::StyleColorsDark();
	//ImGuiの初期化
	ImGui_ImplWin32_Init(window->GetHwnd());

	HRESULT result;

	//デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//デスクリプタヒープ生成
	result = ADXCommon::GetCurrentInstance()->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	//DirectX12用初期化
	ImGui_ImplDX12_Init(
		ADXCommon::GetCurrentInstance()->GetDevice(),
		(uint32_t)ADXCommon::GetCurrentInstance()->GetBackBufferCount(),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart());

	ImGuiIO& io = ImGui::GetIO();
	//標準フォントを追加する
	io.Fonts->AddFontDefault();
}

void ADXImGuiManager::Finalize()
{
	//各種終了処理
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	//デスクリプタヒープを解放
	srvHeap_.Reset();
}

void ADXImGuiManager::PreDraw()
{
	//ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ADXImGuiManager::PostDraw()
{
	//描画前準備
	ImGui::Render();
}

void ADXImGuiManager::StaticDraw()
{
	ID3D12GraphicsCommandList* commandList = ADXCommon::GetCurrentInstance()->GetCommandList();

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//描画コマンドを実行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}
