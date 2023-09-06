#pragma once

class ADXObject;

class ADXComponent
{
protected:
	ADXObject* gameObject = nullptr;

public:
	void Initialize(ADXObject* obj);
	void Update(ADXObject* obj);
	ADXObject* GetGameObject() { return gameObject; };

protected:
	virtual void UniqueInitialize() {};
	virtual void UniqueUpdate() {};

};