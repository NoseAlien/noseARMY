#pragma once

#include "ADXComponent.h"
#include "ADXMaterial.h"

class ADXRenderer : public ADXComponent
{
public:
	ADXMaterial material_{};

public:
	void Renderering();

private:
	virtual void UniqueRendering() {};
};