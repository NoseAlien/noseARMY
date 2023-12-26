#pragma once

#include "ADXScene.h"

//シーンマネージャー
class ADXSceneManager
{
private:
	ADXScene* prevScene_ = nullptr;
	ADXScene* currentScene_ = nullptr;

	std::vector<ADXScene*> scenes_{};

	int32_t prevSceneIndex_ = -1;
	int32_t sceneIndex_ = 0;
	bool reload_ = true;
	bool initializable_ = true;

private:
	static ADXSceneManager* S_current;

public:
	//初期化処理
	void Initialize();

	//更新処理
	void Update();

	//現在のシーンに割り当てられた番号を取得
	int32_t GetSceneIndex() { return sceneIndex_; };

	//シーン切り替え
	void SetSceneIndex(int32_t setSceneIndex) { sceneIndex_ = setSceneIndex; };

protected:
	//シーンマネージャー毎の固有初期化処理
	virtual void UniqueInitialize() {};

	//ゲーム全体で扱う全てのシーンを設定
	void SetScenes(const std::vector<ADXScene*>& setScenes);

public:
	//このクラスのインスタンスを取得
	static ADXSceneManager* GetCurrentInstance() { return S_current; };
};