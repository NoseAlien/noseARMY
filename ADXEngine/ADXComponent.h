#pragma once

class ADXObject;
class ADXCollider;

class ADXComponent
{
private:
	bool initialized = false;

private:
	ADXObject* gameObject = nullptr;

public:
	void Update(ADXObject* obj);
	void SetGameObject(ADXObject* obj);
	ADXObject* GetGameObject() const { return gameObject; };
	virtual void OnPreRender() {};
	virtual void OnWillRenderObject() {};
	virtual void Rendered() {};
	virtual void OnCollisionHit([[maybe_unused]] ADXCollider* col, [[maybe_unused]] ADXCollider* myCol) {};

protected:
	virtual void UniqueInitialize() {};
	virtual void UniqueUpdate() {};
};