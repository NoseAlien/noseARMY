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
	noseImage = ADXImage::LoadADXImage("apEGnoSE.png");
	furImage = ADXImage::LoadADXImage("apEG_fur.png");

	rect.vertices = {
	{{-1.0f,-1.0f,0.0f},{}, {0.0f,1.0f}},//左下
	{{-1.0f,1.0f,0.0f},{},{0.0f,0.0f}},//左上
	{{1.0f,-1.0f,0.0f},{},{1.0f,1.0f}},//右下
	{{1.0f,1.0f,0.0f},{},{1.0f,0.0f}},//右上
	};
	//インデックスデータ
	rect.indices =
	{
		0,1,2,
		2,1,3,

		1,0,2,
		1,2,3,
	};
	rect.Initialize();

	texture = furImage;

	nose.Initialize();
	nose.transform.translation_ = { 0,0,1.01f };
	nose.transform.rotation_ = { 0,3.1415f,0 };
	nose.transform.scale_ = { 0.42,0.35,0.35 };
	nose.transform.parent_ = &transform;
	nose.transform.UpdateMatrix();
	nose.model = &rect;
	nose.texture = noseImage;
	nose.material = material;
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
	if (keyboard->KeyPress(config[0]) || keyboard->KeyPress(config[1]) || keyboard->KeyPress(config[2]) || keyboard->KeyPress(config[3]))
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
		transform.rotation_.y = atan2(velocity.x, velocity.z);
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
	bool moveInput = 
		!keyboard->KeyPress(config[0]) || keyboard->KeyPress(config[1]) || keyboard->KeyPress(config[2]) || keyboard->KeyPress(config[3]);

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

		minis.remove_if([=](auto& itr)
			{
				for (auto& colItr : colliders)
				{
					for (auto& colListItr : colItr.collideList)
					{
						for (auto& colItr2 : itr.colliders)
						{
							if (colListItr == &colItr2)
							{
								return true;
							}
						}
					}
				}
		return false; });

	if (keyboard->KeyRelease(config[5]) && minis.size() < 20)
	{
		PlayerMini mini;
		ADXObject* miniObj = &mini;
		*miniObj = Duplicate(*this);
		mini.transform.scale_ *= 0.7f;
		mini.transform.translation_ = ADXMatrix4::transform({ 0,0,1.8f }, transform.matWorld_);

		mini.Initialize(this, nose);
		minis.push_back(mini);
	}

	minis.remove_if([=](auto& itr) 
		{ return ADXMatrix4::transform(itr.transform.translation_, transform.matWorld_.Inverse()).length() > 60; });

	for (auto& itr : minis)
	{
		itr.Update();
	}
	nose.Update();
}