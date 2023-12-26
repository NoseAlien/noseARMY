#pragma once
#include "ADXObject.h"
#include "json.hpp"

//レベルデータを格納するクラス
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
	//レベルデータ読み込み
	static ADXLevelData Load(const std::string& filename);

private:
	//レベルデータ読み込み時、ツリー構造の全オブジェクトを走査するのに使う
	static void LoadTreeData(ADXLevelData* levelData, 
		nlohmann::json deserialized, int32_t parentIndex = -1);
};