#include "Player.h"

Player::Player()
{

}

void Player::Initialize(ADXKeyBoardInput* setKeyboard, std::vector<int> setConfig)
{
	keyboard = setKeyboard;
	config = setConfig;
	VelocityInitialize();
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

	if (keyboard->KeyTrigger(config[4]))
	{
		velocity.y = jumpPower;
		se.SoundPlayWave();
	}
	if (keyboard->KeyRelease(config[4]) && velocity.y > 0)
	{
		velocity.y *= 0.2;
	}
}

void Player::VelocityInitialize()
{
	prevPos = transform.translation_;
}

void Player::VelocityMove(float drag)
{
	velocity = transform.translation_ - prevPos;
	prevPos = transform.translation_;

	velocity *= drag;
}

void Player::VelocityUpdate()
{
	transform.translation_ += velocity;
	transform.UpdateMatrix();
}

void Player::UniqueUpdate()
{	
	VelocityMove(0.8f);

	velocity.y /= 0.8f;
	velocity.y -= 0.015f;

	Move(0.05f, 0.4f);

	if (keyboard->KeyPress(config[5]))
	{
		velocity = { 0,0,0 };
	}

	VelocityUpdate();

	if (keyboard->KeyRelease(config[5]) && minis.size() < 10)
	{
		PlayerMini mini;
		ADXObject* miniObj = &mini;
		*miniObj = Duplicate(*this);
		mini.transform.scale_ *= 0.5f;
		mini.Initialize(this);
		minis.push_back(mini);
	}

	for (auto& itr : minis)
	{
		itr.Update();
	}
}