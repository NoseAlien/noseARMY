#include "ADXLevelData.h"
#include <fstream>

ADXLevelData ADXLevelData::Load(const std::string& filename)
{
	//連結してフルパスを得る
	const std::string fullpath = "Resources/" + filename;

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name =
		deserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	ADXLevelData ret{};

	LoadTreeData(&ret, deserialized["objects"]);

	return ret;
}

void ADXLevelData::LoadTreeData(ADXLevelData* levelData, 
	nlohmann::json deserialized, int32_t parentIndex)
{
	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized)
	{
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//MESH
		if (type.compare("MESH") == 0)
		{
			//要素追加
			levelData->objs.emplace_back();
			//今追加した要素の参照を得る
			levelDataCell& obj = levelData->objs.back();

			if (object.contains("file_name"))
			{
				obj.name = object["file_name"];
			}

			//親を設定
			obj.parentIndex = parentIndex;

			//トランスフォームのパラメーター読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			obj.transform.localPosition_ =
				ADXVector3{
				(float)transform["translation"][0],
				(float)transform["translation"][2],
				(float)transform["translation"][1] };
			obj.transform.localRotation_ = ADXQuaternion::EulerToQuaternion(
				ADXVector3{
				-(float)transform["rotation"][0],
				-(float)transform["rotation"][2],
				(float)transform["rotation"][1] });
			obj.transform.localScale_ =
				ADXVector3{
				(float)transform["scaling"][0],
				(float)transform["scaling"][2],
				(float)transform["scaling"][1] };

			if (object.contains("collider"))
			{
				obj.colliderEnabled = true;
				nlohmann::json& collider = object["collider"];
				obj.colliderPosition =
					ADXVector3{
					(float)collider["center"][0],
					(float)collider["center"][2],
					(float)collider["center"][1] };
				obj.colliderScale =
					ADXVector3{
					(float)collider["size"][0],
					(float)collider["size"][2],
					(float)collider["size"][1] } *0.5f;
			}
		}

		//TODO:オブジェクト走査を再帰関数にまとめ、再帰呼び出しで枝を走査する
		if (object.contains("children"))
		{
			LoadTreeData(levelData, object["children"], (int32_t)levelData->objs.size() - 1);
		}
	}
}