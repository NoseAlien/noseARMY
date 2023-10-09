#include "ADXSprite.h"
#include "ADXUtility.h"

void ADXSprite::SetClippingStatus(const ADXVector3& maxBoxRUF, const ADXVector3& maxBoxLDB, 
	const ADXVector3& nowBoxRUF, const ADXVector3& nowBoxLDB)
{
	boxRUF = nowBoxRUF;
	boxLDB = nowBoxLDB;

	rectRD = { ADXUtility::LerpInverse(boxRUF.x,maxBoxLDB.x,maxBoxRUF.x),ADXUtility::LerpInverse(boxLDB.y,maxBoxRUF.y,maxBoxLDB.y),0 };
	rectLU = { ADXUtility::LerpInverse(boxLDB.x,maxBoxLDB.x,maxBoxRUF.x),ADXUtility::LerpInverse(boxRUF.y,maxBoxRUF.y,maxBoxLDB.y),0 };
}

void ADXSprite::UniqueInitialize()
{
	rect = ADXModel::CreateRect();
}

void ADXSprite::UniqueUpdate()
{
	GetGameObject()->model = &rect;

	rect.vertices = {
	{{-1.0f,-1.0f,0.0f},{}, {rectLU.x,rectRD.y}},//左下
	{{-1.0f,1.0f,0.0f},{},{rectLU.x,rectLU.y}},//左上
	{{1.0f,-1.0f,0.0f},{},{rectRD.x,rectRD.y}},//右下
	{{1.0f,1.0f,0.0f},{},{rectRD.x,rectLU.y}},//右上
	};

	if (boxControl)
	{
		GetGameObject()->transform.localPosition_ = (boxRUF + boxLDB);
		GetGameObject()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
		GetGameObject()->transform.localScale_ = boxRUF - boxLDB;
	}
}