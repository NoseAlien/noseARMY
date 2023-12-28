#include "ADXComponent.h"

void ADXComponent::Update(ADXObject* obj)
{
	//このコンポーネントが入っているオブジェクトを設定
	gameObject_ = obj;

	//最初の更新を呼ぶ前に自動的に初期化
	if (gameObject_ != nullptr)
	{
		if (!initialized_)
		{
			UniqueInitialize();
			initialized_ = true;
		}
		UniqueUpdate();
	}
}

void ADXComponent::SetGameObject(ADXObject* obj)
{
	//このコンポーネントが入っているオブジェクトを設定（未設定の場合のみ）
	if (gameObject_ == nullptr)
	{
		gameObject_ = obj;
	}
}
