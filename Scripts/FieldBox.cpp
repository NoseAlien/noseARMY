#include "FieldBox.h"

void FieldBox::UniqueInitialize()
{
	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger_ = true;
	tempCol->colType_ = box;
	FieldInitialize();
}

void FieldBox::UniqueUpdate()
{
	for (auto& colItr : GetGameObject()->GetComponents<ADXCollider>())
	{
		for (auto& objItr : insideObjects)
		{
			bool pullBack = true;
			for (auto& fieldItr : adjacentFields)
			{
				for (auto& colItr2 : fieldItr->GetGameObject()->GetComponents<ADXCollider>())
				{
					if (objItr != nullptr && objItr->transform_.GetWorldPosition() == colItr2->ClosestPoint(objItr->transform_.GetWorldPosition()))
					{
						pullBack = false;
						break;
					}
				}
				if (!pullBack)
				{
					break;
				}
			}
			if (objItr != nullptr && pullBack)
			{
				objItr->transform_.SetWorldPosition(colItr->ClosestPoint(objItr->transform_.GetWorldPosition()));
			}
		}
	}
	insideObjects.clear();

	adjacentFields.clear();

	FieldUpdate();
}

void FieldBox::OnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	if (col->GetGameObject()->GetComponent<FieldBox>() != nullptr
		&& col->GetGameObject()->GetComponent<FieldBox>()->fieldLayer == fieldLayer)
	{
		adjacentFields.push_back(col->GetGameObject()->GetComponent<FieldBox>());
	}

	if (!col->isTrigger_ && col->pushable_)
	{
		insideObjects.push_back(col->GetGameObject());
	}

	FieldOnCollisionHit(col, myCol);
}