#include "ADXAudioListener.h"
#include "ADXObject.h"

ADXAudioListener* ADXAudioListener::S_current = nullptr;

void ADXAudioListener::UniqueUpdate()
{
	S_current = this;
}

void ADXAudioListener::SafetyPhase()
{
	if (S_current == this && GetGameObject()->GetDeleteFlag())
	{
		S_current = nullptr;
	}
}
