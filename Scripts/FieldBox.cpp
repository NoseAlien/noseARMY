#include "FieldBox.h"

std::list<FieldBox*> FieldBox::allFieldPtr{};
std::list<FieldBox*> FieldBox::fields{};

void FieldBox::Initialize()
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
					if (objItr->transform.GetWorldPosition() == colItr2->ClosestPoint(objItr->transform.GetWorldPosition()))
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
			if (pullBack)
			{
				objItr->transform.SetWorldPosition(colItr->ClosestPoint(objItr->transform.GetWorldPosition()));
			}
		}
	}
	insideObjects.clear();

	adjacentFields.clear();
	allFieldPtr.push_back(this);

	FieldUpdate();
}

void FieldBox::OnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	for (auto& objItr : GetFields())
	{
		if (col->GetGameObject() == objItr->GetGameObject() && col->GetGameObject() != gameObject
			&& objItr->fieldLayer == fieldLayer)
		{
			adjacentFields.push_back(objItr);
		}
	}

	if (!col->isTrigger && col->pushable_)
	{
		insideObjects.push_back(col->GetGameObject());
	}

	FieldOnCollisionHit(col, myCol);
}

void FieldBox::StaticUpdate()
{
	fields = allFieldPtr;
	allFieldPtr.clear();
}
