#include "ADXMaterial.h"

#include <fstream>
#include <sstream>
#include <string>

ADXMaterial ADXMaterial::LoadMaterial(const std::string& filePath)
{
	ADXMaterial material;

	//�t�@�C���X�g���[��
	std::ifstream file;
	//�}�e���A���t�@�C�����J��
	file.open("Resources/" + filePath);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	//1�s���ǂݍ���
	std::string line;
	while (getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ�
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		std::string key_;
		getline(line_stream, key_, ' ');

		//�擪�̃^�u�͖�������
		if (key_[0] == *"\t")
		{
			key_.erase(key_.begin());
		}

		//�擪������newmtl�Ȃ�}�e���A����
		if (key_ == "newmtl")
		{
			line_stream >> material.name_;
		}
		//�擪������Ka�Ȃ�A���r�G���g�F
		if (key_ == "Ka")
		{
			line_stream >> material.ambient_.x;
			line_stream >> material.ambient_.y;
			line_stream >> material.ambient_.z;
		}
		//�擪������Kd�Ȃ�f�B�t���[�Y�F
		if (key_ == "Kd")
		{
			line_stream >> material.diffuse_.x;
			line_stream >> material.diffuse_.y;
			line_stream >> material.diffuse_.z;
		}
		//�擪������Ks�Ȃ�X�y�L�����[�F
		if (key_ == "Ks")
		{
			line_stream >> material.specular_.x;
			line_stream >> material.specular_.y;
			line_stream >> material.specular_.z;
		}
		//�擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
		/*if (key == "map_Kd")
		{
			//�e�N�X�`���̃t�@�C�����ǂݍ���
			line_stream >> material.textureFilename;
			//�e�N�X�`���ǂݍ���
			LoadTexture(directoryPath, material.textureFilename);
		}*/
	}
	file.close();

	return material;
}