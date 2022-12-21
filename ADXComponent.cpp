#include "ADXComponent.h"

void ADXComponent::Update(ADXObject* obj)
{
	gameObject = obj;

	UniqueUpdate();
}

void ADXComponent::UniqueUpdate()
{

}