#pragma once
#include "ADXScene.h"

class TitleScene : public ADXScene
{
private:
	uint32_t titleImg = 0;
	uint32_t keyImg = 0;

	ADXObject* title = nullptr;
	ADXObject* key = nullptr;

	ADXModel rect{};

	ADXMaterial unlitMat{};

	ADXCamera* camera_ = nullptr;

public:
	void Initialize();
	void Update();
};