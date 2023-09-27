#pragma once

#include "ADXObject.h"
#include <list>

class FieldBox : public ADXComponent
{
public:
	int fieldLayer = 0;

private:
	std::list<FieldBox*> adjacentFields{};
	std::list<ADXObject*> insideObjects{};

private:
	static std::list<FieldBox*> allFieldPtr;
	static std::list<FieldBox*> fields;

public:
	void Initialize();

protected:
	virtual void FieldUpdate() {};
	virtual void FieldOnCollisionHit(ADXCollider* col, ADXCollider* myCol) {};


private:
	void UniqueUpdate() final;
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol) final;
	std::list<FieldBox*> GetAdjacentFields() { return adjacentFields; };

public:
	static void StaticUpdate();
	static std::list<FieldBox*> GetFields() { return fields; };
};