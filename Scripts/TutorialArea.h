#pragma once

#include "ADXObject.h"

class TutorialArea : public ADXComponent
{
private:
	ADXObject* tutorialWindow = nullptr;
	uint32_t tutorialImg{};
	ADXModel rect{};

private:
	static std::list<TutorialArea*> allAreaPtr;
	static std::list<TutorialArea*> areas;

public:
	TutorialArea();
	void Initialize(uint32_t setTutorialImg);
	uint32_t GetTutorialImg() { return tutorialImg; };

private:
	void UniqueUpdate();

public:
	static void StaticUpdate();
	static std::list<TutorialArea*> GetAreas() { return areas; };
};