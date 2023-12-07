#include "ADXKeyConfig.h"

void ADXKeyConfig::Update()
{
	S_current = this;
}

void ADXKeyConfig::AddConfigCell(configCell cell)
{
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
	configCells_.remove_if([=](auto& itr)
		{ return itr.configCode == cellName; });
}

ADXKeyConfig::configCell ADXKeyConfig::GetConfigCell(std::string cellName) const
{
	for (auto& itr : configCells_)
	{
		if (itr.configCode == cellName)
		{
			return itr;
		}
	}
	return configCell();
}