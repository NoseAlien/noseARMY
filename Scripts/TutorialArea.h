#pragma once

#include "ADXObject.h"

//触れると操作方法が表示されるエリア
class TutorialArea : public ADXComponent
{
public:
	struct GenerateData
	{
		ADXVector3 localPosition{};
		ADXQuaternion localRotation{};
		ADXVector3 localScale{};
		std::string tutorialImgName = "";
	};

private:
	uint32_t tutorialImg_ = 0;

public:
	//表示するチュートリアル画像を設定
	void SetTutorialImg(uint32_t setTutorialImg_);

	//表示するチュートリアル画像を取得
	uint32_t GetTutorialImg() { return tutorialImg_; };

private:
	//---以下の関数は必要な時に自動で呼び出される---

	//初期化処理
	void UniqueInitialize();

	//---以上の関数は必要な時に自動で呼び出される---
};