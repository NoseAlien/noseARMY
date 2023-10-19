#pragma once

#include "ADXObject.h"

class TutorialArea : public ADXComponent
{
private:
	uint32_t tutorialImg = 0;

public:
	void SetTutorialImg(uint32_t setTutorialImg);
	uint32_t GetTutorialImg() { return tutorialImg; };

private:
	void UniqueInitialize();
};