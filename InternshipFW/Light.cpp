#include "Light.h"
#include "Matrix.h"
#include "Timer.h"


Light::Light()
{
	lightType = -1;
	movingType = -1;
	radius = -1.0;
	speed = -1.0;
}

Light::~Light()
{
}

void Light::Update()
{
	Matrix translationMatrix, rotationMatrix, axisTranslationMatrix;

	translationMatrix.SetTranslation(this->radius, 0.0 ,0.0);
	rotationMatrix.SetRotationY(this->speed * Timer::GameTime());
	axisTranslationMatrix.SetTranslation(this->center);

	Vector4 moveVector = (translationMatrix * rotationMatrix * axisTranslationMatrix).translationVector;
	gameObject->SetPosition(Vector3(moveVector.x, moveVector.y, moveVector.z));
}

GameObject * Light::GetGameObject()
{
	return this->gameObject;
}

int Light::GetLightType()
{
	return this->lightType;
}

Vector4 Light::GetColor()
{
	return this->color;
}

int Light::GetMovingType()
{
	return this->movingType;
}

float Light::GetRadius()
{
	return this->radius;
}

float Light::GetSpeed()
{
	return this->speed;
}

void Light::SetGameObject(GameObject * newGameObject)
{
	this->gameObject = newGameObject;
	center = this->gameObject->GetPosition();
}

void Light::SetLightType(int newLightType)
{
	this->lightType = newLightType;
}

void Light::SetColor(Vector4 newColor)
{
	this->color = newColor;
}

void Light::SetMovingType(int newMovingType)
{
	this->movingType = newMovingType;
}

void Light::SetRadius(float newRadius)
{
	this->radius = newRadius;
}

void Light::SetSpeed(float newSpeed)
{
	this->speed = newSpeed;
}
