#include "Player.h"
#include "ADXUtility.h"
#include "FieldBox.h"
#include "SceneTransition.h"
#include <time.h>
#include <imgui.h>

Player::Player()
{

}

void Player::Initialize(ADXKeyBoardInput* setKeyboard, std::vector<int> setConfig, ADXCamera* setCamera)
{
	keyboard = setKeyboard;
	config = setConfig;
	rigidbody.Initialize(this);
	jumpSE = ADXAudio::LoadADXAudio("sound/jump.wav");
	damageSE = ADXAudio::LoadADXAudio("sound/damage.wav");
	defeatSE = ADXAudio::LoadADXAudio("sound/despawn.wav");
	windowOpenSE = ADXAudio::LoadADXAudio("sound/windowOpen.wav");

	rect = ADXModel::CreateRect();
	playerModel = ADXModel::LoadADXModel("model/sphere.obj");

	model = &playerModel;
	texture = ADXImage::LoadADXImage("apEG_fur.png");

	colliders.push_back(ADXCollider(this));
	colliders.back().pushable_ = true;

	nose.Initialize();
	nose.transform.localPosition_ = { 0,0,1.01f };
	nose.transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,3.1415f,0 });
	nose.transform.localScale_ = { 0.42f,0.35f,0.35f };
	nose.transform.parent_ = &transform;
	nose.transform.UpdateMatrix();
	nose.model = &rect;
	nose.texture = ADXImage::LoadADXImage("apEGnoSE.png");
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

	gameOverFilter.Initialize();
	gameOverFilter.transform.rectTransform = true;
	gameOverFilter.transform.UpdateMatrix();
	gameOverFilter.model = &rect;
	gameOverFilter.texture = ADXImage::LoadADXImage("WhiteDot.png");
	gameOverFilter.material = material;
	gameOverFilter.renderLayer = 1;

	dead.Initialize();
	dead.transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,3.1415f,0 });
	dead.transform.parent_ = &transform;
	dead.transform.UpdateMatrix();
	dead.model = &rect;
	dead.texture = ADXImage::LoadADXImage("apEGnoSE_dead.png");
	dead.material = material;

	keyUI.Initialize();
	keyUI.transform.rectTransform = true;
	keyUI.model = &rect;
	keyUI.renderLayer = 5;
	keyUI.texture = ADXImage::LoadADXImage("PRESS_SPACE.png");


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
		jumpSE.Play();
	}
	if (keyboard->KeyRelease(config[4]) && rigidbody.velocity.y > 0)
	{
		rigidbody.velocity.y *= 0.2f;
	}
}

void Player::LiveEntitiesUpdate()
{
	renderLayer = 0;
	nose.renderLayer = 0;

	deadAnimationProgress = 0;

	float scale = ADXUtility::ValueMapping((float)minis.size(), 0, (float)maxMinisNum, 1, 0.25f);
	transform.localScale_ = { scale,scale,scale };

	transform.modelScale_ = { 1 + sinf((float)clock() * 0.001f) * 0.03f,1 + cosf((float)clock() * 0.001f) * 0.03f,1 + sinf((float)clock() * 0.001f) * 0.03f };

	ADXVector3 cameraVec = camera->transform.GetWorldPosition() - transform.GetWorldPosition();
	cameraVec.y = 0;
	cameraVec = cameraVec.Normalize();
	camera->transform.localPosition_ = transform.localPosition_ + cameraVec * 20;
	camera->transform.localPosition_.y += 5;
	camera->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,atan2(-cameraVec.x, -cameraVec.z),0 });
	camera->transform.UpdateMatrix();
	camera->transform.localRotation_ = camera->transform.localRotation_ * ADXQuaternion::MakeAxisAngle({ 1,0,0 }, 0.3f);

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
						for (auto& colItr2 : itr->colliders)
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
		{ return ADXMatrix4::Transform(itr->transform.localPosition_, transform.GetMatWorldInverse()).Length() > 30 / scale; });

	nose.transform.localScale_ = ADXVector3{ 0.42f,0.35f,0.35f } *(float)fmax(1, 1 + pow(fmax(0, splitInterval), 2) * 0.02f);
	nose.transform.localPosition_ = { 0,sinf((float)clock() * 0.001f) * 0.03f,1.01f + sinf((float)clock() * 0.001f) * 0.03f };

	splitInterval--;
	splitInterval = max(-20, splitInterval);

	if (splitable && keyboard->KeyRelease(config[5]) && splitInterval <= 0)
	{
		nose.transform.localScale_ = { 0.42f,0.35f,0.35f };
		nose.transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,3.1415f,0 });
		if (minis.size() < maxMinisNum)
		{
			std::unique_ptr<PlayerMini, ADXUtility::NPManager<PlayerMini>> mini(new PlayerMini);
			ADXObject* miniObj = mini.get();
			*miniObj = Duplicate(*this);
			mini->transform.localScale_ = { 0.5f,0.5f,0.5f };
			mini->transform.localPosition_ = ADXMatrix4::Transform({ 0,0,1 }, transform.GetMatWorld());

			mini->Initialize(this, nose);
			minis.push_back(move(mini));
		}
		splitInterval = 7;
	}

	if (!keyboard->KeyPress(config[5]))
	{
		splitable = true;
	}

	for (auto& itr : minis)
	{
		itr->Update();
		SetAttackObj({ &itr->colliders.back(), this, (float)minis.size() });
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
	tutorialWindowExAmount = max(0, min(tutorialWindowExAmount, 1));

	if (tutorialWindowExAmount >= 1 || tutorialWindowExAmount <= 0)
	{
		windowOpening = false;
		windowClosing = false;
	}


	if (windowOpening && !prevwindowOpening)
	{
		windowOpenSE.Play();
	}

	if (!windowClosing)
	{
		tutorialWindow.texture = setTutorialImg;
	}

	tutorialWindow.transform.localScale_ = ADXUtility::Lerp({ 0,0.3f,0 }, { 0.3f / ADXWindow::GetAspect(),0.3f,0 }, ADXUtility::EaseOut(tutorialWindowExAmount, 4));
	tutorialWindow.transform.localPosition_ = { 0.65f,-0.65f + sin(clock() * 0.002f) * 0.01f,0 };

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

#ifdef _DEBUG
	float pos[3] = { transform.localPosition_.x,transform.localPosition_.y,transform.localPosition_.z };

	bool tool_active = true;
	ImGui::Begin("My First Tool", &tool_active, ImGuiWindowFlags_MenuBar);
	ImGui::InputFloat3("Position", pos);

	ImGui::End();

	transform.localPosition_ = { pos[0],pos[1],pos[2] };
#endif
}

void Player::DeadUpdate()
{
	gameOverFilter.renderLayer = 3;
	gameOverFilter.material.ambient = { 0,0,0 };
	gameOverFilter.material.alpha = 0.8f;
	gameOverFilter.Update();

	renderLayer = 4;
	nose.renderLayer = 4;
	dead.renderLayer = 4;

	transform.localRotation_ = camera->transform.localRotation_;
	transform.UpdateMatrix();
	transform.SetWorldRotation(ADXQuaternion::MakeAxisAngle(transform.TransformPointOnlyRotation({ 0,1,0 }), 3.1415f) * transform.GetWorldRotation());

	nose.transform.localPosition_ = { 0,sin(min(max(0,
		ADXUtility::ValueMapping(deadAnimationProgress,0.3f,0.8f,0.0f,1.0f)
		), 1) * 3.1415f) * 4,1.01f };
	nose.transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,3.14159f,min(max(0,
		ADXUtility::ValueMapping(deadAnimationProgress,0.3f,0.8f,0.0f,1.0f)
		), 1) * 50 });
	nose.Update();

	dead.Update();

	isVisible = deadAnimationProgress < 0.3f;
	nose.isVisible = deadAnimationProgress < 0.8f;
	dead.isVisible = !nose.isVisible;

	keyUI.transform.localPosition_ = { 0,-0.5f + sin(clock() * 0.001f - 1) * 0.02f,0 };
	keyUI.transform.localScale_ = { keyUI.transform.localScale_.x,0.45f,1 };
	if (deadAnimationProgress >= 1)
	{
		keyUI.transform.localScale_.x += (0.45f / ADXWindow::GetAspect() - keyUI.transform.localScale_.x) * 0.3f;
		if (ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_SPACE))
		{
			SceneTransition::ChangeScene(2);
		}
	}
	else
	{
		keyUI.transform.localScale_.x = 0;
		if (deadAnimationProgress > 0.3f
			&& ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_SPACE))
		{
			deadAnimationProgress = 1;
		}
	}

	keyUI.Update();

	deadAnimationProgress = min(max(0, deadAnimationProgress + 0.01f), 1);

}