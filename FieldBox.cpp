#include "FieldBox.h"

std::list<FieldBox*> FieldBox::allFieldPtr{};
std::list<FieldBox*> FieldBox::fields{};

void FieldBox::UniqueUpdate()
{
	for (auto& objItr : GetFields())
	{
		for (auto& colItr : colliders)
		{
			for (auto& colItr2 : colItr.GetCollideList())
			{
				if (colItr2->GetGameObject() == objItr)
				{
					adjacentFields.push_back(objItr);
				}
			}
		}
	}

	for (auto& colItr : colliders)
	{
		for (auto& colItr2 : colItr.GetCollideList())
		{
			if (!colItr2->isTrigger && colItr2->pushable_)
			{
				bool pullBack = true;
				for (auto& objItr : adjacentFields)
				{
					for (auto& colItr3 : objItr->colliders)
					{
						if (colItr2->GetGameObject()->transform.GetWorldPosition() == colItr3.ClosestPoint(colItr2->GetGameObject()->transform.GetWorldPosition()))
						{
							pullBack = false;
							break;
						}
					}
					if (pullBack)
					{
						break;
					}
				}
				if (pullBack)
				{
					colItr2->GetGameObject()->transform.SetWorldPosition(colItr.ClosestPoint(colItr2->GetGameObject()->transform.GetWorldPosition()));
				}
			}
		}
	}
	allFieldPtr.push_back(this);
}

void FieldBox::StaticUpdate()
{
	fields = allFieldPtr;
	allFieldPtr.clear();
}
