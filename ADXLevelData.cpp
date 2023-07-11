#include "ADXLevelData.h"
#include <fstream>

ADXLevelData ADXLevelData::Load(const std::string& filename)
{
	//�A�����ăt���p�X�𓾂�
	const std::string fullpath = "Resources/" + filename;

	//�t�@�C���X�g���[��
	std::ifstream file;

	//�t�@�C�����J��
	file.open(fullpath);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	//JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	//��
	file >> deserialized;

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"�𕶎���Ƃ��Ď擾
	std::string name =
		deserialized["name"].get<std::string>();
	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	ADXLevelData ret{};

	LoadTreeData(&ret, deserialized["objects"]);

	return ret;
}

void ADXLevelData::LoadTreeData(ADXLevelData* levelData, 
	nlohmann::json deserialized, int32_t parentIndex)
{
	//"objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized)
	{
		assert(object.contains("type"));

		//��ʂ��擾
		std::string type = object["type"].get<std::string>();

		//MESH
		if (type.compare("MESH") == 0)
		{
			//�v�f�ǉ�
			levelData->objs.emplace_back();
			//���ǉ������v�f�̎Q�Ƃ𓾂�
			levelDataCell& obj = levelData->objs.back();

			if (object.contains("file_name"))
			{
				obj.name = object["file_name"];
			}

			//�e��ݒ�
			obj.parentIndex = parentIndex;

			//�g�����X�t�H�[���̃p�����[�^�[�ǂݍ���
			nlohmann::json& transform = object["transform"];
			//���s�ړ�
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

		//TODO:�I�u�W�F�N�g�������ċA�֐��ɂ܂Ƃ߁A�ċA�Ăяo���Ŏ}�𑖍�����
		if (object.contains("children"))
		{
			LoadTreeData(levelData, object["children"], (int32_t)levelData->objs.size() - 1);
		}
	}
}