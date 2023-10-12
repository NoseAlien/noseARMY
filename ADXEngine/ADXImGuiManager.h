#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "ADXWindow.h"

class ADXImGuiManager
{
public:
	void Initialize(ADXWindow* window);
	void Finalize();

	void Begin();
	void End();
	void StaticDraw();

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_ = nullptr;
};