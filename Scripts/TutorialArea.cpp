#include "TutorialArea.h"

void TutorialArea::SetTutorialImg(uint32_t setTutorialImg)
{
	tutorialImg = setTutorialImg;
}

void TutorialArea::UniqueInitialize()
{
	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger_ = true;
	tempCol->colType_ = box;
}