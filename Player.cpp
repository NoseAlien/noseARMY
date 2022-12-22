#include "Player.h"
#include "PlayerMini.h"

Player::Player()
{

}

void Player::Initialize(ADXKeyBoardInput* setKeyboard, std::vector<int> setConfig)
{
	keyboard = setKeyboard;
	config = setConfig;
	prevPos = transform.translation_;
	se = ADXAudio::SoundLoadWave("Resources/sound/jump.wav");
}

void Player::Move(float walkSpeed, float jumpPower)
{
	if (keyboard->KeyPress(config[0]))
	{
		velocity.z += walkSpeed;
	}
	if (keyboard->KeyPress(config[1]))
	{
		velocity.z -= walkSpeed;
	}
	if (keyboard->KeyPress(config[2]))
	{
		velocity.x += walkSpeed;
	}
	if (keyboard->KeyPress(config[3]))
	{
		velocity.x -= walkSpeed;
	}

	if (keyboard->KeyPress(config[4]) && !prevJump)
	{
		velocity.y = jumpPower;
		se.SoundPlayWave();
	}
	if (!keyboard->KeyPress(config[4]) && velocity.y > 0 && prevJump)
	{
		velocity.y *= 0.2;
	}
	prevJump = keyboard->KeyPress(config[4]);
}

void Player::VelocityMove(float drag, float dropSpeed)
{
	velocity = transform.translation_ - prevPos;
	prevPos = transform.translation_;

	velocity *= drag;
	velocity.y /= drag;
	velocity.y -= dropSpeed;
}

void Player::UniqueUpdate()
{	
	VelocityMove(0.8f, 0.015f);
	Move(0.05f, 0.4f);

	transform.translation_ += velocity;
	transform.UpdateMatrix();

	if (keyboard->KeyTrigger(config[5]))
	{
		ADXObject mini;
		ADXObject* miniObj = &mini;
		*miniObj = Duplicate(*this);
		minis.push_back(mini);
	}

	for (auto& itr : minis)
	{
		itr.Update();
	}
}