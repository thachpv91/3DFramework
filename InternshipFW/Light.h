#pragma once
#include "GameObject.h"
#include "Vector3.h"
#include "Vector4.h"

class Light
{
public:
	static const int POSITION = 0;
	static const int DIRECTION = 1;
	static const int NONE = 2;
	static const int CIRCLE = 3;

private:
	GameObject * gameObject;
	int lightType;
	Vector4 color;
	int movingType;
	float radius;
	float speed;
	Vector3 center;

public:
	Light();
	virtual ~Light();

	virtual void AbstractFunction() {}

	virtual void Update();

	virtual GameObject * GetGameObject();
	virtual int GetLightType();
	virtual Vector4 GetColor();
	virtual int GetMovingType();
	virtual float GetRadius();
	virtual float GetSpeed();

	virtual void SetGameObject(GameObject * newGameObject);
	virtual void SetLightType(int newLightType);
	virtual void SetColor(Vector4 newColor);
	virtual void SetMovingType(int newMovingType);
	virtual void SetRadius(float newRadius);
	virtual void SetSpeed(float newSpeed);
};
