#pragma once

#include "ADXObject.h"

//くぐるとシーンが切り替わるゲート
class SceneGate : public ADXComponent
{
public:
	const int32_t MaxSceneTransitionCount = 60;

private:
	std::string team_ = "";
	std::string stageName_ = "";
	bool hitted_ = false;
	ADXObject* keyUI_ = nullptr;
	ADXObject* stageNameUI_ = nullptr;
	ADXModel boxModel_{};
	ADXModel rectModel_{};

public:
	//ワープ先のステージ名を設定
	void Initialize(const std::string& team, const std::string& stageName);

	//ワープ先のステージ名を取得
	std::string GetStageName() { return stageName_; }

private:
	//---以下の関数は必要な時に自動で呼び出される---

	//更新処理
	void UniqueUpdate();

	//何かに触れた時に呼び出される
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol);

	//---以上の関数は必要な時に自動で呼び出される---

private:
	static std::string S_nextStageName;

public:
	//読み込むシーンの名前を取得
	static std::string GetNextStageName() { return S_nextStageName; }
};