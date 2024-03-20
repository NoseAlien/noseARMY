#pragma once

#include <string>
#include <vector>
#include <list>
#include "ADXRenderer.h"
#include "ADXModel.h"

//画像をフォントとして表示するコンポーネント
class ADXTextRenderer : public ADXRenderer
{
public:
	enum anchor
	{
		upperLeft,
		upperCenter,
		upperRight,
		middleLeft,
		middleCenter,
		middleRight,
		lowerLeft,
		lowerCenter,
		lowerRight,
	};

public:
	struct fontLetter
	{
		uint32_t font = 0;
		char character = ' ';
	};

	struct font
	{
		std::string name = "";
		std::vector<fontLetter> fontDataCells_{};
	};

	struct charAndString
	{
		char c = ' ';
		std::string str = "";
	};

public:
	std::string text_ = "";
	font* font_ = nullptr;
	float fontSize_ = 1;
	float fontExtend_ = 1;
	float fontAspect_ = 1;
	anchor anchor_{};

private:
	ADXModel model_ = ADXModel::CreateRect();
	std::vector<ADXWorldTransform>fontWtfs_;

	//文字からフォント画像を検索して追加
	uint32_t GetFontTex(const char& character);

private:
	//描画処理
	void UniqueRendering(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);

private:
	// パイプラインステートオブジェクト
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> S_pipelineState;

	//フォント
	static std::vector<font> S_fonts;

public:
	//クラスを初期化
	static void StaticInitialize();

	//フォントを追加
	static void AddFont(const std::string letters, const std::string& folderPath, const std::string& largeLetterFolderPath,
		const std::vector<charAndString>& translateFileNameDatas);

	//フォントを取得
	static font* GetFont(const std::string fontName);

private:
	//フォント内に文字データを1文字追加
	static void AddLetter(font* targetFont, const fontLetter& setLetter);
};