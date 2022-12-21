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

void ADXPlayer::Update(ADXObject* obj)
{
	gameObject = obj;
	
	velocity = obj->transform.translation_ - prevPos;
	prevPos = obj->transform.translation_;

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

	obj->transform.translation_ += velocity;
	obj->transform.UpdateMatrix();
}