#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "ADXWindow.h"

//ImGuiを扱うクラス
class ADXImGuiManager
{
private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_ = nullptr;

public:
	//初期化処理
	void Initialize(ADXWindow* window);

	//終了処理
	void Finalize();

	//描画開始処理
	void PreDraw();

	//描画終了処理
	void PostDraw();

	//描画処理
	void StaticDraw();

private:
	//コンストラクタ、デストラクタをprivateにしてシングルトンに

	ADXImGuiManager() {};
	~ADXImGuiManager() {};

	ADXImGuiManager(const ADXImGuiManager&) = delete;
	ADXImGuiManager& operator=(const ADXImGuiManager&) = delete;


private:
	static ADXImGuiManager S_instance;

public:
	static ADXImGuiManager* GetInstance() { return &S_instance; }
};