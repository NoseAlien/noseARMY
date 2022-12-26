#pragma once

#include "ADXComponent.h"
#include "ADXVector3.h"
#include "ADXMatrix4.h"
#include <list>

class ADXObject;

enum colType
{
    box,
    sphere,
};

class ADXCollider : public ADXComponent
{
private:
    static std::list<ADXCollider*> cols;

public:
    static void CollidersUpdate();

public:
    bool enabled = true;
    bool isTrigger = false;
    bool pushable_ = false;
    colType colType_ = sphere;
    float radius_ = 1;
    ADXVector3 pos_ = { 0,0,0 };
    ADXVector3 scale_ = { 1,1,1 };
    float pushBackPriority = 0;

private:
    ADXVector3 preTranslation;
    ADXMatrix4 preMatrix;
    std::list<ADXCollider*> collideList{};
    ADXVector3 pushBackVector = { 0,0,0 };

public:
    ADXCollider(ADXObject* obj);
    ADXVector3 ClosestPoint(ADXVector3 pos);
    ADXVector3 EdgePoint(ADXVector3 pos);
    ADXVector3 EdgePoint(ADXVector3 pos, ADXVector3 prePos);
    ADXVector3 CollidePoint(ADXVector3 pos, ADXVector3 colSenter, ADXVector3 move);
    ADXVector3 CollideVector(ADXCollider col);
    bool IsHit(ADXCollider col);
    void SendPushBack();
    void Collide(ADXCollider* col);

    std::list<ADXCollider*> GetCollideList() { return collideList; };

protected:
    ADXVector3 EdgeLocalPoint(ADXVector3 pos);
    ADXVector3 EdgeLocalPoint(ADXVector3 pos, ADXVector3 prePos);

private:
    void UniqueInitialize();
    void UniqueUpdate();
};