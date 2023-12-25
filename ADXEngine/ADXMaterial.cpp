#include "ADXMaterial.h"

#include <fstream>
#include <sstream>
#include <string>

ADXMaterial ADXMaterial::LoadMaterial(const std::string& filePath)
{
	ADXMaterial material;

	//ファイルストリーム
	std::ifstream file;
	//マテリアルファイルを開く
	file.open("Resources/" + filePath);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//1行ずつ読み込む
	std::string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		std::string key_;
		getline(line_stream, key_, ' ');

		//先頭のタブは無視する
		if (key_[0] == *"\t")
		{
			key_.erase(key_.begin());
		}

		//先頭文字列がnewmtlならマテリアル名
		if (key_ == "newmtl")
		{
			line_stream >> material.name_;
		}
		//先頭文字列がKaならアンビエント色
		if (key_ == "Ka")
		{
			line_stream >> material.ambient_.x;
			line_stream >> material.ambient_.y;
			line_stream >> material.ambient_.z;
		}
		//先頭文字列がKdならディフューズ色
		if (key_ == "Kd")
		{
			line_stream >> material.diffuse_.x;
			line_stream >> material.diffuse_.y;
			line_stream >> material.diffuse_.z;
		}
		//先頭文字列がKsならスペキュラー色
		if (key_ == "Ks")
		{
			line_stream >> material.specular_.x;
			line_stream >> material.specular_.y;
			line_stream >> material.specular_.z;
		}
		//先頭文字列がmap_Kdならテクスチャファイル名
		/*if (key == "map_Kd")
		{
			//テクスチャのファイル名読み込み
			line_stream >> material.textureFilename;
			//テクスチャ読み込み
			LoadTexture(directoryPath, material.textureFilename);
		}*/
	}
	file.close();

	return material;
}