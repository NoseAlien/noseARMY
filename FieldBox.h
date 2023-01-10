#pragma once

#include "ADXObject.h"

class FieldBox : public ADXObject
{
private:
	std::list<FieldBox*> adjacentFields{};

private:
	static std::list<FieldBox*> allFieldPtr;
	static std::list<FieldBox*> fields;

private:
	void UniqueUpdate();
	std::list<FieldBox*> GetAdjacentFields() { return adjacentFields; };

public:
	static void StaticUpdate();
	static std::list<FieldBox*> GetFields() { return fields; };
};