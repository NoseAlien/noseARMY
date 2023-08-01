#pragma once
#include "ADXScene.h"

class TitleScene : public ADXScene
{
private:
	uint32_t titleImg = 0;
	uint32_t keyImg = 0;

	ADXObject title{};
	ADXObject key{};

	ADXModel rect{};

	ADXMaterial unlitMat{};

	ADXCamera camera_{};

	std::list<ADXObject*> objs{};

public:
	TitleScene();
	void Initialize();
	void Update();
};