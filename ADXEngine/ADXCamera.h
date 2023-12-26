#pragma once
#include "ADXObject.h"

//カメラとして使うオブジェクトに入れるコンポーネント
class ADXCamera : public ADXComponent
{

private:
	//射影変換行列
	ADXMatrix4 matProjection_{};

	//ビュー変換行列
	ADXMatrix4 matView_{};
	DirectX::XMFLOAT3 eye_{};
	DirectX::XMFLOAT3 target_{};
	DirectX::XMFLOAT3 up_{};

public:
	//---以下の関数は必要な時に自動で呼び出される---

	//レンダリング直前に呼ばれる関数
	void PrepareToRandering();

private:
	//初期化処理
	void UniqueInitialize();

	//更新処理
	void UniqueUpdate();

	//---以上の関数は必要な時に自動で呼び出される---

private:
	static ADXCamera* S_current;
	static ADXVector3 S_cameraWorldPos;

public:
	//カメラのワールド座標を取得
	static ADXVector3 GetCameraWorldPos() { return S_cameraWorldPos; };

	//現在描画に使っているカメラを取得
	static ADXCamera* GetCurrentCamera() { return S_current; };
};