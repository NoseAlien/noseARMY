#include "ADXComponent.h"

void ADXComponent::Update(ADXObject* obj)
{
	gameObject = obj;

	if (gameObject != nullptr)
	{
		if (!initialized)
		{
			UniqueInitialize();
		}
		UniqueUpdate();
	}
}

void ADXComponent::SetGameObject(ADXObject* obj)
{
	if (gameObject == nullptr)
	{
		gameObject = obj;
	}
}
