#include "ADXComponent.h"

void ADXComponent::Update(ADXObject* obj)
{
	gameObject = obj;

	if (gameObject != nullptr)
	{
		if (!initialized)
		{
			UniqueInitialize();
			initialized = true;
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
