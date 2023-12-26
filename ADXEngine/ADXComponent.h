#pragma once

class ADXObject;
class ADXCollider;

//オブジェクトに入れて使うコンポーネントの基底クラス
class ADXComponent
{
private:
	bool initialized_ = false;
	ADXObject* gameObject_ = nullptr;

public:
	//更新処理
	void Update(ADXObject* obj);

	//Update()より前にgameObjectを代入する
	void SetGameObject(ADXObject* obj);

	//gameObjectを取得
	ADXObject* GetGameObject() const { return gameObject_; };

public:
	//全オブジェクトで一斉に呼ばれる描画前処理
	virtual void OnPreRender() {};

	//gameObjectが描画される直前に呼ばれる描画前処理
	virtual void OnWillRenderObject() {};

	//gameObjectが描画された直後に呼ばれる描画後処理
	virtual void Rendered() {};

	/// <summary>
	/// <para>アクセス違反を防ぐための更新処理を行う関数</para>
	/// <para>消えそうなオブジェクトを参照していたらここで切っておけば安全</para>
	/// </summary>
	virtual void SafetyPhase() {};

	//gameObjectのDestroy()が呼ばれた直後に呼ばれる関数
	virtual void OnDestroy() {};

	//gameObjectが他のコライダーと接触した瞬間に呼ばれる処理
	virtual void OnCollisionHit([[maybe_unused]] ADXCollider* col, [[maybe_unused]] ADXCollider* myCol) {};

protected:
	/// <summary>
	/// <para>最初のUpdateが呼ばれたタイミングで呼ばれる</para>
	/// <para>派生クラス固有の初期化処理</para>
	/// </summary>
	virtual void UniqueInitialize() {};

	//派生クラス固有の更新処理
	virtual void UniqueUpdate() {};
};