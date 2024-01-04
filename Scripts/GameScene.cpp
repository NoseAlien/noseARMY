#include "GameScene.h"
#include "SceneTransition.h"
#include "SceneGate.h"

#include "ADXKeyConfig.h"

void GameScene::Initialize()
{
	key_ = nullptr;

	//画像
	keyImg_ = ADXImage::LoadADXImage("texture/QUIT_TITLE.png");
	backGroundTex_ = ADXImage::LoadADXImage("texture/skyBG.png");
	groundImg_ = ADXImage::LoadADXImage("texture/GroundBlock.png");

	rect_ = ADXModel::CreateRect();
	ground_ = ADXModel::LoadADXModel("model/groundBlock.obj");

	//オブジェクト
	ADXObject* temp = ADXObject::Create();
	shutter_ = temp->AddComponent<SceneTransition>();

	temp = ADXObject::Create({ 0,5,-20 }, ADXQuaternion::EulerToQuaternion({ 0.3f,0,0 }));

	temp->transform_.localPosition_ = { 0,5,-20 };
	temp->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0.3f,0,0 });
	ADXCamera* camera = temp->AddComponent<ADXCamera>();

	temp = ADXObject::Create({ 0,2,0 });
	Player* tempPlayer = temp->AddComponent<Player>();
	tempPlayer->Initialize(camera);
	tempPlayer->LiveEntity::Initialize("player");


	//ステージ生成に使う情報
	std::vector<TransformData> floorGenerateData{};
	std::vector<TransformData> fieldGenerateData{};
	std::vector<TutorialArea::GenerateData> tutorialAreaGenerateData{};
	EnemySpawnData enemySpawnData{};
	std::vector<BattleFieldBox::GenerateData> battleFieldGenerateData{};
	std::vector<TransformData> GoalGenerateData{};

	if (SceneGate::GetNextStageName() == "1-1")
	{
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
			{{ 3,10,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6,1,6 }},
			{{ -18.5f,-14.8f,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,1 }), { 30,1,2 }},
			{{ -40,-60,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6,1,6 }},
		};
		//自機や敵が動き回れる範囲
		fieldGenerateData = {
			{{ 0,13,5 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,14,15 }},
			{{ 0,18,35 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,10,35 }},
			{{ 0,18,66 }, ADXQuaternion::EulerToQuaternion({ 0.5f,0,0 }), { 10,9,10 }},
			{{ 13.5,15,82 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 4.5,5,32 }},
			{{ 3,15.5f,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6.1f,5,6 }},
			{{ -22,-14,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,1 }), { 38,4,2 }},
			{{ -40,-50,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6,10,6 }},
		};
		//チュートリアルが表示されるエリア
		tutorialAreaGenerateData = {
			{{ 0,3,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 4,4,4 }, "texture/tutorial_move.png"},
			{{ 0,-1.5f,16 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,1,10 }, "texture/tutorial_jump.png"},
			{{ 0,10,15.5f }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,10,4 }, "texture/tutorial_jump_2.png"},
			{{ 0,23,38 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,4,15 }, "texture/tutorial_army.png"},
			{{ 3,15.5,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6,5,6 }, "texture/tutorial_army_2.png"},
		};
		//敵
		enemySpawnData.SetSpawnList({
			{ EnemySpawnData::eType_Cub_E,{2,20,40},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi,0}) },
			{ EnemySpawnData::eType_Cub_E,{-2,20,44},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi,0}) },
			{ EnemySpawnData::eType_Clex,{0,21,55},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi,0}) },
			{ EnemySpawnData::eType_Cub_E,{15,12,86},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi,0}) },
			{ EnemySpawnData::eType_Cub_E,{14,12,100},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi,0}) },
			});
		//入ると敵が出てくるエリア
		battleFieldGenerateData = {
			{{ 3,15.4f,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6,5,6 },{
			{EnemySpawnData::eType_Cub_E,{0.5f,0,0.5f},ADXQuaternion::IdentityQuaternion()},
			{EnemySpawnData::eType_Clex,{0.5f,0,-0.5f},ADXQuaternion::IdentityQuaternion()},
			{EnemySpawnData::eType_Cub_E,{-0.5f,0,0.5f},ADXQuaternion::IdentityQuaternion()},
			{EnemySpawnData::eType_Clex,{-0.5f,0,-0.5f},ADXQuaternion::IdentityQuaternion()},
			}, "enemy"},
		};
		//ゴール
		GoalGenerateData = {
			{{ -40,-50,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6,10,6 }}
		};
	}

	//設定された情報を元に生成
	//床
	for (auto& itr : floorGenerateData)
	{
		temp = ADXObject::Create(itr.localPosition, itr.localRotation, itr.localScale);
		temp->model_ = &ground_;
		temp->texture_ = groundImg_;
		ADXCollider* tempCol = temp->AddComponent<ADXCollider>();
		tempCol->colType_ = ADXCollider::box;
	}
	//自機や敵が動き回れる範囲
	for (auto& itr : fieldGenerateData)
	{
		temp = ADXObject::Create(itr.localPosition, itr.localRotation, itr.localScale);
		temp->AddComponent<FieldBox>();
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
	for (auto& itr : GoalGenerateData)
	{
		temp = ADXObject::Create(itr.localPosition, itr.localRotation, itr.localScale);
		Goal* tempGoal = temp->AddComponent<Goal>();
		tempGoal->Initialize("player");
	}

	ADXObject* backGround = ADXObject::Create();
	backGround->transform_.rectTransform_ = true;
	backGround->transform_.UpdateMatrix();
	backGround->model_ = &rect_;
	backGround->texture_ = backGroundTex_;
	backGround->renderLayer_ = -1;

	key_ = ADXObject::Duplicate(*backGround);
	key_->transform_.localPosition_ = { -0.65f,0.85f,0 };
	key_->transform_.localScale_ = { 0.3f,0.45f,1 };
	key_->transform_.UpdateMatrix();
	key_->texture_ = keyImg_;
	key_->renderLayer_ = 1;
}

void GameScene::Update()
{
	key_->transform_.localPosition_ = { -0.65f,0.85f,0 };
	key_->transform_.localPosition_.y_ += sinf(clock() * 0.001f) * 0.01f;
	key_->transform_.localScale_ = { 0.45f / ADXWindow::GetAspect(),0.45f,1 };

	if (ADXKeyConfig::GetCurrentInstance()->GetInputDown("return"))
	{
		SceneTransition::ChangeScene(2);
	}
}