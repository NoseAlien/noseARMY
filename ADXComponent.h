#pragma once

class ADXObject;

class ADXComponent
{
protected:
	ADXObject* gameObject = nullptr;

public:
	void Update(ADXObject* obj);

protected:
	virtual void UniqueUpdate();
};