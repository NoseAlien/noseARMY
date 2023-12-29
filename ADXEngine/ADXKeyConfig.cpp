#include "ADXKeyConfig.h"

ADXKeyConfig* ADXKeyConfig::S_current = nullptr;

void ADXKeyConfig::Update()
{
	//これを現在のインスタンスとする
	S_current = this;
}

void ADXKeyConfig::AddConfigCell(configCell cell)
{
	//既に同じ名前のセルがあったら設定上書き、無かったら新規作成
	for (auto& itr : configCells_)
	{
		if (itr.configCode == cell.configCode)
		{
			itr = cell;
			return;
		}
	}
	configCells_.push_back(cell);
}

void ADXKeyConfig::DeleteConfigCell(std::string cellName)
{
	//引数と同じ名前のセルを削除
	configCells_.remove_if([=](auto& itr)
		{ return itr.configCode == cellName; });
}

void ADXKeyConfig::AddVecConfigCell(vecConfigCell cell)
{
	//既に同じ名前のセルがあったら設定上書き、無かったら新規作成
	for (auto& itr : vecConfigCells_)
	{
		if (itr.configCode == cell.configCode)
		{
			itr = cell;
			return;
		}
	}
	vecConfigCells_.push_back(cell);
}

void ADXKeyConfig::DeleteVecConfigCell(std::string cellName)
{
	//引数と同じ名前のセルを削除
	vecConfigCells_.remove_if([=](auto& itr)
		{ return itr.configCode == cellName; });
}

ADXKeyConfig::configCell ADXKeyConfig::GetConfigCell(std::string cellName) const
{
	//同じ名前のセルがヒットしたらそれを返す
	for (auto& itr : configCells_)
	{
		if (itr.configCode == cellName)
		{
			return itr;
		}
	}
	return configCell();
}

ADXKeyConfig::vecConfigCell ADXKeyConfig::GetVecConfigCell(std::string cellName) const
{
	//同じ名前のセルがヒットしたらそれを返す
	for (auto& itr : vecConfigCells_)
	{
		if (itr.configCode == cellName)
		{
			return itr;
		}
	}
	return vecConfigCell();
}

bool ADXKeyConfig::GetInput(std::string cellName) const
{
	ADXKeyBoardInput* keyBoard = ADXKeyBoardInput::GetCurrentInstance();
	ADXGamePadInput* gamePad = ADXGamePadInput::GetCurrentInstance();
	configCell cell = GetConfigCell(cellName);

	//ゲームパッドが接続されていたら入力状態を取る
	if (gamePad != nullptr)
	{
		for (auto& itr : cell.buttons)
		{
			if (gamePad->GetButton(itr))
			{
				return true;
			}
		}
	}

	//キーボードの入力状態を取る
	for (auto& itr : cell.keys)
	{
		if (keyBoard->GetKey(itr))
		{
			return true;
		}
	}

	return false;
}

bool ADXKeyConfig::GetInputDown(std::string cellName) const
{
	ADXKeyBoardInput* keyBoard = ADXKeyBoardInput::GetCurrentInstance();
	ADXGamePadInput* gamePad = ADXGamePadInput::GetCurrentInstance();
	configCell cell = GetConfigCell(cellName);

	//ゲームパッドが接続されていたら入力状態を取る
	if (gamePad != nullptr)
	{
		for (auto& itr : cell.buttons)
		{
			if (gamePad->GetButtonDown(itr))
			{
				return true;
			}
		}
	}

	//キーボードの入力状態を取る
	for (auto& itr : cell.keys)
	{
		if (keyBoard->GetKeyDown(itr))
		{
			return true;
		}
	}

	return false;
}

bool ADXKeyConfig::GetInputUp(std::string cellName) const
{
	ADXKeyBoardInput* keyBoard = ADXKeyBoardInput::GetCurrentInstance();
	ADXGamePadInput* gamePad = ADXGamePadInput::GetCurrentInstance();
	configCell cell = GetConfigCell(cellName);

	//ゲームパッドが接続されていたら入力状態を取る
	if (gamePad != nullptr)
	{
		for (auto& itr : cell.buttons)
		{
			if (gamePad->GetButtonUp(itr))
			{
				return true;
			}
		}
	}

	//キーボードの入力状態を取る
	for (auto& itr : cell.keys)
	{
		if (keyBoard->GetKeyUp(itr))
		{
			return true;
		}
	}

	return false;
}

ADXVector2 ADXKeyConfig::GetVecInput(std::string cellName) const
{
	ADXKeyBoardInput* keyBoard = ADXKeyBoardInput::GetCurrentInstance();
	ADXGamePadInput* gamePad = ADXGamePadInput::GetCurrentInstance();
	vecConfigCell cell = GetVecConfigCell(cellName);
	ADXVector2 ret = { 0,0 };

	//ゲームパッドが接続されていたら入力状態を取る
	if (gamePad != nullptr)
	{
		for (auto& itr : cell.buttonsUp)
		{
			if (gamePad->GetButton(itr))
			{
				ret.y_ += 1;
			}
		}
		for (auto& itr : cell.buttonsDown)
		{
			if (gamePad->GetButton(itr))
			{
				ret.y_ -= 1;
			}
		}
		for (auto& itr : cell.buttonsRight)
		{
			if (gamePad->GetButton(itr))
			{
				ret.x_ += 1;
			}
		}
		for (auto& itr : cell.buttonsLeft)
		{
			if (gamePad->GetButton(itr))
			{
				ret.x_ -= 1;
			}
		}

		for (auto& itr : cell.sticks)
		{
			ret += gamePad->GetStickVec(itr);
		}
	}

	//キーボードの入力状態を取る
	for (auto& itr : cell.keysUp)
	{
		if (keyBoard->GetKey(itr))
		{
			ret.y_ += 1;
		}
	}
	for (auto& itr : cell.keysDown)
	{
		if (keyBoard->GetKey(itr))
		{
			ret.y_ -= 1;
		}
	}
	for (auto& itr : cell.keysRight)
	{
		if (keyBoard->GetKey(itr))
		{
			ret.x_ += 1;
		}
	}
	for (auto& itr : cell.keysLeft)
	{
		if (keyBoard->GetKey(itr))
		{
			ret.x_ -= 1;
		}
	}

	return ret;
}
