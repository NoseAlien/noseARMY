#include "GameScene.h"
#include "SceneTransition.h"
#include "SceneGate.h"
#include "Pebble.h"

#include "ADXKeyConfig.h"

void GameScene::Initialize()
{
	ground_ = {};
	rect_ = {};
	shutter_ = nullptr;
	key_ = nullptr;

	//画像
	keyImg = ADXImage::LoadADXImage("texture/QUIT_TITLE.png");
	backGroundTex = ADXImage::LoadADXImage("texture/skyBG.png");
	groundImg = ADXImage::LoadADXImage("texture/GroundBlock.png");
	constructionInfoImg = ADXImage::LoadADXImage("texture/constructionInfo.png");

	//3Dモデル
	rect_ = ADXModel::CreateRect();
	ground_ = ADXModel::LoadADXModel("model/groundBlock.obj");

	//オブジェクト
	ADXObject* temp = ADXObject::Create();
	shutter_ = temp->AddComponent<SceneTransition>();


	//ステージ生成に使う情報
	transformData PlayerStartTransform{};
	std::vector<transformData> fieldGenerateData{};
	std::vector<transformData> floorGenerateData{};
	std::vector<pebbleSpawnData> pebbleGenerateData{};
	std::vector<TutorialArea::generateData> tutorialAreaGenerateData{};
	EnemySpawnData enemySpawnData{};
	std::vector<BattleFieldBox::generateData> battleFieldGenerateData{};
	std::vector<transformData> goalGenerateData{};
	std::vector<transformData> constructionSignGenerateData{};
	std::vector<transformData> constructionInfoGenerateData{};

	if (SceneGate::GetNextStageName() == "1-1")
	{
		//自機のスタート位置
		PlayerStartTransform = { {0,2,0}, ADXQuaternion::EulerToQuaternion({ 0,0,0 }) };
		//自機や敵が動き回れる範囲
		fieldGenerateData = {
			{{ 0,13,5 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,14,15 }},
			{{ 0,18,35 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,10,35 }},
			{{ 0,18,66 }, ADXQuaternion::EulerToQuaternion({ 0.5f,0,0 }), { 10,9,10 }},
			{{ 13.5,15,82 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 4.5,5,32 }},
			{{ -1,15.5f,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,5,10 }},
			{{ -30,-14,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,1 }), { 38,4,2 }},
			{{ -48,-50,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6,10,6 }},
		};
		//床
		floorGenerateData = {
			{{ 0,-1,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 2,2,2 }},
			{{ 0,-2,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,1,10 }},
			{{ 0,-1,15 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,1,5 }},
			{{ 0,10,25 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,10,5 }},
			{{ 0,9,40 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,10,10 }},
			{{ 9.5f,19.5f,40 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 0.5f,0.5f,10 }},
			{{ -9.5f,19.5f,40 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 0.5f,0.5f,10 }},
			{{ 0,19,60 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,1,10 }},
			{{ 0,18,77 }, ADXQuaternion::EulerToQuaternion({ 0.5f,0,0 }), { 10,9,1 }},
			{{ 4,10,61 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 14,1,11 }},
			{{ 13.5f,10,93 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 4.5f,1,21 }},
			{{ 10,11.5f,85 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 1,0.5f,11 }},
			{{ -1,10,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,1,10 }},
			{{ -26.5f,-14.8f,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,1 }), { 30,1,2 }},
			{{ -48,-60,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6,1,6 }},
		};
		//小石
		pebbleGenerateData = {
			{{-6,1,-2}, 0.5f},
			{{4,1,5}, 0.4f},
			{{-4,20,38}, 0.5f},
			{{4,20,46}, 0.7f},
			{{5,20,46}, 0.2f},
			{{6,20,59}, 0.3f},
			{{-2,20,67}, 0.2f},
			{{-6,15,59}, 0.3f},
			{{2,15,67}, 0.5f},
			{{-8,15,70}, 0.8f},
			{{4,15,62}, 0.2f},
			{{13,15,55}, 0.4f},
			{{17,12,82}, 0.2f},
			{{12,12,88}, 0.2f},
			{{15,12,102}, 0.3f},
			{{-1,12,116}, 0.4f},
			{{7,12,114}, 0.2f},
			{{0,12,109}, 0.2f},
			{{7,12,106}, 0.3f},
			{{7.5f,12,106.5f}, 0.2f},
			{{-7,12,116}, 1},
		};
		//チュートリアルが表示されるエリア
		tutorialAreaGenerateData = {
			{{ 0,3,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 4,4,4 }, "texture/tutorial_move.png"},
			{{ 0,-1.5f,16 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,1,10 }, "texture/tutorial_jump.png"},
			{{ 0,10,15.5f }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,10,4 }, "texture/tutorial_jump_2.png"},
			{{ 0,23,38 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,4,15 }, "texture/tutorial_army.png"},
			{{ -1,15.5f,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,5,10 }, "texture/tutorial_army_2.png"},
		};
		//敵
		enemySpawnData.SetSpawnList({
			{ EnemySpawnData::enemyType::Cub_E,{2,20,40},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi,0}) },
			{ EnemySpawnData::enemyType::Cub_E,{-2,20,44},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi,0}) },
			{ EnemySpawnData::enemyType::Clex,{0,21,55},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi,0}) },
			{ EnemySpawnData::enemyType::Cub_E,{15,12,86},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi,0}) },
			{ EnemySpawnData::enemyType::Cub_E,{14,12,100},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi,0}) },
			});
		//入ると敵が出てくるエリア
		battleFieldGenerateData = {
			{{ -1,15.4f,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,5,10 },{
			{EnemySpawnData::enemyType::Cub_E,{0.5f,0,0.5f},ADXQuaternion::IdentityQuaternion()},
			{EnemySpawnData::enemyType::Clex,{0.5f,0,-0.5f},ADXQuaternion::IdentityQuaternion()},
			{EnemySpawnData::enemyType::Cub_E,{-0.5f,0,0.5f},ADXQuaternion::IdentityQuaternion()},
			{EnemySpawnData::enemyType::Clex,{-0.5f,0,-0.5f},ADXQuaternion::IdentityQuaternion()},
			}, "enemy"},
		};
		//ゴール
		goalGenerateData = {
			{{ -48,-50,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6,10,6 }}
		};
	}
	else if (SceneGate::GetNextStageName() == "1-2")
	{
		//自機のスタート位置
		PlayerStartTransform = { {0,4,-19}, ADXQuaternion::EulerToQuaternion({ 0,0,0 }) };
		//自機や敵が動き回れる範囲
		fieldGenerateData = {
			{{ 0,5,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,5,20 }},
			{{ 0,5,30 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 3,5,10 }},
			{{ 0,5,40 }, ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi * 0.25f,0 }), { 5,5,5 }},
			{{ 10,5,40 }, ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi * 0.5f,0 }), { 3,5,10 }},
			{{ -10,5,40 }, ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi * -0.5f,0 }), { 3,5,10 }},
			{{ 0,5,50 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 3,5,10 }},
			{{ 0,5,85 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,7,30 }},
		};
		//床
		floorGenerateData = {
			{{ 0,-1,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,2,20 }},
			{{ 3,5,20 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 0.3f,5,0.3f }},
			{{ 1.5f,5,20 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 0.3f,5,0.3f }},
			{{ 0,5,20 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 0.3f,5,0.3f }},
			{{ -1.5f,5,20 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 0.3f,5,0.3f }},
			{{ -3,5,20 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 0.3f,5,0.3f }},
			{{ 0,-1,30 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 3,2,10 }},
			{{ 0,-1,40 }, ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi * 0.25f,0 }), { 5,3,5 }},
			{{ 10,-1,40 }, ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi * 0.5f,0 }), { 3,2,10 }},
			{{ -10,-1,40 }, ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi * -0.5f,0 }), { 3,2,10 }},
			{{ 0,-1,50 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 3,2,10 }},
			{{ 0,0,60 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,2,5 }},
			{{ -7.5f,0,70 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 2.5f,2,5 }},
			{{ 0,-3,70 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 5,2,5 }},
			{{ 7.5f,0,70 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 2.5f,2,5 }},
			{{ 0,0,80 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,2,5 }},
			{{ 0,0,100 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,2,15 }},
		};
		//小石
		pebbleGenerateData = {
			{{3,3,73}, 1},
			{{3,3,67}, 1},
			{{-3,3,73}, 1},
			{{-3,3,67}, 1},
			{{1.5f,3,71.5f}, 1},
			{{1.5f,3,68.5f}, 1},
			{{-1.5f,3,71.5f}, 1},
			{{-1.5f,3,68.5f}, 1},
			{{0,3,70}, 1},
			{{3,1,73}, 1.5f},
			{{3,1,67}, 1.5f},
			{{-3,1,73}, 1.5f},
			{{-3,1,67}, 1.5f},
			{{0,1,70}, 1.5f},
		};
		//敵
		enemySpawnData.SetSpawnList({
			{ EnemySpawnData::enemyType::Cub_E,{7,3,0},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi * 1.25f,0}) },
			{ EnemySpawnData::enemyType::Cub_E,{-7,3,10},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi * 0.75f,0}) },
			{ EnemySpawnData::enemyType::Clex,{0,8,40},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi,0}) },
			{ EnemySpawnData::enemyType::Clex,{2,5,40},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi,0}) },
			{ EnemySpawnData::enemyType::Clex,{-2,5,40},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi,0}) },
		});
		//入ると敵が出てくるエリア
		battleFieldGenerateData = {
			{{ 0,5,70 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,7,15 },{
			{EnemySpawnData::enemyType::Cub_E,{0.5f,0,0.5f},ADXQuaternion::IdentityQuaternion()},
			{EnemySpawnData::enemyType::Clex,{0.5f,0,-0.5f},ADXQuaternion::IdentityQuaternion()},
			{EnemySpawnData::enemyType::Cub_E,{-0.5f,0,0.5f},ADXQuaternion::IdentityQuaternion()},
			{EnemySpawnData::enemyType::Clex,{-0.5f,0,-0.5f},ADXQuaternion::IdentityQuaternion()},
			}, "enemy"},
		};
		//ゴール
		goalGenerateData = {
			{{ 0,5,105 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,10,10 }}
		};
	}
	else if (SceneGate::GetNextStageName() == "1-BOSS")
	{
		//自機のスタート位置
		PlayerStartTransform = { {0,4,-19}, ADXQuaternion::EulerToQuaternion({ 0,0,0 }) };
		//自機や敵が動き回れる範囲
		fieldGenerateData = {
			{{ 0,20,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,20,20 }},
			{{ 0,100,120 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 100,100,100 }},
			{{ 0,20,250 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 20,20,30 }},

		};
		//床
		floorGenerateData = {
			{{ 0,-1,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,2,20 }},
			{{ 0,-1,120 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 100,2,100 }},
			{{ 0,3,120 }, ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi / 4,0 }), { 20,4,20 }},
			{{ 50,1,170 }, ADXQuaternion::EulerToQuaternion({ -0.2f,ADXUtility::Pi / 4,0.2f }), { 20,5,20 }},
			{{ 50,1,70 }, ADXQuaternion::EulerToQuaternion({ 0.2f,ADXUtility::Pi / 4,0.2f }), { 20,5,20 }},
			{{ -50,1,70 }, ADXQuaternion::EulerToQuaternion({ 0.2f,ADXUtility::Pi / 4,-0.2f }), { 20,5,20 }},
			{{ -50,1,170 }, ADXQuaternion::EulerToQuaternion({ -0.2f,ADXUtility::Pi / 4,-0.2f }), { 20,5,20 }},
			{{ 0,-1,250 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 20,2,30 }},
		};
		//小石
		pebbleGenerateData = {
			{{3,3,3}, 1},
			{{20,3,200}, 1},
			{{-20,3,100}, 1},
			{{80,3,70}, 1},
		};
		//入ると敵が出てくるエリア
		battleFieldGenerateData = {
			{{ 0,100,120 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 100,100,100 },{
			{EnemySpawnData::enemyType::Crazer_Unown,{0,-0.5f,0},ADXQuaternion::IdentityQuaternion()},
			}, "enemy"},
		};
		//ゴール
		goalGenerateData = {
			{{ 0,20.5f,260 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 20,19.49f,20 }}
		};
	}

	//設定された情報を元に生成
	//自機
	temp = ADXObject::Create(PlayerStartTransform.localPosition, PlayerStartTransform.localRotation);
	temp->transform_.UpdateMatrix();
	Player* tempPlayer = temp->AddComponent<Player>();
	tempPlayer->SetTeam("player");
	//自機や敵が動き回れる範囲
	for (auto& itr : fieldGenerateData)
	{
		temp = ADXObject::Create(itr.localPosition, itr.localRotation, itr.localScale);
		temp->AddComponent<FieldBox>();
	}
	//床
	for (auto& itr : floorGenerateData)
	{
		temp = ADXObject::Create(itr.localPosition, itr.localRotation, itr.localScale);
		temp->GetComponent<ADXModelRenderer>()->model_ = &ground_;
		temp->GetComponent<ADXModelRenderer>()->texture_ = groundImg;
		ADXCollider* tempCol = temp->AddComponent<ADXCollider>();
		tempCol->colType_ = ADXCollider::box;
	}
	//小石
	for (auto& itr : pebbleGenerateData)
	{
		temp = ADXObject::Create(itr.position, ADXQuaternion::IdentityQuaternion(), { itr.scale,itr.scale,itr.scale });
		temp->AddComponent<Pebble>();
	}
	//チュートリアルが表示されるエリア
	for (auto& itr : tutorialAreaGenerateData)
	{
		temp = ADXObject::Create(itr.localPosition, itr.localRotation, itr.localScale);
		TutorialArea* tempTutorialArea = temp->AddComponent<TutorialArea>();
		tempTutorialArea->SetTutorialImg(ADXImage::LoadADXImage(itr.tutorialImgName));
	}
	//敵
	enemySpawnData.Spawn("enemy");
	//入ると敵が出てくるエリア
	for (auto& itr : battleFieldGenerateData)
	{
		temp = ADXObject::Create(itr.localPosition, itr.localRotation, itr.localScale);
		BattleFieldBox* tempBattleFieldBox = temp->AddComponent<BattleFieldBox>();
		tempBattleFieldBox->Initialize(itr.setGuarders, itr.setTeam);
		tempBattleFieldBox->fieldLayer_ = 2;
	}
	//ゴール
	for (auto& itr : goalGenerateData)
	{
		temp = ADXObject::Create(itr.localPosition, itr.localRotation, itr.localScale);
		Goal* tempGoal = temp->AddComponent<Goal>();
		tempGoal->Initialize("player");
	}
	//工事案内
	for (auto& itr : constructionInfoGenerateData)
	{
		temp = ADXObject::Create(itr.localPosition, itr.localRotation, itr.localScale);
		temp->GetComponent<ADXModelRenderer>()->model_ = &rect_;
		temp->GetComponent<ADXModelRenderer>()->texture_ = constructionInfoImg;
	}

	ADXObject* backGround = ADXObject::Create();
	backGround->transform_.rectTransform_ = true;
	backGround->transform_.UpdateMatrix();
	backGround->GetComponent<ADXModelRenderer>()->model_ = &rect_;
	backGround->GetComponent<ADXModelRenderer>()->texture_ = backGroundTex;
	backGround->renderLayer_ = -1;

	key_ = ADXObject::Duplicate(*backGround);
	key_->transform_.localPosition_ = { -0.65f,0.85f,0 };
	key_->transform_.localScale_ = { 0.3f,0.45f,1 };
	key_->transform_.UpdateMatrix();
	key_->GetComponent<ADXModelRenderer>()->texture_ = keyImg;
	key_->renderLayer_ = 1;
}

void GameScene::Update()
{
	key_->transform_.localPosition_ = { -0.65f,0.85f,0 };
	key_->transform_.localPosition_.y_ += sinf(clock() * 0.001f) * 0.01f;
	key_->transform_.localScale_ = { 0.45f / ADXWindow::GetInstance()->GetAspect(),0.45f,1 };

	if (ADXKeyConfig::GetCurrentInstance()->GetInputDown("return"))
	{
		SceneTransition::ChangeScene(2);
	}
}