#include "ADXComponent.h"

void ADXComponent::Update(ADXObject* obj)
{
	gameObject_ = obj;

	if (gameObject_ != nullptr)
	{
		if (!initialized_)
		{
			UniqueInitialize();
			initialized_ = true;
		}
		UniqueUpdate();
	}
}

void ADXComponent::SetGameObject(ADXObject* obj)
{
	if (gameObject_ == nullptr)
	{
		gameObject_ = obj;
	}
}
