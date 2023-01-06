#include "TutorialArea.h"

std::list<TutorialArea*> TutorialArea::allAreaPtr{};
std::list<TutorialArea*> TutorialArea::areas{};

TutorialArea::TutorialArea()
{

}

void TutorialArea::Initialize(ADXImage setTutorialImg)
{
	tutorialImg = setTutorialImg;
}

void TutorialArea::UniqueUpdate()
{
	allAreaPtr.push_back(this);
}

void TutorialArea::StaticUpdate()
{
	areas = allAreaPtr;
	allAreaPtr.clear();
}
