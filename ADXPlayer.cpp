#include "ADXPlayer.h"
#include "ADXObject.h"

ADXPlayer::ADXPlayer(ADXObject* obj, ADXKeyBoardInput* setKeyboard, std::vector<int> setConfig)
{
	gameObject = obj;

	keyboard = setKeyboard;
	config = setConfig;
	prevPos = obj->transform.translation_;
	se = ADXAudio::SoundLoadWave("Resources/sound/jump.wav");
}

void ADXPlayer::UniqueUpdate()
{	
	velocity = gameObject->transform.translation_ - prevPos;
	prevPos = gameObject->transform.translation_;

	velocity *= 0.8;
	velocity.y /= 0.8;
	velocity.y -= 0.015;
	if (keyboard->KeyPress(config[0]))
	{
		velocity.z += 0.05;
	}
	if (keyboard->KeyPress(config[1]))
	{
		velocity.z -= 0.05;
	}
	if (keyboard->KeyPress(config[2]))
	{
		velocity.x += 0.05;
	}
	if (keyboard->KeyPress(config[3]))
	{
		velocity.x -= 0.05;
	}

	if (keyboard->KeyPress(config[4]) && !prevJump)
	{
		velocity.y = 0.4;
		se.SoundPlayWave();
	}
	if (!keyboard->KeyPress(config[4]) && velocity.y > 0 && prevJump)
	{
		velocity.y *= 0.2;
	}
	prevJump = keyboard->KeyPress(config[4]);

	gameObject->transform.translation_ += velocity;
	gameObject->transform.UpdateMatrix();
}