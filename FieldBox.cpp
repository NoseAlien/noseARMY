#include "FieldBox.h"

void FieldBox::UniqueUpdate()
{
	for (auto& colItr : colliders)
	{
		for (auto& colItr2 : colItr.GetCollideList())
		{
			if (!colItr2->isTrigger && colItr2->pushable_)
			{
				colItr2->GetGameObject()->transform.SetWorldPosition(colItr.ClosestPoint(colItr2->GetGameObject()->transform.GetWorldPosition()));
			}
		}
	}
}
