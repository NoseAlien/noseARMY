#pragma once
#include "ADXObject.h"
#include "json.hpp"

class ADXLevelData
{
	struct levelDataCell
	{
		bool enabled = true;
		std::string name{};
		ADXWorldTransform transform{};
		int32_t parentIndex = -1;
		bool colliderEnabled = false;
		ADXVector3 colliderPosition{};
		ADXVector3 colliderScale{};
	};

public:
	std::vector<levelDataCell> objs_{};

public:
	static ADXLevelData Load(const std::string& filename);

private:
	static void LoadTreeData(ADXLevelData* levelData, 
		nlohmann::json deserialized, int32_t parentIndex = -1);
};