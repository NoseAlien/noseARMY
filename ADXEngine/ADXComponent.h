#pragma once

class ADXObject;
class ADXCollider;

class ADXComponent
{
private:
	bool initialized = false;
	ADXObject* gameObject = nullptr;

public:
	void Update(ADXObject* obj);
	void SetGameObject(ADXObject* obj);
	void SafetyPhase() {};
	ADXObject* GetGameObject() const { return gameObject; };

public:
	virtual void OnPreRender() {};
	virtual void OnWillRenderObject() {};
	virtual void Rendered() {};
	virtual void OnCollisionHit([[maybe_unused]] ADXCollider* col, [[maybe_unused]] ADXCollider* myCol) {};

protected:
	virtual void UniqueInitialize() {};
	virtual void UniqueUpdate() {};
};