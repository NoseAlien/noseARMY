#include "TutorialArea.h"

std::list<TutorialArea*> TutorialArea::allAreaPtr{};
std::list<TutorialArea*> TutorialArea::areas{};

void TutorialArea::SetTutorialImg(uint32_t setTutorialImg)
{
	tutorialImg = setTutorialImg;
}

void TutorialArea::UniqueInitialize()
{
	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger = true;
	tempCol->colType_ = box;
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
