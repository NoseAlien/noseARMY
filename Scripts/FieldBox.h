#pragma once

#include "ADXObject.h"
#include <list>

//プレイエリア
class FieldBox : public ADXComponent
{
public:
	int fieldLayer_ = 0;

private:
	std::list<FieldBox*> adjacentFields_{};
	std::list<ADXObject*> insideObjects_{};

protected:
	//フィールド毎の固有初期化処理
	virtual void FieldInitialize() {};

	//フィールド毎の固有更新処理
	virtual void FieldUpdate() {};

	//フィールド毎の固有接触処理
	virtual void FieldOnCollisionHit([[maybe_unused]] ADXCollider* col, [[maybe_unused]] ADXCollider* myCol) {};

private:
	//---以下の関数は必要な時に自動で呼び出される---

	//初期化処理
	void UniqueInitialize() final;

	//更新処理
	void UniqueUpdate() final;

	//何かに触れた時に呼び出される
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol) final;

	//メモリ管理
	void SafetyPhase() final;

	//---以上の関数は必要な時に自動で呼び出される---

	//隣接しているFieldBoxを全て取得
	std::list<FieldBox*> GetAdjacentFields() { return adjacentFields_; };
};