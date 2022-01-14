#include "Camera.h"
#include "InputManager.h"
#include "Timer.h"
#include "Game.h"


Camera::Camera(float speed, float fov, float nearPlane, float farPlane) :
	Object(), MAX_SPEED(10.0f), MIN_SPEED(0.0f)
{
	this->speed = speed;
	this->fov = fov;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;

	this->viewMatrix = new Matrix();
	this->projectionMatrix = new Matrix();

	this->moveVector = Vector3();
}

Camera::~Camera(void)
{
	this->Destroy();
}

void Camera::Update()
{

	float distance = this->speed * Timer::FrameTime();
	InputManager * inputManager = InputManager::GetInstance();
	
	//this->position.z -= distance;

	Matrix translationMatrix, rotationMatrix, scaleMatrix;	

	scaleMatrix.SetScale(this->scale);
	rotationMatrix.SetRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z);
	translationMatrix.SetTranslation(this->position);

	*this->worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;

	* this->viewMatrix = (* this->worldMatrix).Inverse();

	int width = Game::GetInstance()->width;
	int height = Game::GetInstance()->height;
	this->projectionMatrix->SetPerspective(fov / 2, (float) width / height, nearPlane, farPlane);
}

void Camera::Destroy()
{
	Object::Destroy();
	
	SAFE_DEL(this->viewMatrix);
	SAFE_DEL(this->projectionMatrix);
}

Matrix * Camera::GetViewMatrix()
{
	return this->viewMatrix;
}

Matrix * Camera::GetProjectionMatrix()
{
	return this->projectionMatrix;
}

void Camera::SpeedUp()
{
	if (this->speed < this->MAX_SPEED)
	{
		this->speed += 0.1f;
	}
	else
	{
		this->speed = this->MAX_SPEED;
	}
}

void Camera::SpeedDown()
{
	if (this->speed > this->MIN_SPEED)
	{
		this->speed -= 0.1f;
	}
	else
	{
		this->speed = this->MIN_SPEED;
	}
}

void Camera::StopMove(){
	this->speed = 0;
}
void Camera::StartMove(float speed){
	this->speed = speed;
}