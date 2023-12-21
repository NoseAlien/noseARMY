#pragma once

#include "ADXRenderer.h"
#include "ADXModel.h"

class ADXModelRenderer : public ADXRenderer
{
public:
	ADXModel* model_ = nullptr;
	uint32_t texture_ = 0;

private:
	void UniqueRendering();
};