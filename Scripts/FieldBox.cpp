#include "FieldBox.h"

void FieldBox::UniqueInitialize()
{
	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger = true;
	tempCol->colType_ = box;
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
					if (objItr != nullptr && objItr->transform.GetWorldPosition() == colItr2->ClosestPoint(objItr->transform.GetWorldPosition()))
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
				objItr->transform.SetWorldPosition(colItr->ClosestPoint(objItr->transform.GetWorldPosition()));
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

	if (!col->isTrigger && col->pushable_)
	{
		insideObjects.push_back(col->GetGameObject());
	}

	FieldOnCollisionHit(col, myCol);
}