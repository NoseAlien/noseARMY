#include "ADXCollider.h"
#include "ADXObject.h"

/*
【接触検知から押し戻しベクトル算出までの流れ】

・空間上の一つの点が二つの図形の中にあるかを調べる（二つの立体図形が同じ点を共有しているかを調べる）
これにより二つの図形が接触しているかを判定する

・当たっていると判定されたらそれぞれのオブジェクトで『衝突点』を求める

・互いのオブジェクトの衝突点の座標の差が押し戻しの方向と強さが入ったベクトルになる
*/

std::list<ADXCollider*> ADXCollider::cols = {};

ADXCollider::ADXCollider(ADXObject* obj)
{
	Initialize(obj);
}

void ADXCollider::UniqueInitialize()
{
	preTranslation = gameObject->transform.localPosition_;
	preMatrix = gameObject->transform.matWorld_;
}

void ADXCollider::UniqueUpdate()
{
	cols.push_back(this);
}

//空間上の点をコライダーの中に収めた時の座標
ADXVector3 ADXCollider::ClosestPoint(ADXVector3 pos)
{
	ADXVector3 ret = ADXMatrix4::transform(pos, gameObject->transform.matWorld_.Inverse());
	ADXVector3 closPos = ret;

	if (colType_ == box)
	{
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
	}
	else if (colType_ == sphere)
	{
		if ((closPos - pos_).length() > radius_)
		{
			closPos = pos_ + ADXVector3::normalized(closPos - pos_)* radius_;
		}
	}
	else
	{
		closPos = pos_;
	}

	if ((closPos - ret).length() > 0)
	{
		ret = closPos;
		ret = ADXMatrix4::transform(ret, gameObject->transform.matWorld_);
	}
	else
	{
		ret = pos;
	}

	return ret;
}

//空間上の点をコライダーのフチに寄せた時の相対座標
ADXVector3 ADXCollider::EdgeLocalPoint(ADXVector3 pos)
{
	return EdgeLocalPoint(pos, pos);
}

//空間上の点をコライダーのフチに寄せた時の相対座標
ADXVector3 ADXCollider::EdgeLocalPoint(ADXVector3 pos, ADXVector3 prePos)
{
	ADXVector3 ret = ADXMatrix4::transform(pos, gameObject->transform.matWorld_.Inverse());
	ret -= pos_;

	ADXVector3 prevPos = ADXMatrix4::transform(prePos, preMatrix.Inverse());
	prevPos -= pos_;

	if (colType_ == box)
	{
		ret.x /= scale_.x;
		ret.y /= scale_.y;
		ret.z /= scale_.z;

		prevPos.x /= scale_.x;
		prevPos.y /= scale_.y;
		prevPos.z /= scale_.z;

		ADXVector3 absLocalPos = prevPos;
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
	}
	else if (colType_ == sphere)
	{
		ret = ADXVector3::normalized(ret) * radius_;
	}
	else
	{
		ret = { 0,0,0 };
	}

	ret += pos_;
	ret = ADXMatrix4::transform(ClosestPoint(ADXMatrix4::transform(ret, gameObject->transform.matWorld_)), gameObject->transform.matWorld_.Inverse());

	return ret;
}

//空間上の点をコライダーのフチに寄せた時の座標
ADXVector3 ADXCollider::EdgePoint(ADXVector3 pos)
{
	return EdgePoint(pos, pos);
}

//空間上の点をコライダーのフチに寄せた時の座標
ADXVector3 ADXCollider::EdgePoint(ADXVector3 pos, ADXVector3 prePos)
{
	ADXVector3 ret = EdgeLocalPoint(pos, prePos);
	ret = ADXMatrix4::transform(ret, gameObject->transform.matWorld_);
	return ret;
}

//相手のコライダーとの衝突点の座標
ADXVector3 ADXCollider::CollidePoint(ADXVector3 pos, ADXVector3 targetColSenter, ADXVector3 move)
{
	ADXVector3 ret = EdgeLocalPoint(pos, pos - move);
	ADXVector3 targetLocalSenter = ADXMatrix4::transform(targetColSenter, gameObject->transform.matWorld_.Inverse()) - pos_;

	if (targetLocalSenter.dot(ret) < 0)
	{
		ret = -ret;
	}

	ret = ADXMatrix4::transform(ret, gameObject->transform.matWorld_);
	return ret;
}

//押し返す方向と強さのベクトル
ADXVector3 ADXCollider::CollideVector(ADXCollider col)
{
	ADXVector3 ret;

	ADXVector3 myTranslation = ADXMatrix4::transform(pos_, gameObject->transform.matWorld_);
	ADXVector3 myMove = myTranslation - ADXMatrix4::transform(pos_, preMatrix);

	ADXVector3 targetTranslation = ADXMatrix4::transform(col.pos_, col.gameObject->transform.matWorld_);
	ADXVector3 targetMove = targetTranslation - ADXMatrix4::transform(col.pos_, col.preMatrix);

	ADXVector3 myPushBack1 = col.CollidePoint(myTranslation, myTranslation, myMove) - CollidePoint(col.CollidePoint(myTranslation, myTranslation, myMove), targetTranslation, targetMove);
	ADXVector3 myPushBack2 = col.CollidePoint(CollidePoint(targetTranslation, targetTranslation, targetMove), myTranslation, myMove) - CollidePoint(targetTranslation, targetTranslation, targetMove);

	ADXVector3 pushBackDiff = myPushBack1 - myPushBack2;

	if ((targetTranslation - myTranslation).dot(pushBackDiff) > 0)
	{
		ret = myPushBack2;
	}
	else
	{
		ret = myPushBack1;
	}

	if (ret.dot(targetTranslation - myTranslation) > 0)
	{
		ret = -ret;
	}

	return ret;
}

//相手のコライダーと重なっているか
bool ADXCollider::IsHit(ADXCollider col)
{
	ADXVector3 closestVec1 = col.ClosestPoint(ClosestPoint(ADXMatrix4::transform(col.pos_, col.gameObject->transform.matWorld_)));
	ADXVector3 closestVec2 = ClosestPoint(col.ClosestPoint(ClosestPoint(ADXMatrix4::transform(col.pos_, col.gameObject->transform.matWorld_))));
	float colPointDiff = (closestVec1 - closestVec2).length();
	if ((closestVec1 - closestVec2).length() <= 0)
	{
		return true;
	}

	closestVec1 = ClosestPoint(col.ClosestPoint(ADXMatrix4::transform(pos_, gameObject->transform.matWorld_)));
	closestVec2 = col.ClosestPoint(ClosestPoint(col.ClosestPoint(ADXMatrix4::transform(pos_, gameObject->transform.matWorld_))));
	colPointDiff = (closestVec1 - closestVec2).length();
	if ((closestVec1 - closestVec2).length() <= 0)
	{
		return true;
	}

	return false;
}

//コライダー同士で押し合う（動かないコライダーにぶつかったら一方的に押される）
void ADXCollider::Collide(ADXCollider* col)
{
	if (IsHit(*col) && enabled && col->enabled && col->gameObject != gameObject)
	{
		if (!isTrigger && !col->isTrigger)
		{
			ADXVector3 myPushBack = CollideVector(*col);
			ADXVector3 targetPushBack = col->CollideVector(*this);

			int conditionState[4][3] = { {0,0,0},{1,1,1},{2,2,2},{3,1,2} };

			int pushableCondition = 0;
			int priorityCondition = 0;

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

			int ConditionStateResult = conditionState[pushableCondition][priorityCondition];

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

		if (collideList.back() != col)
		{
			collideList.push_back(col);
			gameObject->OnCollisionHit(col, this);
		}
		if (col->collideList.back() != this)
		{
			col->collideList.push_back(this);
			gameObject->OnCollisionHit(this, col);
		}
	}
}

//先のCollidersUpdateで別のコライダーにぶつかっていたらオブジェクトを押し戻す
void ADXCollider::SendPushBack()
{
	if (pushable_)
	{
		gameObject->transform.localPosition_ += pushBackVector;
		gameObject->transform.UpdateMatrix();
	}
	preTranslation = gameObject->transform.localPosition_;
	preMatrix = gameObject->transform.matWorld_;
	pushBackVector = { 0,0,0 };
}

//全てのコライダーで接触判定と押し戻しベクトルの算出を行う
//ゲーム内の全てのコライダーが入った配列を入れて使う
void ADXCollider::CollidersUpdate()
{
	//現在の座標を保存しておく
	std::vector<ADXVector3> objsTranslation = {};
	for (int i = 0; i < ADXObject::GetObjs().size(); i++)
	{
		objsTranslation.push_back(ADXObject::GetObjs()[i]->transform.localPosition_);
	}

	//すべてのコライダーで移動距離÷(最小絶対半径×0.95)を求め、最も大きい値をtranslateDivNumFに入れる
	float translateDivNumF = 1;
	for (auto& colItr : cols)
	{
		//ついでにcollideListもこのタイミングでリセット
		colItr->collideList.clear();


		ADXVector3 move = colItr->gameObject->transform.localPosition_ - colItr->preTranslation;

		ADXVector3 scaleX1 = { colItr->scale_.x,0,0 };
		ADXVector3 scaleY1 = { 0,colItr->scale_.y,0 };
		ADXVector3 scaleZ1 = { 0,0,colItr->scale_.z };

		float worldScaleX1 = ADXMatrix4::transform(scaleX1, colItr->gameObject->transform.matScale_ * colItr->gameObject->transform.matRot_).length();
		float worldScaleY1 = ADXMatrix4::transform(scaleY1, colItr->gameObject->transform.matScale_ * colItr->gameObject->transform.matRot_).length();
		float worldScaleZ1 = ADXMatrix4::transform(scaleZ1, colItr->gameObject->transform.matScale_ * colItr->gameObject->transform.matRot_).length();

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

		float moveDivnum1 = move.length() / (minimumWorldRadius1 * 0.95f);
		if (moveDivnum1 >= translateDivNumF)
		{
			translateDivNumF = moveDivnum1;
		}
	}
	translateDivNumF = ceilf(translateDivNumF);

	//全てのオブジェクトを移動する前の座標へ移動させる
	for (auto& colItr : cols)
	{
		colItr->gameObject->transform.localPosition_ = colItr->preTranslation;
	}

	//行列更新のついでに移動する前の座標を保存
	std::vector<ADXVector3> objsPreTranslation = {};
	for (auto& objItr : ADXObject::GetObjs())
	{
		objsPreTranslation.push_back(objItr->transform.localPosition_);
		objItr->transform.UpdateMatrix();
	}

	//少しづつ移動させながら当たり判定と押し戻し処理を行う
	for (int i = 0; i < translateDivNumF; i++)
	{
		//移動
		for (int j = 0; j < ADXObject::GetObjs().size(); j++)
		{
			ADXVector3 move = objsTranslation[j] - objsPreTranslation[j];

			ADXObject::GetObjs()[j]->transform.localPosition_ += move / translateDivNumF;
			ADXObject::GetObjs()[j]->transform.UpdateMatrix();
		}

		//当たり判定と押し戻しベクトルの算出
		for (auto& colItr1 : cols)
		{
			for (auto& colItr2 : cols)
			{
				if (&colItr1 != &colItr2)
				{
					colItr1->Collide(colItr2);
					colItr1->SendPushBack();
					colItr2->SendPushBack();
				}
			}
		}

		//押し戻し
		/*
		for (auto& colItr : cols)
		{
			colItr->SendPushBack();
		}
		*/
	}

	cols.clear();
}