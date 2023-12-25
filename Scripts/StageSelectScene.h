#pragma once

#include "ADXScene.h"
#include "ADXObject.h"
#include "ADXCamera.h"
#include "Player.h"
#include "FieldBox.h"
#include "ADXTexAnimation.h"
#include "SceneGate.h"
#include "SceneTransition.h"

class StageSelectScene : public ADXScene
{
private:
	//�摜
	uint32_t keyImg_{};
	uint32_t backGroundTex_{};
	uint32_t groundImg_{};

	//3D���f��
	ADXModel ground_{};
	ADXModel rect_{};

	//�J����
	ADXCamera* camera_ = nullptr;

	//�I�u�W�F�N�g
	SceneTransition* shutter_ = nullptr;
	ADXObject* key_ = nullptr;
	Player* player_ = nullptr;
	std::list<ADXObject*> floors_{};
	std::list<TutorialArea*> tutorialAreas_{};
	std::list<FieldBox*> fields_{};
	std::list<SceneGate*> gates_{};

	ADXObject* backGround_{};

public:
	void Initialize();
	void Update();
};