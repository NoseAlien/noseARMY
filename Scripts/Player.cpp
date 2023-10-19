#include "Player.h"
#include "SceneTransition.h"
#include "ADXUtility.h"
#include "FieldBox.h"
#include <time.h>
#include <imgui.h>

void Player::Initialize(ADXKeyBoardInput* setKeyboard, std::vector<BYTE> setConfig, ADXCamera* setCamera)
{
	keyboard = setKeyboard;
	config = setConfig;
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
	ADXVector3 cameraRight = camera->GetGameObject()->transform.TransformPointOnlyRotation({ 1,0,0 });
	cameraRight = cameraRight.Normalize();
	ADXVector3 cameraForward = camera->GetGameObject()->transform.TransformPointOnlyRotation({ 0,0,1 });
	cameraForward.y = 0;
	cameraForward = cameraForward.Normalize();

	if (keyboard->KeyPress(config[0]) || keyboard->KeyPress(config[1]) || keyboard->KeyPress(config[2]) || keyboard->KeyPress(config[3]))
	{
		if (keyboard->KeyPress(config[0]))
		{
			rigidbody->velocity += cameraForward * walkSpeed;
		}
		if (keyboard->KeyPress(config[1]))
		{
			rigidbody->velocity -= cameraForward * walkSpeed;
		}
		if (keyboard->KeyPress(config[2]))
		{
			rigidbody->velocity += cameraRight * walkSpeed;
		}
		if (keyboard->KeyPress(config[3]))
		{
			rigidbody->velocity -= cameraRight * walkSpeed;
		}
		GetGameObject()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,atan2(rigidbody->velocity.x, rigidbody->velocity.z),0 });
	}

	if (keyboard->KeyTrigger(config[4]))
	{
		rigidbody->velocity.y = jumpPower;
		jumpSE.Play();
	}
	if (keyboard->KeyRelease(config[4]) && rigidbody->velocity.y > 0)
	{
		rigidbody->velocity.y *= 0.2f;
	}
}

void Player::LiveEntitiesInitialize()
{
	jumpSE = ADXAudio::LoadADXAudio("sound/jump.wav");
	//damageSE = ADXAudio::LoadADXAudio("sound/damage.wav");
	//defeatSE = ADXAudio::LoadADXAudio("sound/despawn.wav");
	windowOpenSE = ADXAudio::LoadADXAudio("sound/windowOpen.wav");

	rect = ADXModel::CreateRect();
	playerModel = ADXModel::LoadADXModel("model/sphere.obj");

	GetGameObject()->model = &playerModel;
	GetGameObject()->texture = ADXImage::LoadADXImage("apEG_fur.png");

	rigidbody = GetGameObject()->AddComponent<ADXRigidbody>();

	nose = ADXObject::Create({ 0,0,1.01f }, ADXQuaternion::EulerToQuaternion({ 0,3.1415f,0 }), { 0.42f,0.35f,0.35f });
	nose->transform.parent_ = &GetGameObject()->transform;
	nose->transform.UpdateMatrix();
	nose->model = &rect;
	nose->texture = ADXImage::LoadADXImage("apEGnoSE.png");
	nose->material = GetGameObject()->material;

	tutorialWindow = ADXObject::Create();
	tutorialWindow->transform.rectTransform = true;
	tutorialWindow->transform.UpdateMatrix();
	tutorialWindow->model = &rect;
	tutorialWindow->texture = ADXImage::LoadADXImage("WhiteDot.png");
	tutorialWindow->material = GetGameObject()->material;
	tutorialWindow->renderLayer = 1;

	outOfField = ADXObject::Create();
	outOfField->transform.rectTransform = true;
	outOfField->transform.UpdateMatrix();
	outOfField->model = &rect;
	outOfField->texture = ADXImage::LoadADXImage("outOfField.png");
	outOfField->material = GetGameObject()->material;
	outOfField->renderLayer = 1;

	gameOverFilter = ADXObject::Create();
	gameOverFilter->transform.rectTransform = true;
	gameOverFilter->transform.UpdateMatrix();
	gameOverFilter->model = &rect;
	gameOverFilter->texture = ADXImage::LoadADXImage("WhiteDot.png");
	gameOverFilter->material = GetGameObject()->material;
	gameOverFilter->renderLayer = 1;

	dead = ADXObject::Create();
	dead->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,3.1415f,0 });
	dead->transform.parent_ = &GetGameObject()->transform;
	dead->transform.UpdateMatrix();
	dead->model = &rect;
	dead->texture = ADXImage::LoadADXImage("apEGnoSE_dead.png");
	dead->material = GetGameObject()->material;
	dead->renderLayer = 4;

	keyUI = ADXObject::Create();
	keyUI->transform.rectTransform = true;
	keyUI->model = &rect;
	keyUI->renderLayer = 5;
	keyUI->texture = ADXImage::LoadADXImage("PRESS_SPACE.png");
}

void Player::LiveEntitiesUpdate()
{
	GetGameObject()->renderLayer = 0;
	nose->renderLayer = 0;

	gameOverFilter->isVisible =
	dead->isVisible =
	keyUI->isVisible = false;

	deadAnimationProgress = 0;

	float scale = ADXUtility::ValueMapping((float)minis.size(), 0, (float)maxMinisNum, 1, 0.25f);
	GetGameObject()->transform.localScale_ = { scale,scale,scale };

	GetGameObject()->transform.modelScale_ = { 1 + sinf((float)clock() * 0.001f) * 0.03f,1 + cosf((float)clock() * 0.001f) * 0.03f,1 + sinf((float)clock() * 0.001f) * 0.03f };

	ADXVector3 cameraVec = camera->GetGameObject()->transform.GetWorldPosition() - GetGameObject()->transform.GetWorldPosition();
	cameraVec.y = 0;
	cameraVec = cameraVec.Normalize();
	camera->GetGameObject()->transform.localPosition_ = GetGameObject()->transform.localPosition_ + cameraVec * 20;
	camera->GetGameObject()->transform.localPosition_.y += 5;
	camera->GetGameObject()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,atan2(-cameraVec.x, -cameraVec.z),0 });
	camera->GetGameObject()->transform.UpdateMatrix();
	camera->GetGameObject()->transform.localRotation_ = camera->GetGameObject()->transform.localRotation_ * ADXQuaternion::MakeAxisAngle({ 1,0,0 }, 0.3f);

	rigidbody->drag = 0.8f;

	rigidbody->VelocityMove();

	if (keyboard->KeyPress(config[5]))
	{
		rigidbody->velocity *= 0.8f;
		rigidbody->gravityScale = 0;
	}
	else
	{
		rigidbody->gravityScale = 0.01f;
		rigidbody->dragAxis.y = false;
		Move(0.05f, 0.4f);
	}

	for (auto& itr : minis)
	{
		if (itr->GetGameObject() != nullptr
			&& ADXMatrix4::Transform(itr->GetGameObject()->transform.localPosition_, GetGameObject()->transform.GetMatWorldInverse()).Length() > 30 / scale)
		{
			itr->GetGameObject()->Destroy();
		}
	}

	nose->transform.localScale_ = ADXVector3{ 0.42f,0.35f,0.35f } *(float)fmax(1, 1 + pow(fmax(0, splitInterval), 2) * 0.02f);
	nose->transform.localPosition_ = { 0,sinf((float)clock() * 0.001f) * 0.03f,1.01f + sinf((float)clock() * 0.001f) * 0.03f };

	splitInterval--;
	splitInterval = max(-20, splitInterval);

	if (splitable && keyboard->KeyRelease(config[5]) && splitInterval <= 0)
	{
		nose->transform.localScale_ = { 0.42f,0.35f,0.35f };
		nose->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,3.1415f,0 });
		if (minis.size() < maxMinisNum)
		{
			
			ADXObject* miniObj = nullptr;
			miniObj = ADXObject::Duplicate(*GetGameObject());
			PlayerMini* mini = miniObj->AddComponent<PlayerMini>();
			
			mini->GetGameObject()->transform.localScale_ = { 0.5f,0.5f,0.5f };
			mini->GetGameObject()->transform.localPosition_ = ADXMatrix4::Transform({ 0,0,1 }, GetGameObject()->transform.GetMatWorld());
			
			mini->Initialize(this, *nose);
			minis.push_back(mini);
		}
		splitInterval = 7;
	}

	if (!keyboard->KeyPress(config[5]))
	{
		splitable = true;
	}

	for (auto& itr : minis)
	{
		for (auto& colItr : itr->GetGameObject()->GetComponents<ADXCollider>())
		{
			if (colItr->isTrigger)
			{
				SetAttackObj({ colItr, this, (float)minis.size() });
			}
		}
	}

	uint32_t prevTutorialImg = tutorialWindow->texture;

	if (prevTutorialImg != setTutorialImg)
	{
		windowExtend = false;
	}
	if (windowOpening)
	{
		windowExtend = true;
	}


	bool prevwindowOpening = windowOpening;
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
		tutorialWindow->texture = setTutorialImg;
	}

	tutorialWindow->transform.localScale_ = ADXUtility::Lerp({ 0,0.3f,0 }, { 0.3f / ADXWindow::GetAspect(),0.3f,0 }, ADXUtility::EaseOut(tutorialWindowExAmount, 4));
	tutorialWindow->transform.localPosition_ = { 0.65f,-0.65f + sin(clock() * 0.002f) * 0.01f,0 };


	if (isOutOfField)
	{
		outOfField->transform.localScale_ = { 0.4f,0.7f,0 };
	}
	else
	{
		outOfField->transform.localScale_ = { 0,0,0 };
	}

	outOfField->transform.localPosition_ = { -0.6f,0.65f + sin(clock() * 0.003f) * 0.01f,0 };

	isOutOfField = true;
	windowExtend = false;


#ifdef _DEBUG
	float pos[3] = { GetGameObject()->transform.localPosition_.x,GetGameObject()->transform.localPosition_.y,GetGameObject()->transform.localPosition_.z };

	bool tool_active = true;
	ImGui::Begin("My First Tool", &tool_active, ImGuiWindowFlags_MenuBar);
	ImGui::InputFloat3("Position", pos);

	ImGui::End();

	GetGameObject()->transform.localPosition_ = { pos[0],pos[1],pos[2] };
#endif
}

void Player::LiveEntitiesOnCollisionHit(ADXCollider* col, [[maybe_unused]]ADXCollider* myCol)
{
	if (col->GetGameObject()->GetComponent<FieldBox>() != nullptr
		&& (GetGameObject()->transform.GetWorldPosition() - col->ClosestPoint(GetGameObject()->transform.GetWorldPosition())).Length() < 0.1)
	{
		isOutOfField = false;
	}

	if (col->GetGameObject()->GetComponent<TutorialArea>() != nullptr)
	{
		if (!windowClosing)
		{
			windowExtend = true;
		}
		setTutorialImg = col->GetGameObject()->GetComponent<TutorialArea>()->GetTutorialImg();
	}

	if (splitInterval <= -20)
	{
		for (auto& itr : minis)
		{
			if (col->GetGameObject() == itr->GetGameObject())
			{
				splitable = false;
				itr->GetGameObject()->Destroy();
			}
		}
	}
}

void Player::DeadUpdate()
{
	gameOverFilter->isVisible =
	dead->isVisible =
	keyUI->isVisible = true;

	gameOverFilter->renderLayer = 3;
	gameOverFilter->material.ambient = { 0,0,0 };
	gameOverFilter->material.alpha = 0.8f;

	GetGameObject()->renderLayer = 4;
	nose->renderLayer = 4;

	GetGameObject()->transform.modelPosition_ = { 0,0,0 };
	GetGameObject()->transform.localRotation_ = camera->GetGameObject()->transform.localRotation_;
	GetGameObject()->transform.UpdateMatrix();
	GetGameObject()->transform.SetWorldRotation(
		ADXQuaternion::MakeAxisAngle(GetGameObject()->transform.TransformPointOnlyRotation({ 0,1,0 }), 3.1415f)
		* GetGameObject()->transform.GetWorldRotation());

	nose->transform.localPosition_ = { 0,sin(min(max(0,
		ADXUtility::ValueMapping(deadAnimationProgress,0.3f,0.8f,0.0f,1.0f)
		), 1) * 3.1415f) * 4,1.01f };
	nose->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,3.14159f,min(max(0,
		ADXUtility::ValueMapping(deadAnimationProgress,0.3f,0.8f,0.0f,1.0f)
		), 1) * 50 });


	GetGameObject()->isVisible = deadAnimationProgress < 0.3f;
	nose->isVisible = deadAnimationProgress < 0.8f;
	dead->isVisible = !nose->isVisible;

	keyUI->transform.localPosition_ = { 0,-0.5f + sin(clock() * 0.001f - 1) * 0.02f,0 };
	keyUI->transform.localScale_ = { keyUI->transform.localScale_.x,0.45f,1 };
	if (deadAnimationProgress >= 1)
	{
		keyUI->transform.localScale_.x += (0.45f / ADXWindow::GetAspect() - keyUI->transform.localScale_.x) * 0.3f;
		if (ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_SPACE))
		{
			SceneTransition::ChangeScene(2);
		}
	}
	else
	{
		keyUI->transform.localScale_.x = 0;
		if (deadAnimationProgress > 0.3f
			&& ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_SPACE))
		{
			deadAnimationProgress = 1;
		}
	}

	deadAnimationProgress = min(max(0, deadAnimationProgress + 0.01f), 1);

	rigidbody->velocity = { 0,0,0 };
}

void Player::SafetyPhase()
{
	minis.remove_if([=](auto& itr)
		{ return itr->GetGameObject()->GetDeleteFlag(); });
}
