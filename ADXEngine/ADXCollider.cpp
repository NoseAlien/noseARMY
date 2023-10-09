﻿#include "ADXCollider.h"
#include "ADXObject.h"

std::vector<collidePattern> ADXCollider::S_ignoreCollidePatterns = {};
std::vector<collidePattern> ADXCollider::S_ignorePushBackPatterns = {};

/*
【接触検知から押し戻しベクトル算出までの流れ】

・空間上の一つの点が二つの図形の中にあるかを調べる（二つの立体図形が同じ点を共有しているかを調べる）
これにより二つの図形が接触しているかを判定する

・当たっていると判定されたらそれぞれのオブジェクトで『衝突点』を求める

・互いのオブジェクトの衝突点の座標の差が押し戻しの方向と強さが入ったベクトルになる
*/

std::list<ADXCollider*> ADXCollider::S_cols = {};

void ADXCollider::UniqueInitialize()
{
	preTranslation = GetGameObject()->transform.localPosition_;
	preMatrix = GetGameObject()->transform.GetMatWorld();
	preMatrixInverse = GetGameObject()->transform.GetMatWorldInverse();
}

void ADXCollider::UniqueUpdate()
{
	S_cols.push_back(this);
}

//空間上の点をコライダーの中に収めた時の座標
ADXVector3 ADXCollider::ClosestPoint(const ADXVector3& pos) const
{
	ADXVector3 ret = ADXMatrix4::Transform(pos, GetGameObject()->transform.GetMatWorldInverse());
	ADXVector3 closPos = ret;

	switch (colType_)
	{
	case box:
		if (closPos.x > pos_.x + scale_.x)
		{
			closPos.x = pos_.x + scale_.x;
		}
		else if (closPos.x < pos_.x - scale_.x)
		{
			closPos.x = pos_.x - scale_.x;
		}

		if (closPos.y > pos_.y + scale_.y)
		{
			closPos.y = pos_.y + scale_.y;
		}
		else if (closPos.y < pos_.y - scale_.y)
		{
			closPos.y = pos_.y - scale_.y;
		}

		if (closPos.z > pos_.z + scale_.z)
		{
			closPos.z = pos_.z + scale_.z;
		}
		else if (closPos.z < pos_.z - scale_.z)
		{
			closPos.z = pos_.z - scale_.z;
		}
		break;
	case sphere:
		if ((closPos - pos_).Length() > radius_)
		{
			closPos = pos_ + (closPos - pos_).Normalize() * radius_;
		}
		break;
	case plain:
		closPos.y = 0;
		break;
	case quad:
		if (closPos.x > pos_.x + scale_.x)
		{
			closPos.x = pos_.x + scale_.x;
		}
		else if (closPos.x < pos_.x - scale_.x)
		{
			closPos.x = pos_.x - scale_.x;
		}

		if (closPos.z > pos_.z + scale_.z)
		{
			closPos.z = pos_.z + scale_.z;
		}
		else if (closPos.z < pos_.z - scale_.z)
		{
			closPos.z = pos_.z - scale_.z;
		}

		closPos.y = 0;
		break;
	default:
		closPos = pos_;
		break;
	}

	if ((closPos - ret).Length() > 0)
	{
		ret = closPos;
		ret = ADXMatrix4::Transform(ret, GetGameObject()->transform.GetMatWorld());
	}
	else
	{
		ret = pos;
	}

	return ret;
}

//空間上の点をコライダーのフチに寄せた時の相対座標
ADXVector3 ADXCollider::EdgeLocalPoint(const ADXVector3& pos) const
{
	return EdgeLocalPoint(pos, pos);
}

//空間上の点をコライダーのフチに寄せた時の相対座標
ADXVector3 ADXCollider::EdgeLocalPoint(const ADXVector3& pos, const ADXVector3& prePos) const
{
	ADXVector3 ret = ADXMatrix4::Transform(pos, GetGameObject()->transform.GetMatWorldInverse());
	ret -= pos_;

	ADXVector3 prevPos = ADXMatrix4::Transform(prePos, preMatrixInverse);
	prevPos -= pos_;

	ADXVector3 absLocalPos{};

	switch (colType_)
	{
	case box:
		ret.x /= scale_.x;
		ret.y /= scale_.y;
		ret.z /= scale_.z;

		prevPos.x /= scale_.x;
		prevPos.y /= scale_.y;
		prevPos.z /= scale_.z;

		absLocalPos = prevPos;
		if (absLocalPos.x < 0)
		{
			absLocalPos.x = -absLocalPos.x;
		}
		if (absLocalPos.y < 0)
		{
			absLocalPos.y = -absLocalPos.y;
		}
		if (absLocalPos.z < 0)
		{
			absLocalPos.z = -absLocalPos.z;
		}

		if (absLocalPos.z > absLocalPos.x && absLocalPos.z > absLocalPos.y)
		{
			if (ret.z > 0)
			{
				ret.z = 1;
			}
			else
			{
				ret.z = -1;
			}
		}
		else
		{
			if (absLocalPos.x > absLocalPos.y)
			{
				if (ret.x > 0)
				{
					ret.x = 1;
				}
				else
				{
					ret.x = -1;
				}
			}
			else
			{
				if (ret.y > 0)
				{
					ret.y = 1;
				}
				else
				{
					ret.y = -1;
				}
			}
		}

		ret.x *= scale_.x;
		ret.y *= scale_.y;
		ret.z *= scale_.z;
		break;
	case sphere:
		ret = ret.Normalize() * radius_;
		break;
	case plain:
		ret.y = 0;
		break;
	case quad:
		if (ret.x > pos_.x + scale_.x)
		{
			ret.x = pos_.x + scale_.x;
		}
		else if (ret.x < pos_.x - scale_.x)
		{
			ret.x = pos_.x - scale_.x;
		}

		if (ret.z > pos_.z + scale_.z)
		{
			ret.z = pos_.z + scale_.z;
		}
		else if (ret.z < pos_.z - scale_.z)
		{
			ret.z = pos_.z - scale_.z;
		}

		ret.y = 0;
		break;
	default:
		ret = { 0,0,0 };
		break;
	}

	ret += pos_;
	ret = ADXMatrix4::Transform(ClosestPoint(ADXMatrix4::Transform(ret, GetGameObject()->transform.GetMatWorld())), GetGameObject()->transform.GetMatWorldInverse());

	return ret;
}

//空間上の点をコライダーのフチに寄せた時の座標
ADXVector3 ADXCollider::EdgePoint(const ADXVector3& pos)
{
	return EdgePoint(pos, pos);
}

//空間上の点をコライダーのフチに寄せた時の座標
ADXVector3 ADXCollider::EdgePoint(const ADXVector3& pos, const ADXVector3& prePos)
{
	ADXVector3 ret = EdgeLocalPoint(pos, prePos);
	ret = ADXMatrix4::Transform(ret, GetGameObject()->transform.GetMatWorld());
	return ret;
}

//相手のコライダーとの衝突点の座標
ADXVector3 ADXCollider::CollidePoint(const ADXVector3& pos, const ADXVector3& targetColSenter, const ADXVector3& move) const
{
	ADXVector3 ret = EdgeLocalPoint(pos, pos - move);
	ADXVector3 targetLocalSenter = ADXMatrix4::Transform(targetColSenter, GetGameObject()->transform.GetMatWorldInverse()) - pos_;

	if (targetLocalSenter.Dot(ret) < 0)
	{
		ret = -ret;
	}

	ret = ADXMatrix4::Transform(ret, GetGameObject()->transform.GetMatWorld());
	return ret;
}

//押し返す方向と強さのベクトル
ADXVector3 ADXCollider::CollideVector(const ADXCollider& col)
{
	ADXVector3 ret;

	ADXVector3 myTranslation = ADXMatrix4::Transform(pos_, GetGameObject()->transform.GetMatWorld());
	ADXVector3 myMove = myTranslation - ADXMatrix4::Transform(pos_, preMatrix);

	ADXVector3 targetTranslation = ADXMatrix4::Transform(col.pos_, col.GetGameObject()->transform.GetMatWorld());
	ADXVector3 targetMove = targetTranslation - ADXMatrix4::Transform(col.pos_, col.preMatrix);

	ADXVector3 myPushBack1 = col.CollidePoint(myTranslation, myTranslation, myMove) - CollidePoint(col.CollidePoint(myTranslation, myTranslation, myMove), targetTranslation, targetMove);
	ADXVector3 myPushBack2 = col.CollidePoint(CollidePoint(targetTranslation, targetTranslation, targetMove), myTranslation, myMove) - CollidePoint(targetTranslation, targetTranslation, targetMove);

	ADXVector3 pushBackDiff = myPushBack1 - myPushBack2;

	if ((targetTranslation - myTranslation).Dot(pushBackDiff) > 0)
	{
		ret = myPushBack2;
	}
	else
	{
		ret = myPushBack1;
	}

	if (ret.Dot(targetTranslation - myTranslation) > 0)
	{
		ret = -ret;
	}

	return ret;
}

//相手のコライダーと重なっているか
bool ADXCollider::IsHit(const ADXCollider& col)
{
	ADXVector3 closestVec1 = col.ClosestPoint(ClosestPoint(ADXMatrix4::Transform(col.pos_, col.GetGameObject()->transform.GetMatWorld())));
	ADXVector3 closestVec2 = ClosestPoint(col.ClosestPoint(ClosestPoint(ADXMatrix4::Transform(col.pos_, col.GetGameObject()->transform.GetMatWorld()))));
	float colPointDiff = (closestVec1 - closestVec2).Length();
	if ((closestVec1 - closestVec2).Length() <= 0)
	{
		return true;
	}

	closestVec1 = ClosestPoint(col.ClosestPoint(ADXMatrix4::Transform(pos_, GetGameObject()->transform.GetMatWorld())));
	closestVec2 = col.ClosestPoint(ClosestPoint(col.ClosestPoint(ADXMatrix4::Transform(pos_, GetGameObject()->transform.GetMatWorld()))));
	colPointDiff = (closestVec1 - closestVec2).Length();
	if ((closestVec1 - closestVec2).Length() <= 0)
	{
		return true;
	}

	return false;
}

//コライダー同士で押し合う（動かないコライダーにぶつかったら一方的に押される）
void ADXCollider::Collide(ADXCollider* col)
{
	if (GetGameObject() == col->GetGameObject())
	{
		return;
	}

	for (auto& itr : S_ignoreCollidePatterns)
	{
		if ((itr.layer1 == collideLayer && itr.layer2 == col->collideLayer) ||
			(itr.layer2 == collideLayer && itr.layer1 == col->collideLayer))
		{
			return;
		}
	}

	bool executePushBack = true;
	for (auto& itr : S_ignorePushBackPatterns)
	{
		if ((itr.layer1 == collideLayer && itr.layer2 == col->collideLayer) ||
			(itr.layer2 == collideLayer && itr.layer1 == col->collideLayer))
		{
			executePushBack = false;
		}
	}

	if (enabled && col->enabled && col->GetGameObject() != GetGameObject() && IsHit(*col))
	{
		if (executePushBack && !isTrigger && !col->isTrigger)
		{
			ADXVector3 myPushBack = CollideVector(*col);
			ADXVector3 targetPushBack = col->CollideVector(*this);

			int32_t conditionState[4][3] = { {0,0,0},{1,1,1},{2,2,2},{3,1,2} };

			int32_t pushableCondition = 0;
			int32_t priorityCondition = 0;

			if (pushable_ && col->pushable_)
			{
				pushableCondition = 3;
			}
			else if (pushable_)
			{
				pushableCondition = 1;
			}
			else if (col->pushable_)
			{
				pushableCondition = 2;
			}

			if (pushBackPriority == col->pushBackPriority)
			{
				priorityCondition = 0;
			}
			else if (pushBackPriority < col->pushBackPriority)
			{
				priorityCondition = 1;
			}
			else if (pushBackPriority > col->pushBackPriority)
			{
				priorityCondition = 2;
			}

			int32_t ConditionStateResult = conditionState[pushableCondition][priorityCondition];

			switch (ConditionStateResult)
			{
			case 1:
				pushBackVector += myPushBack;
				break;
			case 2:
				col->pushBackVector += targetPushBack;
				break;
			case 3:
				pushBackVector += myPushBack * 0.5f;
				col->pushBackVector += targetPushBack * 0.5f;
				break;
			default:
				break;
			}
		}

		if (collideList.empty() || !collideList.empty() && collideList.back() != col)
		{
			collideList.push_back(col);
			GetGameObject()->OnCollisionHit(col, this);
		}
		if (collideList.empty() || !col->collideList.empty() && col->collideList.back() != this)
		{
			col->collideList.push_back(this);
			GetGameObject()->OnCollisionHit(this, col);
		}
	}
}

//先のCollidersUpdateで別のコライダーにぶつかっていたらオブジェクトを押し戻す
void ADXCollider::SendPushBack()
{
	if (pushable_)
	{
		GetGameObject()->transform.localPosition_ += pushBackVector;
		GetGameObject()->transform.UpdateMatrix();
	}
	preTranslation = GetGameObject()->transform.localPosition_;
	preMatrix = GetGameObject()->transform.GetMatWorld();
	preMatrixInverse = GetGameObject()->transform.GetMatWorldInverse();
	pushBackVector = { 0,0,0 };
}

//全てのコライダーで接触判定と押し戻しベクトルの算出を行う
//ゲーム内の全てのコライダーが入った配列を入れて使う
void ADXCollider::StaticUpdate()
{
	//現在の座標を保存しておく
	std::vector<ADXVector3> objsTranslation = {};
	for (auto& itr : ADXObject::GetObjs())
	{
		objsTranslation.push_back(itr->transform.localPosition_);
	}

	//すべてのコライダーで移動距離÷(最小絶対半径×0.95)を求め、最も大きい値をtranslateDivNumFに入れる
	float translateDivNumF = 1;
	for (auto& colItr : S_cols)
	{
		//ついでにcollideListもこのタイミングでリセット
		colItr->collideList.clear();


		ADXVector3 move = colItr->GetGameObject()->transform.localPosition_ - colItr->preTranslation;

		ADXVector3 scaleX1 = { colItr->scale_.x,0,0 };
		ADXVector3 scaleY1 = { 0,colItr->scale_.y,0 };
		ADXVector3 scaleZ1 = { 0,0,colItr->scale_.z };

		ADXMatrix4 WorldScalingMat = colItr->GetGameObject()->transform.GetMatScale();
		WorldScalingMat *= colItr->GetGameObject()->transform.GetMatRot();

		float worldScaleX1 = ADXMatrix4::Transform(scaleX1, WorldScalingMat).Length();
		float worldScaleY1 = ADXMatrix4::Transform(scaleY1, WorldScalingMat).Length();
		float worldScaleZ1 = ADXMatrix4::Transform(scaleZ1, WorldScalingMat).Length();

		float minimumWorldRadius1 = 1;

		if (worldScaleX1 < worldScaleY1 && worldScaleX1 < worldScaleZ1 && worldScaleX1 > 0)
		{
			minimumWorldRadius1 = worldScaleX1;
		}
		else if (worldScaleY1 < worldScaleZ1 && worldScaleY1 > 0)
		{
			minimumWorldRadius1 = worldScaleY1;
		}
		else if (worldScaleZ1 > 0)
		{
			minimumWorldRadius1 = worldScaleZ1;
		}

		ADXVector3 limitedMove = move.Normalize() * minimumWorldRadius1 * 0.95f * colItr->maxMoveDistanceRate;

		if (colItr->maxMoveDistanceRate >= 0 && move.Length() > limitedMove.Length())
		{
			move = limitedMove;
		}

		float moveDivnum1 = move.Length() / (minimumWorldRadius1 * 0.95f);
		if (moveDivnum1 >= translateDivNumF)
		{
			translateDivNumF = moveDivnum1;
		}
	}
	translateDivNumF = ceilf(translateDivNumF);

	//全てのオブジェクトを移動する前の座標へ移動させる
	for (auto& colItr : S_cols)
	{
		colItr->GetGameObject()->transform.localPosition_ = colItr->preTranslation;
	}

	//行列更新のついでに移動する前の座標を保存
	std::vector<ADXVector3> objsPreTranslation = {};
	for (auto& objItr : ADXObject::GetObjs())
	{
		objsPreTranslation.push_back(objItr->transform.localPosition_);
		objItr->transform.UpdateMatrix();
	}

	//少しづつ移動させながら当たり判定と押し戻し処理を行う
	for (int32_t i = 0; i < translateDivNumF; i++)
	{
		uint32_t index = 0;
		//移動
		for (auto& itr : ADXObject::GetObjs())
		{
			ADXVector3 move = objsTranslation[index] - objsPreTranslation[index];

			itr->transform.localPosition_ += move / translateDivNumF;
			itr->transform.UpdateMatrix();

			index++;
		}

		//当たり判定と押し戻しベクトルの算出
		for (auto& colItr1 : S_cols)
		{
			for (auto& colItr2 : S_cols)
			{
				if (&colItr1 != &colItr2)
				{
					colItr1->Collide(colItr2);
					colItr1->SendPushBack();
					colItr2->SendPushBack();
				}
			}
		}
	}

	S_cols.clear();
}