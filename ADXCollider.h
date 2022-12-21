#pragma once
#include "ADXVector3.h"
#include "ADXMatrix4.h"
#include <vector>

class ADXObject;

enum colType
{
    box,
    sphere,
};

class ADXCollider
{
private:
    static std::vector<ADXCollider*> cols;

public:
    static void CollidersUpdate();

protected:
    ADXVector3 EdgeLocalPoint(ADXVector3 pos);
    ADXVector3 EdgeLocalPoint(ADXVector3 pos, ADXVector3 prePos);

public:
    bool enabled = true;
    bool isTrigger = false;
    bool pushable_ = false;
    colType colType_ = sphere;
    ADXObject* gameObject = nullptr;
    float radius_ = 1;
    ADXVector3 pos_ = { 0,0,0 };
    ADXVector3 scale_ = { 1,1,1 };
    std::vector<ADXCollider*> collideList{};
    ADXVector3 pushBackVector = { 0,0,0 };

    ADXCollider(ADXObject* obj);
    void Update(ADXObject* obj);
    ADXVector3 ClosestPoint(ADXVector3 pos);
    ADXVector3 EdgePoint(ADXVector3 pos);
    ADXVector3 EdgePoint(ADXVector3 pos, ADXVector3 prePos);
    ADXVector3 CollidePoint(ADXVector3 pos, ADXVector3 colSenter, ADXVector3 move);
    ADXVector3 CollideVector(ADXCollider col);
    bool IsHit(ADXCollider col);
    void SendPushBack();
    void Collide(ADXCollider* col);

private:
    ADXVector3 preTranslation;
    ADXMatrix4 preMatrix;
};