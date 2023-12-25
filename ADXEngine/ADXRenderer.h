#pragma once

#include "ADXComponent.h"
#include "ADXMaterial.h"

class ADXRenderer : public ADXComponent
{
public:
	ADXMaterial material_{};

public:
	void Rendering();

private:
	virtual void UniqueRendering([[maybe_unused]]ID3D12Device* device, [[maybe_unused]] ID3D12GraphicsCommandList* cmdList) {};
};