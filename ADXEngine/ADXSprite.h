#pragma once

#include "ADXObject.h"

//スプライトを表示する際に用いる、表示範囲を簡単に指定できるコンポーネント
class ADXSprite : public ADXComponent
{
public:
    bool boxControl_ = false;
    ADXVector3 boxRUF_ = { 0.5f,0.5f,0.5f };
    ADXVector3 boxLDB_ = { -0.5f,-0.5f,-0.5f };
    ADXVector3 rectLU_ = { 0,0,0 };
    ADXVector3 rectRD_ = { 1,1,0 };

private:
    ADXModel* rect_ = nullptr;

public:
    //表示範囲を指定
    void SetClippingStatus(const ADXVector3& maxBoxRUF, const ADXVector3& maxBoxLDB, 
        const ADXVector3& nowBoxRUF, const ADXVector3& nowBoxLDB);

private:
    //初期化処理
    void UniqueInitialize();

    //更新処理
    void UniqueUpdate();
};
