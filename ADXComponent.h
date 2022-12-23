#pragma once

class ADXObject;

class ADXComponent
{
protected:
	ADXObject* gameObject = nullptr;

public:
	void Initialize(ADXObject* obj);
	void Update(ADXObject* obj);

protected:
	virtual void UniqueInitialize();
	virtual void UniqueUpdate();

};