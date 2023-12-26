#pragma once

#include "ADXComponent.h"
#include "ADXVector3.h"
#include "ADXMatrix4.h"
#include <list>
#include <vector>

class ADXObject;

//接触判定を取りたいオブジェクトに入れるコンポーネント
class ADXCollider : public ADXComponent
{
public:
    enum colType
    {
        box,
        sphere,
        quad,
    };

    struct collidePattern
    {
        int32_t layer1;
        int32_t layer2;
    };

public:
    bool enabled_ = true;
    bool isTrigger_ = false;
    bool pushable_ = false;
    colType colType_ = sphere;
    float radius_ = 1;
    ADXVector3 pos_ = { 0,0,0 };
    ADXVector3 scale_ = { 1,1,1 };
    float pushBackPriority_ = 0;
    int32_t collideLayer_ = 0;
    float maxMoveDistanceRate_ = 2;

private:
    ADXVector3 preTranslation_{};
    ADXMatrix4 preMatrix_{};
    ADXMatrix4 preMatrixInverse_{};
    std::list<ADXCollider*> collideList_{};
    ADXVector3 pushBackVector_ = { 0,0,0 };

public:
    //簡易接触判定に用いる球体の半径を取得
    float GenerateBoundingSphereRadius() const;

    //引数に入れた点をコライダーの中に入れた時の座標を返す
    ADXVector3 ClosestPoint(const ADXVector3& pos) const;

    //引数に入れた点をコライダーの面に入れた時の座標を返す
    ADXVector3 EdgePoint(const ADXVector3& pos);

    //引数に入れた点をコライダーの面に入れた時の座標を返す、こちらは前フレームの座標も考慮する
    ADXVector3 EdgePoint(const ADXVector3& pos, const ADXVector3& prePos);

    //引数に入れた点とコライダーの衝突点を返す
    ADXVector3 CollidePoint(const ADXVector3& pos, const ADXVector3& colSenter, const ADXVector3& move) const;

    //引数に入れたコライダーを押し返す方向
    ADXVector3 CollideVector(const ADXCollider& col);

    //引数に入れたコライダーに触れていたらtrueを返す
    bool IsHit(const ADXCollider& col);

protected:
    //引数に入れた点をコライダーの面に入れた時のローカル座標を返す
    ADXVector3 EdgeLocalPoint(const ADXVector3& pos) const;

    //引数に入れた点をコライダーの面に入れた時のローカル座標を返す、こちらは前フレームの座標も考慮する
    ADXVector3 EdgeLocalPoint(const ADXVector3& pos, const ADXVector3& prePos) const;

private:
    //初期化処理
    void UniqueInitialize();

    //更新処理
    void UniqueUpdate();

    //先のCollidersUpdateで別のコライダーにぶつかっていたらオブジェクトを移動させる
    void SendPushBack();

    //引数に入れたコライダーと押し合う
    void Collide(ADXCollider* col);

private:
    static std::list<ADXCollider*> S_cols;
    static std::vector<collidePattern> S_ignoreCollidePatterns;
    static std::vector<collidePattern> S_ignorePushBackPatterns;

public:
    //全てのコライダーで接触判定と押し戻しベクトルの算出を行う
    static void StaticUpdate();

    //簡易接触判定に使用、球体同士が触れていたらtrueを返す
    static bool IsSphereHit(ADXVector3 s1Pos,float s1Rad, ADXVector3 s2Pos, float s2Rad);
};