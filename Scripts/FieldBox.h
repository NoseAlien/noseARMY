﻿#pragma once

#include "ADXObject.h"
#include <list>

class FieldBox : public ADXComponent
{
public:
	int fieldLayer_ = 0;

private:
	std::list<FieldBox*> adjacentFields_{};
	std::list<ADXObject*> insideObjects_{};

protected:
	virtual void FieldUpdate() {};
	virtual void FieldInitialize() {};
	virtual void FieldOnCollisionHit([[maybe_unused]] ADXCollider* col, [[maybe_unused]] ADXCollider* myCol) {};

private:
	void UniqueInitialize() final;
	void UniqueUpdate() final;
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol) final;
	std::list<FieldBox*> GetAdjacentFields() { return adjacentFields_; };
};