#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <DirectXMath.h>

class ADXMaterial
{
public:
	std::string name = "";
	DirectX::XMFLOAT3 ambient = { 1.0f,1.0f,1.0f };
	DirectX::XMFLOAT3 diffuse = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 specular = { 0.0f,0.0f,0.0f };
	float alpha = 1.0f;
	std::string textureFilename = "";

public:
	static ADXMaterial LoadMaterial(const std::string& filePath);
};