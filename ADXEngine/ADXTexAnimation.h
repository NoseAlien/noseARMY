#pragma once

#include <vector>
#include "ADXImage.h"

//アニメーション
class ADXTexAnimation
{
public:
	std::vector<uint32_t> textures_{};
	bool loop_ = true;
	uint32_t delayFrame_ = 0;

private:
	uint32_t nowDelayFrame_ = 0;
	uint32_t nowTexIndex_ = 0;

public:
	//初期化処理
	ADXTexAnimation Initialize(const std::vector<uint32_t>& setTextures, uint32_t setDelayFrame, bool setLoop);

	//今表示する画像のハンドルを取得
	uint32_t GetNowTex();

	//表示する画像のインデックスを直接指定
	void SetIndex(uint32_t index);

	//今何枚目の画像を表示しているかを返す
	uint32_t GetIndex();

	//全ての画像の枚数を取得
	uint32_t GetLength();

	//更新処理
	void Update();

	//アニメーションの最後まで到達していたらtrueを返す
	bool IsAnimEnd();
};
