#include "Player.h"
#include "ADXUtility.h"
#include <time.h>

Player::Player()
{

}

void Player::Initialize(ADXKeyBoardInput* setKeyboard, std::vector<int> setConfig, ADXCamera* setCamera)
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

	tutorialWindow.Initialize();
	tutorialWindow.transform.translation_ = { 0.65,-0.65,0 };
	tutorialWindow.transform.scale_ = { 0,0,0 };
	tutorialWindow.transform.rectTransform = true;
	tutorialWindow.transform.UpdateMatrix();
	tutorialWindow.model = &rect;
	tutorialWindow.texture = ADXImage::LoadADXImage("WhiteDot.png");
	tutorialWindow.material = material;
	tutorialWindow.renderLayer = 1;

	camera = setCamera;
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
			velocity += cameraForward * walkSpeed;
		}
		if (keyboard->KeyPress(config[1]))
		{
			velocity -= cameraForward * walkSpeed;
		}
		if (keyboard->KeyPress(config[2]))
		{
			velocity += cameraRight * walkSpeed;
		}
		if (keyboard->KeyPress(config[3]))
		{
			velocity -= cameraRight * walkSpeed;
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
	ADXVector3 cameraVec = camera->transform.translation_ - transform.translation_;
	cameraVec.y = 0;
	cameraVec = ADXVector3::normalized(cameraVec);
	camera->transform.translation_ = transform.translation_ + cameraVec * 20;
	camera->transform.translation_.y += 5;
	camera->transform.rotation_.y = atan2(-cameraVec.x, -cameraVec.z);

	cameraRight = ADXMatrix4::transform({ 1,0,0 }, camera->transform.matRot_);
	cameraForward = ADXMatrix4::transform({ 0,0,1 }, camera->transform.matRot_);
	cameraForward.y = 0;
	cameraForward = ADXVector3::normalized(cameraForward);


	colliders.back().pushBackPriority = 1;

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

	if (splitInterval <= -20)
	{
		minis.remove_if([=](auto& itr)
			{
				for (auto& colItr : colliders)
				{
					for (auto& colListItr : colItr.GetCollideList())
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
	}

	nose.transform.scale_ = ADXVector3{ 0.42,0.35,0.35 } * max(1,1 + pow(max(0, splitInterval),2) * 0.02);

	nose.transform.rotation_.z *= 0.9f;

	splitInterval--;
	splitInterval = max(-20, splitInterval);

	if (keyboard->KeyRelease(config[5]) && splitInterval <= 0)
	{
		nose.transform.scale_ = { 0.42,0.35,0.35 };
		nose.transform.rotation_ = { 0,3.1415f,0 };
		if (minis.size() < 20)
		{
			PlayerMini mini;
			ADXObject* miniObj = &mini;
			*miniObj = Duplicate(*this);
			mini.transform.scale_ *= 0.7f;
			mini.transform.translation_ = ADXMatrix4::transform({ 0,0,1.8f }, transform.matWorld_);

			mini.Initialize(this, nose);
			minis.push_back(mini);
		}
		nose.transform.rotation_.z = 10;
		splitInterval = 7;
	}

	minis.remove_if([=](auto& itr) 
		{ return ADXMatrix4::transform(itr.transform.translation_, transform.matWorld_.Inverse()).length() > 60; });

	for (auto& itr : minis)
	{
		itr.Update();
	}
	nose.Update();

	bool windowExtend = false;
	for (auto& objItr : TutorialArea::GetAreas())
	{
		for (auto& colItr : colliders)
		{
			for (auto& colItr2 : colItr.GetCollideList())
			{
				if (colItr2->GetGameObject() == objItr)
				{
					tutorialWindow.texture = objItr->GetTutorialImg();
					windowExtend = true;
				}
			}
		}
	}

	if (windowExtend)
	{
		tutorialWindowExAmount += 0.1;
	}
	else
	{
		tutorialWindowExAmount -= 0.1;
	}
	tutorialWindowExAmount = max(0,min(tutorialWindowExAmount,1));

	tutorialWindow.transform.scale_ = ADXUtility::Lerp({ 0,0.3,0 }, { 0.3,0.3,0 },ADXUtility::EaseOut(tutorialWindowExAmount,4));
	tutorialWindow.transform.translation_ = { 0.65,-0.65f + sin(clock() * 0.002f) * 0.01f,0};

	tutorialWindow.Update();
}