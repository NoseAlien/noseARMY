#include "ADXComponent.h"

void ADXComponent::Initialize(ADXObject* obj)
{
	gameObject = obj;

	UniqueInitialize();
}

void ADXComponent::Update(ADXObject* obj)
{
	gameObject = obj;

	UniqueUpdate();
}