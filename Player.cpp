#include "Player.h"
#include "ADXUtility.h"
#include "FieldBox.h"
#include <time.h>

Player::Player()
{

}

void Player::Initialize(ADXKeyBoardInput* setKeyboard, std::vector<int> setConfig, ADXCamera* setCamera)
{
	keyboard = setKeyboard;
	config = setConfig;
	VelocityInitialize();
	jumpSE = ADXAudio::SoundLoadWave("Resources/sound/jump.wav");
	damageSE = ADXAudio::SoundLoadWave("Resources/sound/damage.wav");
	windowOpenSE = ADXAudio::SoundLoadWave("Resources/sound/windowOpen.wav");

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
	nose.transform.localPosition_ = { 0,0,1.01f };
	nose.transform.localEulerAngles_ = { 0,3.1415f,0 };
	nose.transform.localScale_ = { 0.42f,0.35f,0.35f };
	nose.transform.parent_ = &transform;
	nose.transform.UpdateMatrix();
	nose.model = &rect;
	nose.texture = noseImage;
	nose.material = material;

	tutorialWindow.Initialize();
	tutorialWindow.transform.rectTransform = true;
	tutorialWindow.transform.UpdateMatrix();
	tutorialWindow.model = &rect;
	tutorialWindow.texture = ADXImage::LoadADXImage("WhiteDot.png");
	tutorialWindow.material = material;
	tutorialWindow.renderLayer = 1;

	outOfField.Initialize();
	outOfField.transform.rectTransform = true;
	outOfField.transform.UpdateMatrix();
	outOfField.model = &rect;
	outOfField.texture = ADXImage::LoadADXImage("outOfField.png");
	outOfField.material = material;
	outOfField.renderLayer = 1;

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
		transform.localEulerAngles_.y = atan2(velocity.x, velocity.z);
	}

	if (keyboard->KeyTrigger(config[4]))
	{
		velocity.y = jumpPower;
		jumpSE.SoundPlayWave();
	}
	if (keyboard->KeyRelease(config[4]) && velocity.y > 0)
	{
		velocity.y *= 0.2f;
	}
}

void Player::VelocityInitialize()
{
	prevPos = transform.localPosition_;
}

void Player::VelocityMove(float drag)
{
	velocity = transform.localPosition_ - prevPos;
	prevPos = transform.localPosition_;

	velocity *= drag;
}

void Player::VelocityUpdate()
{
	transform.localPosition_ += velocity;
	transform.UpdateMatrix();
}

void Player::SpeciesUpdate()
{	
	ADXVector3 cameraVec = camera->transform.localPosition_ - transform.localPosition_;
	cameraVec.y = 0;
	cameraVec = ADXVector3::normalized(cameraVec);
	camera->transform.localPosition_ = transform.localPosition_ + cameraVec * 20;
	camera->transform.localPosition_.y += 5;
	camera->transform.localEulerAngles_.y = atan2(-cameraVec.x, -cameraVec.z);

	cameraRight = ADXMatrix4::transform({ 1,0,0 }, camera->transform.matRot_);
	cameraForward = ADXMatrix4::transform({ 0,0,1 }, camera->transform.matRot_);
	cameraForward.y = 0;
	cameraForward = ADXVector3::normalized(cameraForward);


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

	nose.transform.localScale_ = ADXVector3{ 0.42f,0.35f,0.35f } * (float)fmax(1,1 + pow(fmax(0, splitInterval),2) * 0.02f);

	nose.transform.localEulerAngles_.z *= 0.9f;

	splitInterval--;
	splitInterval = max(-20, splitInterval);

	if (keyboard->KeyRelease(config[5]) && splitInterval <= 0)
	{
		nose.transform.localScale_ = { 0.42f,0.35f,0.35f };
		nose.transform.localEulerAngles_ = { 0,3.1415f,0 };
		if (minis.size() < 20)
		{
			PlayerMini mini;
			ADXObject* miniObj = &mini;
			*miniObj = Duplicate(*this);
			mini.transform.localScale_ *= 0.7f;
			mini.transform.localPosition_ = ADXMatrix4::transform({ 0,0,1.8f }, transform.matWorld_);

			mini.Initialize(this, nose);
			minis.push_back(mini);
		}
		nose.transform.localEulerAngles_.z = 10;
		splitInterval = 7;
	}

	minis.remove_if([=](auto& itr) 
		{ return ADXMatrix4::transform(itr.transform.localPosition_, transform.matWorld_.Inverse()).length() > 60; });

	for (auto& itr : minis)
	{
		itr.Update();
		SetAttackObj({&itr.colliders.back(), (float)minis.size() });
	}
	nose.Update();

	ADXImage prevTutorialImg = tutorialWindow.texture;
	ADXImage setTutorialImg = prevTutorialImg;
	bool windowExtend = false;
	bool isOutOfField = true;
	
	for (auto& colItr : colliders)
	{
		for (auto& colItr2 : colItr.GetCollideList())
		{
			for (auto& objItr : TutorialArea::GetAreas())
			{
				if (colItr2->GetGameObject() == objItr)
				{
					if (!windowClosing)
					{
						windowExtend = true;
						setTutorialImg = objItr->GetTutorialImg();
					}
				}
			}
			for (auto& objItr : FieldBox::GetFields())
			{
				if (colItr2->GetGameObject() == objItr 
					&& (transform.GetWorldPosition() - colItr2->ClosestPoint(transform.GetWorldPosition())).length() < 0.1)
				{
					isOutOfField = false;
				}
			}
		}
	}
	if (prevTutorialImg.GetGHandle() != setTutorialImg.GetGHandle())
	{
		windowExtend = false;
	}
	if (windowOpening)
	{
		windowExtend = true;
	}


	bool prevwindowOpening = windowOpening;
	bool prevwindowClosing = windowClosing;
	if (windowExtend)
	{
		tutorialWindowExAmount += 0.1f;
		windowOpening = true;
	}
	else
	{
		tutorialWindowExAmount -= 0.1f;
		windowClosing = true;
	}
	tutorialWindowExAmount = max(0,min(tutorialWindowExAmount,1));

	if (tutorialWindowExAmount >= 1 || tutorialWindowExAmount <= 0)
	{
		windowOpening = false;
		windowClosing = false;
	}


	if (windowOpening && !prevwindowOpening)
	{
		windowOpenSE.SoundPlayWave();
	}

	if (!windowClosing)
	{
		tutorialWindow.texture = setTutorialImg;
	}

	tutorialWindow.transform.localScale_ = ADXUtility::Lerp({ 0,0.3f,0 }, { 0.3f,0.3f,0 },ADXUtility::EaseOut(tutorialWindowExAmount,4));
	tutorialWindow.transform.localPosition_ = { 0.65f,-0.65f + sin(clock() * 0.002f) * 0.01f,0};

	tutorialWindow.Update();

	if (isOutOfField)
	{
		outOfField.transform.localScale_ = { 0.4f,0.7f,0 };
	}
	else
	{
		outOfField.transform.localScale_ = { 0,0,0 };
	}

	outOfField.transform.localPosition_ = { -0.6f,0.65f + sin(clock() * 0.003f) * 0.01f,0 };

	outOfField.Update();
}