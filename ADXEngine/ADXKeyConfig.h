#pragma once
#include "ADXKeyboardInput.h"
#include "ADXGamePadInput.h"
#include <string>
#include <vector>
#include <list>

//キーコンフィグに関する情報を扱うクラス
class ADXKeyConfig
{
public:
	struct configCell
	{
		std::string configCode = "";
		std::vector<BYTE> keys{};
		std::vector<ADXGamePadInput::ControllerButton> buttons{};
	};
	struct vecConfigCell
	{
		std::string configCode = "";
		std::vector<BYTE> keysUp{};
		std::vector<BYTE> keysDown{};
		std::vector<BYTE> keysRight{};
		std::vector<BYTE> keysLeft{};
		std::vector<ADXGamePadInput::ControllerButton> buttonsUp{};
		std::vector<ADXGamePadInput::ControllerButton> buttonsDown{};
		std::vector<ADXGamePadInput::ControllerButton> buttonsRight{};
		std::vector<ADXGamePadInput::ControllerButton> buttonsLeft{};
		std::vector<ADXGamePadInput::ControllerStick> sticks{};
	};

private:
	std::list<configCell> configCells_{};
	std::list<vecConfigCell> vecConfigCells_{};

public:
	//更新処理
	void Update();

	//コンフィグを追加（ボタン入力用）
	void AddConfigCell(configCell cell);

	//コンフィグを名前で検索して削除（ボタン入力用）
	void DeleteConfigCell(std::string cellName);

	//コンフィグを追加（方向入力用）
	void AddVecConfigCell(vecConfigCell cell);

	//コンフィグを名前で検索して削除（方向入力用）
	void DeleteVecConfigCell(std::string cellName);

	//コンフィグを名前で検索して取得（ボタン入力用）
	configCell GetConfigCell(std::string cellName) const;

	//コンフィグを名前で検索して取得（方向入力用）
	vecConfigCell GetVecConfigCell(std::string cellName) const;

	//引数の名前のコンフィグに割り当てられたボタンが押されている時にtrueを返す
	bool GetInput(std::string cellName) const;

	//引数の名前のコンフィグに割り当てられたボタンが押された瞬間にtrueを返す
	bool GetInputDown(std::string cellName) const;

	//引数の名前のコンフィグに割り当てられたボタンを離した瞬間にtrueを返す
	bool GetInputUp(std::string cellName) const;

	//引数の名前のコンフィグに割り当てられた方向入力の値を返す
	ADXVector2 GetVecInput(std::string cellName) const;

private:
	static ADXKeyConfig* S_current;

public:
	//このクラスのインスタンスを取得
	static ADXKeyConfig* GetCurrentInstance() { return S_current; }
};