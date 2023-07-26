#pragma once

#include "ADXObject.h"

class ADXSprite : public ADXComponent
{
public:
    bool boxControl = false;
    ADXVector3 boxRUF = { 0.5f,0.5f,0.5f };
    ADXVector3 boxLDB = { -0.5f,-0.5f,-0.5f };
    ADXVector3 rectLU = { 0,0,0 };
    ADXVector3 rectRD = { 1,1,0 };

private:
    ADXModel rect{};

public:
    void SetClippingStatus(const ADXVector3& maxBoxRUF, const ADXVector3& maxBoxLDB, 
        const ADXVector3& nowBoxRUF, const ADXVector3& nowBoxLDB);

private:
    void UniqueInitialize();
    void UniqueUpdate();
};
