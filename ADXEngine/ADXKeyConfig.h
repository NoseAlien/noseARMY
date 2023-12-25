#pragma once
#include "ADXKeyboardInput.h"
#include "ADXGamePadInput.h"
#include <string>
#include <vector>
#include <list>

class ADXKeyConfig
{
public:
	struct configCell
	{
		std::string configCode = "";
		std::vector<BYTE> keys{};
		std::vector<ControllerButton> buttons{};
	};
	struct vecConfigCell
	{
		std::string configCode = "";
		std::vector<BYTE> keysUp{};
		std::vector<BYTE> keysDown{};
		std::vector<BYTE> keysRight{};
		std::vector<BYTE> keysLeft{};
		std::vector<ControllerButton> buttonsUp{};
		std::vector<ControllerButton> buttonsDown{};
		std::vector<ControllerButton> buttonsRight{};
		std::vector<ControllerButton> buttonsLeft{};
		std::vector<ControllerStick> sticks{};
	};

private:
	std::list<configCell> configCells_{};
	std::list<vecConfigCell> vecConfigCells_{};

private:
	static ADXKeyConfig* S_current;

public:
	void Update();

	void AddConfigCell(configCell cell);
	void DeleteConfigCell(std::string cellName);

	void AddVecConfigCell(vecConfigCell cell);
	void DeleteVecConfigCell(std::string cellName);

	configCell GetConfigCell(std::string cellName) const;
	vecConfigCell GetVecConfigCell(std::string cellName) const;

	bool GetInput(std::string cellName) const;
	bool GetInputDown(std::string cellName) const;
	bool GetInputUp(std::string cellName) const;

	ADXVector2 GetVecInput(std::string cellName) const;

public:
	static ADXKeyConfig* GetCurrentInstance() { return S_current; }
};