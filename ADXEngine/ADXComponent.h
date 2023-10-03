#pragma once

class ADXObject;
class ADXCollider;

class ADXComponent
{
private:
	bool initialized = false;

protected:
	ADXObject* gameObject = nullptr;

public:
	void Update(ADXObject* obj);
	ADXObject* GetGameObject() { return gameObject; };
	virtual void OnCollisionHit(ADXCollider* col, ADXCollider* myCol) {};

protected:
	virtual void UniqueInitialize() {};
	virtual void UniqueUpdate() {};
};