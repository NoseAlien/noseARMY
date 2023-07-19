#include "ADXCollider.h"
#include "ADXObject.h"

/*
�y�ڐG���m���牟���߂��x�N�g���Z�o�܂ł̗���z

�E��ԏ�̈�̓_����̐}�`�̒��ɂ��邩�𒲂ׂ�i��̗��̐}�`�������_�����L���Ă��邩�𒲂ׂ�j
����ɂ���̐}�`���ڐG���Ă��邩�𔻒肷��

�E�������Ă���Ɣ��肳�ꂽ�炻�ꂼ��̃I�u�W�F�N�g�Łw�Փ˓_�x�����߂�

�E�݂��̃I�u�W�F�N�g�̏Փ˓_�̍��W�̍��������߂��̕����Ƌ������������x�N�g���ɂȂ�
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

//��ԏ�̓_���R���C�_�[�̒��Ɏ��߂����̍��W
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

//��ԏ�̓_���R���C�_�[�̃t�`�Ɋ񂹂����̑��΍��W
ADXVector3 ADXCollider::EdgeLocalPoint(ADXVector3 pos)
{
	return EdgeLocalPoint(pos, pos);
}

//��ԏ�̓_���R���C�_�[�̃t�`�Ɋ񂹂����̑��΍��W
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

//��ԏ�̓_���R���C�_�[�̃t�`�Ɋ񂹂����̍��W
ADXVector3 ADXCollider::EdgePoint(ADXVector3 pos)
{
	return EdgePoint(pos, pos);
}

//��ԏ�̓_���R���C�_�[�̃t�`�Ɋ񂹂����̍��W
ADXVector3 ADXCollider::EdgePoint(ADXVector3 pos, ADXVector3 prePos)
{
	ADXVector3 ret = EdgeLocalPoint(pos, prePos);
	ret = ADXMatrix4::transform(ret, gameObject->transform.matWorld_);
	return ret;
}

//����̃R���C�_�[�Ƃ̏Փ˓_�̍��W
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

//�����Ԃ������Ƌ����̃x�N�g��
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

//����̃R���C�_�[�Əd�Ȃ��Ă��邩
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

//�R���C�_�[���m�ŉ��������i�����Ȃ��R���C�_�[�ɂԂ����������I�ɉ������j
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

//���CollidersUpdate�ŕʂ̃R���C�_�[�ɂԂ����Ă�����I�u�W�F�N�g�������߂�
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

//�S�ẴR���C�_�[�ŐڐG����Ɖ����߂��x�N�g���̎Z�o���s��
//�Q�[�����̑S�ẴR���C�_�[���������z������Ďg��
void ADXCollider::CollidersUpdate()
{
	//���݂̍��W��ۑ����Ă���
	std::vector<ADXVector3> objsTranslation = {};
	for (int i = 0; i < ADXObject::GetObjs().size(); i++)
	{
		objsTranslation.push_back(ADXObject::GetObjs()[i]->transform.localPosition_);
	}

	//���ׂẴR���C�_�[�ňړ�������(�ŏ���Δ��a�~0.95)�����߁A�ł��傫���l��translateDivNumF�ɓ����
	float translateDivNumF = 1;
	for (auto& colItr : cols)
	{
		//���ł�collideList�����̃^�C�~���O�Ń��Z�b�g
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

	//�S�ẴI�u�W�F�N�g���ړ�����O�̍��W�ֈړ�������
	for (auto& colItr : cols)
	{
		colItr->gameObject->transform.localPosition_ = colItr->preTranslation;
	}

	//�s��X�V�̂��łɈړ�����O�̍��W��ۑ�
	std::vector<ADXVector3> objsPreTranslation = {};
	for (auto& objItr : ADXObject::GetObjs())
	{
		objsPreTranslation.push_back(objItr->transform.localPosition_);
		objItr->transform.UpdateMatrix();
	}

	//�����Âړ������Ȃ��瓖���蔻��Ɖ����߂��������s��
	for (int i = 0; i < translateDivNumF; i++)
	{
		//�ړ�
		for (int j = 0; j < ADXObject::GetObjs().size(); j++)
		{
			ADXVector3 move = objsTranslation[j] - objsPreTranslation[j];

			ADXObject::GetObjs()[j]->transform.localPosition_ += move / translateDivNumF;
			ADXObject::GetObjs()[j]->transform.UpdateMatrix();
		}

		//�����蔻��Ɖ����߂��x�N�g���̎Z�o
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

		//�����߂�
		/*
		for (auto& colItr : cols)
		{
			colItr->SendPushBack();
		}
		*/
	}

	cols.clear();
}