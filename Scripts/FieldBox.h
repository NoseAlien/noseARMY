#pragma once

#include "ADXObject.h"
#include <list>

class FieldBox : public ADXObject
{
public:
	int fieldLayer = 0;

private:
	std::list<FieldBox*> adjacentFields{};

private:
	static std::list<FieldBox*> allFieldPtr;
	static std::list<FieldBox*> fields;

public:
	void Initialize();

protected:
	virtual void FieldUpdate();

private:
	void UniqueUpdate();
	std::list<FieldBox*> GetAdjacentFields() { return adjacentFields; };

public:
	static void StaticUpdate();
	static std::list<FieldBox*> GetFields() { return fields; };
};