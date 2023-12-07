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
		std::vector<ControllerStick> sticks{};
	};

private:
	std::list<configCell> configCells_{};

private:
	static ADXKeyConfig* S_current;

public:
	void Update();

	void AddConfigCell(configCell cell);
	void DeleteConfigCell(std::string cellName);

	configCell GetConfigCell(std::string cellName) const;

public:
	static ADXKeyConfig* GetCurrentInstance() { return S_current; }
};