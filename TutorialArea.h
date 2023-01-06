#pragma once

#include "ADXObject.h"

class TutorialArea : public ADXObject
{
private:
	ADXObject tutorialWindow{};
	ADXImage tutorialImg{};
	ADXModel rect{};

private:
	static std::list<TutorialArea*> allAreaPtr;
	static std::list<TutorialArea*> areas;

public:
	TutorialArea();
	void Initialize(ADXImage setTutorialImg);
	ADXImage GetTutorialImg() { return tutorialImg; };

private:
	void UniqueUpdate();

public:
	static void StaticUpdate();
	static std::list<TutorialArea*> GetAreas() { return areas; };
};