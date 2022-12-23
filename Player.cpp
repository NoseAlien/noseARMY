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

bool Player::GetInputStatus(int keyIndex)
{
	return keyboard->KeyPress(config[keyIndex]);
}

bool Player::GetInputStatusTrigger(int keyIndex)
{
	return keyboard->KeyTrigger(config[keyIndex]);
}

bool Player::GetInputStatusRelease(int keyIndex)
{
	return keyboard->KeyRelease(config[keyIndex]);
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

	if (keyboard->KeyPress(config[5]))
	{
		velocity *= 0.8f;
	}
	else
	{
		velocity.y /= 0.8f;
		velocity.y -= 0.015f;
		Move(0.05f, 0.4f);
	}

	VelocityUpdate();

	if (keyboard->KeyRelease(config[5]) && minis.size() < 10)
	{
		PlayerMini mini;
		ADXObject* miniObj = &mini;
		*miniObj = Duplicate(*this);
		mini.transform.scale_ *= 0.5f;
		mini.transform.translation_ = ADXMatrix4::transform({ 0,0,1.6f }, transform.matWorld_);
		mini.Initialize(this);
		minis.push_back(mini);
	}

	minis.remove_if([=](auto& itr) 
		{ return ADXMatrix4::transform(itr.transform.translation_, transform.matWorld_.Inverse()).length() > 60; });

	for (auto& itr : minis)
	{
		itr.Update();
	}
}