#pragma once

#include "ADXObject.h"

class TutorialArea : public ADXComponent
{
private:
	uint32_t tutorialImg_ = 0;

public:
	void SetTutorialImg(uint32_t setTutorialImg_);
	uint32_t GetTutorialImg() { return tutorialImg_; };

private:
	void UniqueInitialize();
};