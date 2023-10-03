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