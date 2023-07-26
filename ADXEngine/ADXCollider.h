#pragma once

#include "ADXComponent.h"
#include "ADXVector3.h"
#include "ADXMatrix4.h"
#include <list>
#include <vector>

class ADXObject;

enum colType
{
    box,
    sphere,
    plain,
    quad,
};

struct collidePattern
{
    int32_t layer1;
    int32_t layer2;
};

class ADXCollider : public ADXComponent
{
private:
    static std::list<ADXCollider*> S_cols;

public:
    static void StaticUpdate();

public:
    bool enabled = true;
    bool isTrigger = false;
    bool pushable_ = false;
    colType colType_ = sphere;
    float radius_ = 1;
    ADXVector3 pos_ = { 0,0,0 };
    ADXVector3 scale_ = { 1,1,1 };
    float pushBackPriority = 0;
    int32_t collideLayer = 0;

private:
    ADXVector3 preTranslation;
    ADXMatrix4 preMatrix;
    ADXMatrix4 preMatrixInverse;
    std::list<ADXCollider*> collideList{};
    ADXVector3 pushBackVector = { 0,0,0 };

public:
    ADXCollider(ADXObject* obj);
    ADXVector3 ClosestPoint(const ADXVector3& pos) const;
    ADXVector3 EdgePoint(const ADXVector3& pos);
    ADXVector3 EdgePoint(const ADXVector3& pos, const ADXVector3& prePos);
    ADXVector3 CollidePoint(const ADXVector3& pos, const ADXVector3& colSenter, const ADXVector3& move) const;
    ADXVector3 CollideVector(const ADXCollider& col);
    bool IsHit(const ADXCollider& col);
    std::list<ADXCollider*> GetCollideList() { return collideList; };

protected:
    ADXVector3 EdgeLocalPoint(const ADXVector3& pos) const;
    ADXVector3 EdgeLocalPoint(const ADXVector3& pos, const ADXVector3& prePos) const;

private:
    void UniqueInitialize();
    void UniqueUpdate();

    void SendPushBack();
    void Collide(ADXCollider* col);

private:
    static std::vector<collidePattern> S_ignoreCollidePatterns;
    static std::vector<collidePattern> S_ignorePushBackPatterns;
};