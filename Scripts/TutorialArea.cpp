#include "TutorialArea.h"

std::list<TutorialArea*> TutorialArea::allAreaPtr{};
std::list<TutorialArea*> TutorialArea::areas{};

TutorialArea::TutorialArea()
{

}

void TutorialArea::Initialize(uint32_t setTutorialImg)
{
	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger = true;
	tempCol->colType_ = box;

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
