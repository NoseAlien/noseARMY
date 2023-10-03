#include "ADXComponent.h"

void ADXComponent::Update(ADXObject* obj)
{
	gameObject = obj;

	if (!initialized)
	{
		UniqueInitialize();
	}
	UniqueUpdate();
}