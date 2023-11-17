#include "ADXCollider.h"
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
	preTranslation_ = GetGameObject()->transform_.localPosition_;
	preMatrix_ = GetGameObject()->transform_.GetMatWorld();
	preMatrixInverse_ = GetGameObject()->transform_.GetMatWorldInverse();
}

void ADXCollider::UniqueUpdate()
{
	S_cols.push_back(this);
}

float ADXCollider::GenerateBoundingSphereRadius() const
{
	ADXVector3 scaledEdgePos{};
	switch (colType_)
	{
	case box:
		scaledEdgePos = GetGameObject()->transform_.TransformPointWithoutTranslation(scale_);
		return scaledEdgePos.Length();
		break;
	case sphere:
		scaledEdgePos = GetGameObject()->transform_.TransformPointWithoutTranslation({radius_,0,0});
		return scaledEdgePos.Length();
		break;
	case quad:
		scaledEdgePos = GetGameObject()->transform_.TransformPointWithoutTranslation({ scale_.x_,0,scale_.z_ });
		return scaledEdgePos.Length();
		break;
	}

	return 0.0f;
}

//空間上の点をコライダーの中に収めた時の座標
ADXVector3 ADXCollider::ClosestPoint(const ADXVector3& pos) const
{
	ADXVector3 ret = ADXMatrix4::Transform(pos, GetGameObject()->transform_.GetMatWorldInverse());
	ADXVector3 closPos = ret;

	switch (colType_)
	{
	case box:
		if (closPos.x_ > pos_.x_ + scale_.x_)
		{
			closPos.x_ = pos_.x_ + scale_.x_;
		}
		else if (closPos.x_ < pos_.x_ - scale_.x_)
		{
			closPos.x_ = pos_.x_ - scale_.x_;
		}

		if (closPos.y_ > pos_.y_ + scale_.y_)
		{
			closPos.y_ = pos_.y_ + scale_.y_;
		}
		else if (closPos.y_ < pos_.y_ - scale_.y_)
		{
			closPos.y_ = pos_.y_ - scale_.y_;
		}

		if (closPos.z_ > pos_.z_ + scale_.z_)
		{
			closPos.z_ = pos_.z_ + scale_.z_;
		}
		else if (closPos.z_ < pos_.z_ - scale_.z_)
		{
			closPos.z_ = pos_.z_ - scale_.z_;
		}
		break;
	case sphere:
		if ((closPos - pos_).Length() > radius_)
		{
			closPos = pos_ + (closPos - pos_).Normalize() * radius_;
		}
		break;
	case quad:
		if (closPos.x_ > pos_.x_ + scale_.x_)
		{
			closPos.x_ = pos_.x_ + scale_.x_;
		}
		else if (closPos.x_ < pos_.x_ - scale_.x_)
		{
			closPos.x_ = pos_.x_ - scale_.x_;
		}

		if (closPos.z_ > pos_.z_ + scale_.z_)
		{
			closPos.z_ = pos_.z_ + scale_.z_;
		}
		else if (closPos.z_ < pos_.z_ - scale_.z_)
		{
			closPos.z_ = pos_.z_ - scale_.z_;
		}

		closPos.y_ = 0;
		break;
	default:
		closPos = pos_;
		break;
	}

	if ((closPos - ret).Length() > 0)
	{
		ret = closPos;
		ret = ADXMatrix4::Transform(ret, GetGameObject()->transform_.GetMatWorld());
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
	ADXVector3 ret = ADXMatrix4::Transform(pos, GetGameObject()->transform_.GetMatWorldInverse());
	ret -= pos_;

	ADXVector3 prevPos = ADXMatrix4::Transform(prePos, preMatrixInverse_);
	prevPos -= pos_;

	ADXVector3 absLocalPos{};

	switch (colType_)
	{
	case box:
		ret.x_ /= scale_.x_;
		ret.y_ /= scale_.y_;
		ret.z_ /= scale_.z_;

		prevPos.x_ /= scale_.x_;
		prevPos.y_ /= scale_.y_;
		prevPos.z_ /= scale_.z_;

		absLocalPos = prevPos;
		if (absLocalPos.x_ < 0)
		{
			absLocalPos.x_ = -absLocalPos.x_;
		}
		if (absLocalPos.y_ < 0)
		{
			absLocalPos.y_ = -absLocalPos.y_;
		}
		if (absLocalPos.z_ < 0)
		{
			absLocalPos.z_ = -absLocalPos.z_;
		}

		if (absLocalPos.z_ > absLocalPos.x_ && absLocalPos.z_ > absLocalPos.y_)
		{
			if (ret.z_ > 0)
			{
				ret.z_ = 1;
			}
			else
			{
				ret.z_ = -1;
			}
		}
		else
		{
			if (absLocalPos.x_ > absLocalPos.y_)
			{
				if (ret.x_ > 0)
				{
					ret.x_ = 1;
				}
				else
				{
					ret.x_ = -1;
				}
			}
			else
			{
				if (ret.y_ > 0)
				{
					ret.y_ = 1;
				}
				else
				{
					ret.y_ = -1;
				}
			}
		}

		ret.x_ *= scale_.x_;
		ret.y_ *= scale_.y_;
		ret.z_ *= scale_.z_;
		break;
	case sphere:
		ret = ret.Normalize() * radius_;
		break;
	case quad:
		if (ret.x_ > pos_.x_ + scale_.x_)
		{
			ret.x_ = pos_.x_ + scale_.x_;
		}
		else if (ret.x_ < pos_.x_ - scale_.x_)
		{
			ret.x_ = pos_.x_ - scale_.x_;
		}

		if (ret.z_ > pos_.z_ + scale_.z_)
		{
			ret.z_ = pos_.z_ + scale_.z_;
		}
		else if (ret.z_ < pos_.z_ - scale_.z_)
		{
			ret.z_ = pos_.z_ - scale_.z_;
		}

		ret.y_ = 0;
		break;
	default:
		ret = { 0,0,0 };
		break;
	}

	ret += pos_;
	ret = ADXMatrix4::Transform(ClosestPoint(ADXMatrix4::Transform(ret, GetGameObject()->transform_.GetMatWorld())), GetGameObject()->transform_.GetMatWorldInverse());

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
	ret = ADXMatrix4::Transform(ret, GetGameObject()->transform_.GetMatWorld());
	return ret;
}

//相手のコライダーとの衝突点の座標
ADXVector3 ADXCollider::CollidePoint(const ADXVector3& pos, const ADXVector3& targetColSenter, const ADXVector3& move) const
{
	ADXVector3 ret = EdgeLocalPoint(pos, pos - move);
	ADXVector3 targetLocalSenter = ADXMatrix4::Transform(targetColSenter, GetGameObject()->transform_.GetMatWorldInverse()) - pos_;

	if (targetLocalSenter.Dot(ret) < 0)
	{
		ret = -ret;
	}

	ret = ADXMatrix4::Transform(ret, GetGameObject()->transform_.GetMatWorld());
	return ret;
}

//押し返す方向と強さのベクトル
ADXVector3 ADXCollider::CollideVector(const ADXCollider& col)
{
	ADXVector3 ret;

	ADXVector3 myTranslation = ADXMatrix4::Transform(pos_, GetGameObject()->transform_.GetMatWorld());
	ADXVector3 myMove = myTranslation - ADXMatrix4::Transform(pos_, preMatrix_);

	ADXVector3 targetTranslation = ADXMatrix4::Transform(col.pos_, col.GetGameObject()->transform_.GetMatWorld());
	ADXVector3 targetMove = targetTranslation - ADXMatrix4::Transform(col.pos_, col.preMatrix_);

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
	if (!IsSphereHit(ADXMatrix4::Transform(pos_, GetGameObject()->transform_.GetMatWorld()), GenerateBoundingSphereRadius(),
		ADXMatrix4::Transform(col.pos_, col.GetGameObject()->transform_.GetMatWorld()), col.GenerateBoundingSphereRadius()))
	{
		return false;
	}
	ADXVector3 closestVec1 = col.ClosestPoint(ClosestPoint(ADXMatrix4::Transform(col.pos_, col.GetGameObject()->transform_.GetMatWorld())));
	ADXVector3 closestVec2 = ClosestPoint(col.ClosestPoint(ClosestPoint(ADXMatrix4::Transform(col.pos_, col.GetGameObject()->transform_.GetMatWorld()))));
	float colPointDiff = (closestVec1 - closestVec2).Length();
	if ((closestVec1 - closestVec2).Length() <= 0)
	{
		return true;
	}

	closestVec1 = ClosestPoint(col.ClosestPoint(ADXMatrix4::Transform(pos_, GetGameObject()->transform_.GetMatWorld())));
	closestVec2 = col.ClosestPoint(ClosestPoint(col.ClosestPoint(ADXMatrix4::Transform(pos_, GetGameObject()->transform_.GetMatWorld()))));
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
		if ((itr.layer1 == collideLayer_ && itr.layer2 == col->collideLayer_) ||
			(itr.layer2 == collideLayer_ && itr.layer1 == col->collideLayer_))
		{
			return;
		}
	}

	bool executePushBack = true;
	for (auto& itr : S_ignorePushBackPatterns)
	{
		if ((itr.layer1 == collideLayer_ && itr.layer2 == col->collideLayer_) ||
			(itr.layer2 == collideLayer_ && itr.layer1 == col->collideLayer_))
		{
			executePushBack = false;
		}
	}

	if (enabled_ && col->enabled_ && col->GetGameObject() != GetGameObject() && IsHit(*col))
	{
		if (executePushBack && !isTrigger_ && !col->isTrigger_)
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

			if (pushBackPriority_ == col->pushBackPriority_)
			{
				priorityCondition = 0;
			}
			else if (pushBackPriority_ < col->pushBackPriority_)
			{
				priorityCondition = 1;
			}
			else if (pushBackPriority_ > col->pushBackPriority_)
			{
				priorityCondition = 2;
			}

			int32_t ConditionStateResult = conditionState[pushableCondition][priorityCondition];

			switch (ConditionStateResult)
			{
			case 1:
				pushBackVector_ += myPushBack;
				break;
			case 2:
				col->pushBackVector_ += targetPushBack;
				break;
			case 3:
				pushBackVector_ += myPushBack * 0.5f;
				col->pushBackVector_ += targetPushBack * 0.5f;
				break;
			default:
				break;
			}
		}

		if (collideList_.empty() || !collideList_.empty() && collideList_.back() != col)
		{
			collideList_.push_back(col);
			GetGameObject()->OnCollisionHit(col, this);
		}
		if (collideList_.empty() || !col->collideList_.empty() && col->collideList_.back() != this)
		{
			col->collideList_.push_back(this);
			GetGameObject()->OnCollisionHit(this, col);
		}
	}
}

//先のCollidersUpdateで別のコライダーにぶつかっていたらオブジェクトを押し戻す
void ADXCollider::SendPushBack()
{
	if (pushable_)
	{
		GetGameObject()->transform_.localPosition_ += pushBackVector_;
		GetGameObject()->transform_.UpdateMatrix();
	}
	preTranslation_ = GetGameObject()->transform_.localPosition_;
	preMatrix_ = GetGameObject()->transform_.GetMatWorld();
	preMatrixInverse_ = GetGameObject()->transform_.GetMatWorldInverse();
	pushBackVector_ = { 0,0,0 };
}

//全てのコライダーで接触判定と押し戻しベクトルの算出を行う
//ゲーム内の全てのコライダーが入った配列を入れて使う
void ADXCollider::StaticUpdate()
{
	//現在の座標を保存しておく
	std::vector<ADXVector3> objsTranslation = {};
	for (auto& itr : ADXObject::GetObjs())
	{
		objsTranslation.push_back(itr->transform_.localPosition_);
	}

	//すべてのコライダーで移動距離÷(最小絶対半径×0.95)を求め、最も大きい値をtranslateDivNumFに入れる
	float translateDivNumF = 1;
	for (auto& colItr : S_cols)
	{
		//ついでにcollideListもこのタイミングでリセット
		colItr->collideList_.clear();


		ADXVector3 move = colItr->GetGameObject()->transform_.localPosition_ - colItr->preTranslation_;

		ADXVector3 scaleX1 = { colItr->scale_.x_,0,0 };
		ADXVector3 scaleY1 = { 0,colItr->scale_.y_,0 };
		ADXVector3 scaleZ1 = { 0,0,colItr->scale_.z_ };

		ADXMatrix4 WorldScalingMat = colItr->GetGameObject()->transform_.GetMatScale();
		WorldScalingMat *= colItr->GetGameObject()->transform_.GetMatRot();

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

		ADXVector3 limitedMove = move.Normalize() * minimumWorldRadius1 * 0.95f * colItr->maxMoveDistanceRate_;

		if (colItr->maxMoveDistanceRate_ >= 0 && move.Length() > limitedMove.Length())
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
		colItr->GetGameObject()->transform_.localPosition_ = colItr->preTranslation_;
	}

	//行列更新のついでに移動する前の座標を保存
	std::vector<ADXVector3> objsPreTranslation = {};
	for (auto& objItr : ADXObject::GetObjs())
	{
		objsPreTranslation.push_back(objItr->transform_.localPosition_);
		objItr->transform_.UpdateMatrix();
	}

	//少しづつ移動させながら当たり判定と押し戻し処理を行う
	for (int32_t i = 0; i < translateDivNumF; i++)
	{
		uint32_t index = 0;
		//移動
		for (auto& itr : ADXObject::GetObjs())
		{
			ADXVector3 move = objsTranslation[index] - objsPreTranslation[index];

			itr->transform_.localPosition_ += move / translateDivNumF;
			itr->transform_.UpdateMatrix();

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

bool ADXCollider::IsSphereHit(ADXVector3 s1Pos, float s1Rad, ADXVector3 s2Pos, float s2Rad)
{
	return (s2Pos - s1Pos).Length() <= s1Rad + s2Rad;
}