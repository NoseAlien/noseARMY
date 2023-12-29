#include "ADXSprite.h"
#include "ADXUtility.h"

void ADXSprite::SetClippingStatus(const ADXVector3& maxBoxRUF, const ADXVector3& maxBoxLDB, 
	const ADXVector3& nowBoxRUF, const ADXVector3& nowBoxLDB)
{
	boxRUF_ = nowBoxRUF;
	boxLDB_ = nowBoxLDB;

	//モデルのuv座標の範囲を設定
	rectRD_ = { ADXUtility::LerpInverse(boxRUF_.x_,maxBoxLDB.x_,maxBoxRUF.x_),ADXUtility::LerpInverse(boxLDB_.y_,maxBoxRUF.y_,maxBoxLDB.y_),0 };
	rectLU_ = { ADXUtility::LerpInverse(boxLDB_.x_,maxBoxLDB.x_,maxBoxRUF.x_),ADXUtility::LerpInverse(boxRUF_.y_,maxBoxRUF.y_,maxBoxLDB.y_),0 };
}

void ADXSprite::UniqueInitialize()
{
	rect_ = ADXModel::CreateRect();
}

void ADXSprite::UniqueUpdate()
{
	GetGameObject()->model_ = &rect_;

	//モデルのuv座標を設定
	rect_.vertices_ = {
	{{-1.0f,-1.0f,0.0f},{}, {rectLU_.x_,rectRD_.y_}},//左下
	{{-1.0f,1.0f,0.0f},{},{rectLU_.x_,rectLU_.y_}},//左上
	{{1.0f,-1.0f,0.0f},{},{rectRD_.x_,rectRD_.y_}},//右下
	{{1.0f,1.0f,0.0f},{},{rectRD_.x_,rectLU_.y_}},//右上
	};

	//必要に応じて3次元用のトランスフォーム設定もする
	if (boxControl_)
	{
		GetGameObject()->transform_.localPosition_ = (boxRUF_ + boxLDB_);
		GetGameObject()->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
		GetGameObject()->transform_.localScale_ = boxRUF_ - boxLDB_;
	}
}