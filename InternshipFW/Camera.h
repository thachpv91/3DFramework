#pragma once
#include "Object.h"
#include "Matrix.h"


class Camera : public Object
{
private:
	const float MAX_SPEED;
	const float MIN_SPEED;
	float speed;
	float fov;
	float nearPlane;
	float farPlane;
	Matrix * viewMatrix;
	Matrix * projectionMatrix;
	Vector3 moveVector;

public:
	Camera(float speed, float fov, float near, float far);
	virtual ~Camera(void);

	virtual void AbstractFunction() {}

	virtual void Update();
	virtual void Destroy();
	virtual Matrix * GetViewMatrix();
	virtual Matrix * GetProjectionMatrix();

	virtual void SpeedUp();
	virtual void SpeedDown();

	virtual void StopMove();
	virtual void StartMove(float);
};