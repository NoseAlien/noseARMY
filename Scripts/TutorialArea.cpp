#include "TutorialArea.h"

void TutorialArea::SetTutorialImg(uint32_t setTutorialImg_)
{
	tutorialImg_ = setTutorialImg_;
}

void TutorialArea::UniqueInitialize()
{
	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger_ = true;
	tempCol->colType_ = ADXCollider::box;
}