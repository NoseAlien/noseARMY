#pragma once

#include "ADXObject.h"

//ゴール
class Goal : public ADXComponent
{
public:
	const int32_t MaxSceneTransitionCount = 60;

private:
	std::string team_ = "";
	int32_t sceneTransitionCount_ = MaxSceneTransitionCount;
	ADXObject* clearUI_ = nullptr;
	ADXObject* keyUI_ = nullptr;
	ADXModel* boxModel_ = nullptr;
	ADXModel* rectModel_ = nullptr;

public:
	//初期化処理
	void Initialize(const std::string& setTeam);

private:
	//---以下の関数は必要な時に自動で呼び出される---

	//更新処理
	void UniqueUpdate();

	//何かに触れた時に呼び出される
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol);

	//---以上の関数は必要な時に自動で呼び出される---
};