#pragma once
#include "ADXScene.h"
#include "SceneTransition.h"

class TitleScene : public ADXScene
{
private:
	uint32_t titleImg = 0;
	uint32_t keyImg = 0;

	ADXModel rect{};

	ADXMaterial unlitMat{};

	SceneTransition* shutter_ = nullptr;
	ADXObject* title = nullptr;
	ADXObject* key = nullptr;
	ADXCamera* camera_ = nullptr;

public:
	void Initialize();
	void Update();
};