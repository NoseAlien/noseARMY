#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <DirectXMath.h>

class ADXMaterial
{
public:
	DirectX::XMFLOAT3 ambient_ = { 1.0f,1.0f,1.0f };
	DirectX::XMFLOAT3 diffuse_ = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 specular_ = { 0.0f,0.0f,0.0f };
	float alpha_ = 1.0f;

private:
	std::string name_ = "";

public:
	static ADXMaterial LoadMaterial(const std::string& filePath);
};