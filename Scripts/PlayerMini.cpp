#include "PlayerMini.h"
#include "Player.h"

std::vector<PlayerMini*> PlayerMini::S_minis{};
std::vector<PlayerMini*> PlayerMini::S_allMiniPtr{};

void PlayerMini::Initialize(Player* setParent, ADXObject setNose)
{
	colliders.back().pushBackPriority = -1;
	colliders.push_back(ADXCollider(this));
	colliders.back().isTrigger = true;
	colliders.back().radius_ = 1.1f;

	parent = setParent;

	nose = Duplicate(setNose);
	nose.transform.parent_ = &transform;

	rigidbody.Initialize(this);

	for (int i = 0; i < colliders.size(); i++)
	{
		colliders[i].Initialize(this);
	}
}

void PlayerMini::Move(float walkSpeed, float jumpPower)
{
	ADXVector3 cameraRight = parent->GetCamera()->transform.TransformPointOnlyRotation({1,0,0});
	cameraRight = cameraRight.Normalize();
	ADXVector3 cameraForward = parent->GetCamera()->transform.TransformPointOnlyRotation({ 0,0,1 });
	cameraForward.y = 0;
	cameraForward = cameraForward.Normalize();

	if (parent->GetInputStatus(0) || parent->GetInputStatus(1) || parent->GetInputStatus(2) || parent->GetInputStatus(3))
	{
		if (parent->GetInputStatus(0))
		{
			rigidbody.velocity += cameraForward * walkSpeed;
		}
		if (parent->GetInputStatus(1))
		{
			rigidbody.velocity -= cameraForward * walkSpeed;
		}
		if (parent->GetInputStatus(2))
		{
			rigidbody.velocity += cameraRight * walkSpeed;
		}
		if (parent->GetInputStatus(3))
		{
			rigidbody.velocity -= cameraRight * walkSpeed;
		}
		transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,atan2(rigidbody.velocity.x, rigidbody.velocity.z),0 });
	}

	if (parent->GetInputStatusTrigger(4))
	{
		rigidbody.velocity.y = jumpPower;
	}
	if (parent->GetInputStatusRelease(4) && rigidbody.velocity.y > 0)
	{
		rigidbody.velocity.y *= 0.2f;
	}
}

void PlayerMini::UniqueUpdate()
{
	rigidbody.gravityScale = 0.01f;
	rigidbody.drag = 0.8f;

	rigidbody.VelocityMove();

	rigidbody.velocity.y /= 0.8f;
	rigidbody.velocity.y -= 0.03f;

	Move(0.1f, 0.8f);

	rigidbody.Update(this);

	nose.transform.parent_ = &transform;
	nose.Update();

	S_allMiniPtr.push_back(this);
}

void PlayerMini::StaticUpdate()
{
	S_minis = S_allMiniPtr;
	S_allMiniPtr.clear();
}