#pragma once
#include "ADXScene.h"
#include "SceneTransition.h"

class TitleScene : public ADXScene
{
private:
	uint32_t titleImg_ = 0;
	uint32_t backGroundTex_ = 0;
	uint32_t keyImg_ = 0;

	ADXModel rect_{};

	ADXMaterial unlitMat_{};

	SceneTransition* shutter_ = nullptr;
	ADXObject* title_ = nullptr;
	ADXObject* key_ = nullptr;
	ADXObject* backGround_ = nullptr;
	ADXCamera* camera_ = nullptr;

public:
	void Initialize();
	void Update();
};