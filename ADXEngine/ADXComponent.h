#pragma once

class ADXObject;
class ADXCollider;

class ADXComponent
{
private:
	bool initialized_ = false;
	ADXObject* gameObject_ = nullptr;

public:
	//�X�V����
	void Update(ADXObject* obj);
	//Update()���O��gameObject��������
	void SetGameObject(ADXObject* obj);
	//gameObject���擾
	ADXObject* GetGameObject() const { return gameObject_; };

public:
	//�S�I�u�W�F�N�g�ň�ĂɌĂ΂��`��O����
	virtual void OnPreRender() {};
	//gameObject���`�悳��钼�O�ɌĂ΂��`��O����
	virtual void OnWillRenderObject() {};
	//gameObject���`�悳�ꂽ����ɌĂ΂��`��㏈��
	virtual void Rendered() {};
	/// <summary>
	/// <para>�A�N�Z�X�ᔽ��h�����߂̍X�V�������s���֐�</para>
	/// <para>���������ȃI�u�W�F�N�g���Q�Ƃ��Ă����炱���Ő؂��Ă����Έ��S</para>
	/// </summary>
	virtual void SafetyPhase() {};
	//gameObject��Destroy()���Ă΂ꂽ����ɌĂ΂��֐�
	virtual void OnDestroy() {};
	//gameObject�����̃R���C�_�[�ƐڐG�����u�ԂɌĂ΂�鏈��
	virtual void OnCollisionHit([[maybe_unused]] ADXCollider* col, [[maybe_unused]] ADXCollider* myCol) {};

protected:
	/// <summary>
	/// <para>�ŏ���Update���Ă΂ꂽ�^�C�~���O�ŌĂ΂��</para>
	/// <para>�h���N���X�ŗL�̏���������</para>
	/// </summary>
	virtual void UniqueInitialize() {};
	//�h���N���X�ŗL�̍X�V����
	virtual void UniqueUpdate() {};
};