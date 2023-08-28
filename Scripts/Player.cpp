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
	rigidbody.Initialize(this);
	jumpSE = ADXAudio::SoundLoadWave("Resources/sound/jump.wav");
	damageSE = ADXAudio::SoundLoadWave("Resources/sound/damage.wav");
	windowOpenSE = ADXAudio::SoundLoadWave("Resources/sound/windowOpen.wav");

	noseImage = ADXImage::LoadADXImage("apEGnoSE.png");
	furImage = ADXImage::LoadADXImage("apEG_fur.png");

	rect = ADXModel::CreateRect();
	playerModel = ADXModel::LoadModel("model/sphere.obj");

	model = &playerModel;
	texture = furImage;

	colliders.push_back(ADXCollider(this));
	colliders.back().pushable_ = true;

	nose.Initialize();
	nose.transform.localPosition_ = { 0,0,1.01f };
	nose.transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,3.1415f,0 });
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
	ADXVector3 cameraRight = camera->transform.TransformPointOnlyRotation({ 1,0,0 });
	cameraRight = cameraRight.Normalize();
	ADXVector3 cameraForward = camera->transform.TransformPointOnlyRotation({ 0,0,1 });
	cameraForward.y = 0;
	cameraForward = cameraForward.Normalize();

	if (keyboard->KeyPress(config[0]) || keyboard->KeyPress(config[1]) || keyboard->KeyPress(config[2]) || keyboard->KeyPress(config[3]))
	{
		if (keyboard->KeyPress(config[0]))
		{
			rigidbody.velocity += cameraForward * walkSpeed;
		}
		if (keyboard->KeyPress(config[1]))
		{
			rigidbody.velocity -= cameraForward * walkSpeed;
		}
		if (keyboard->KeyPress(config[2]))
		{
			rigidbody.velocity += cameraRight * walkSpeed;
		}
		if (keyboard->KeyPress(config[3]))
		{
			rigidbody.velocity -= cameraRight * walkSpeed;
		}
		transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,atan2(rigidbody.velocity.x, rigidbody.velocity.z),0 });
	}

	if (keyboard->KeyTrigger(config[4]))
	{
		rigidbody.velocity.y = jumpPower;
		jumpSE.SoundPlayWave();
	}
	if (keyboard->KeyRelease(config[4]) && rigidbody.velocity.y > 0)
	{
		rigidbody.velocity.y *= 0.2f;
	}
}

void Player::SpeciesUpdate()
{	
	float scale = ADXUtility::ValueMapping((float)minis.size(), 0, 20, 1, 0.25f);
	transform.localScale_ = { scale,scale,scale };

	ADXVector3 cameraVec = camera->transform.GetWorldPosition() - transform.GetWorldPosition();
	cameraVec.y = 0;
	cameraVec = cameraVec.Normalize();
	camera->transform.localPosition_ = transform.localPosition_ + cameraVec * 20;
	camera->transform.localPosition_.y += 5;
	camera->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,atan2(-cameraVec.x, -cameraVec.z),0 });
	camera->transform.UpdateMatrix();
	camera->transform.localRotation_ = ADXQuaternion::Multiply(camera->transform.localRotation_, ADXQuaternion::MakeAxisAngle({ 1,0,0 }, 0.3f));

	bool moveInput = 
		!keyboard->KeyPress(config[0]) || keyboard->KeyPress(config[1]) || keyboard->KeyPress(config[2]) || keyboard->KeyPress(config[3]);

	rigidbody.drag = 0.8f;

	rigidbody.VelocityMove();

	if (keyboard->KeyPress(config[5]))
	{
		rigidbody.velocity *= 0.8f;
		rigidbody.gravityScale = 0;
	}
	else
	{
		rigidbody.gravityScale = 0.01f;
		rigidbody.dragAxis.y = false;
		Move(0.05f, 0.4f);
	}

	rigidbody.Update(this);

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
								splitable = false;
								return true;
							}
						}
					}
				}
		return false; });
	}

	minis.remove_if([=](auto& itr)
		{ return ADXMatrix4::Transform(itr.transform.localPosition_, transform.GetMatWorldInverse()).Length() > 30 / scale; });

	nose.transform.localScale_ = ADXVector3{ 0.42f,0.35f,0.35f } * (float)fmax(1,1 + pow(fmax(0, splitInterval),2) * 0.02f);

	//nose.transform.localEulerAngles_.z *= 0.9f;

	splitInterval--;
	splitInterval = max(-20, splitInterval);

	if (splitable && keyboard->KeyRelease(config[5]) && splitInterval <= 0)
	{
		nose.transform.localScale_ = { 0.42f,0.35f,0.35f };
		nose.transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,3.1415f,0 });
		if (minis.size() < 20)
		{
			PlayerMini mini;
			ADXObject* miniObj = &mini;
			*miniObj = Duplicate(*this);
			mini.transform.localScale_ = { 0.5f,0.5f,0.5f };
			mini.transform.localPosition_ = ADXMatrix4::Transform({ 0,0,1 }, transform.GetMatWorld());

			mini.Initialize(this, nose);
			minis.push_back(mini);
		}
		//nose.transform.localEulerAngles_.z = 10;
		splitInterval = 7;
	}

	if (!keyboard->KeyPress(config[5]))
	{
		splitable = true;
	}

	for (auto& itr : minis)
	{
		itr.Update();
		SetAttackObj({&itr.colliders.back(), this, (float)minis.size() });
	}
	nose.Update();

	uint32_t prevTutorialImg = tutorialWindow.texture;
	uint32_t setTutorialImg = prevTutorialImg;
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
					&& (transform.GetWorldPosition() - colItr2->ClosestPoint(transform.GetWorldPosition())).Length() < 0.1)
				{
					isOutOfField = false;
				}
			}
		}
	}
	if (prevTutorialImg != setTutorialImg)
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

	tutorialWindow.transform.localScale_ = ADXUtility::Lerp({ 0,0.3f,0 }, { 0.3f / ADXWindow::GetAspect(),0.3f,0}, ADXUtility::EaseOut(tutorialWindowExAmount, 4));
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