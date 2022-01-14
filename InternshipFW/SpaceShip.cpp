#include "SpaceShip.h"

SpaceShip::SpaceShip(void)
{
	MAX_SPEED = 15.0f;
	MIN_SPEED = 0.5f;
	GameObject::GameObject();
	this->speed = 0.5f;
	bullet = new Bullet();
	dan = bullet->bulletObject[0];
}

SpaceShip::~SpaceShip(void)
{
	GameObject::~GameObject();
	SAFE_DEL(bullet);
}

void SpaceShip::Update(Camera* camera){
	//this->SetPosition(camera->GetPosition()+Vector3(0.0f, 0.0f,-1.0f));

	float distance = this->speed * Timer::FrameTime();
	InputManager * inputManager = InputManager::GetInstance();
	
	//dan->SetPosition(this->GetPosition());

	this->position.z = camera->GetPosition().z -1.0f;
	int keysState = inputManager->GetKeysState();
	int maxTouch = inputManager->GetMaxTouch();
	int * touchState = inputManager->GetTouchState();

	if ( keysState & inputManager->IS_SPEED_UP )
	{
		this->SpeedUp();
	}
	if ( keysState & inputManager->IS_SPEED_DOWN )
	{
		this->SpeedDown();
	}

	bool hasMoved = false;
	if ( keysState & inputManager->IS_MOVE_LEFT_PRESSED )
	{
		hasMoved = this->MoveLeft(distance);
		//this->Shoot(0);
	}
	if ( keysState & inputManager->IS_MOVE_RIGHT_PRESSED )
	{
		hasMoved = this->MoveRight(distance);
	}
	if ( keysState & inputManager->IS_MOVE_BACKWARD_PRESSED )
	{
		hasMoved = this->MoveBackward(distance);
	}
	if ( keysState & inputManager->IS_MOVE_FORWARD_PRESSED )
	{
		hasMoved = this->MoveForward(distance);
	}
	for (int i = 0; i < maxTouch; i++)
	{
		if ( touchState[i] & inputManager->IS_MOVE_LEFT_PRESSED )
		{
			hasMoved = this->MoveLeft(distance);
		}
		if ( touchState[i] & inputManager->IS_MOVE_RIGHT_PRESSED )
		{
			hasMoved = this->MoveRight(distance);
		}
		if ( touchState[i] & inputManager->IS_MOVE_BACKWARD_PRESSED )
		{
			hasMoved = this->MoveBackward(distance);
		}
		if ( touchState[i] & inputManager->IS_MOVE_FORWARD_PRESSED )
		{
			hasMoved = this->MoveForward(distance);
		}
	}
	if (hasMoved)
	{
		this->Move();
		this->moveVector = Vector3();
	}

	if ( keysState & inputManager->IS_TURN_LEFT_PRESSED )
	{
		this->TurnLeft(distance);
		LOGI("SDSADDDDDDDD");
	}
	if ( keysState & inputManager->IS_TURN_RIGHT_PRESSED )
	{
		this->TurnRight(distance);
	}
	if ( keysState & inputManager->IS_TURN_UP_PRESSED )
	{
		this->TurnUp(distance);
	}
	if ( keysState & inputManager->IS_TURN_DOWN_PRESSED )
	{
		this->TurnDown(distance);
	}

	for (int i = 0; i < maxTouch; i++)
	{
		if ( touchState[i] & inputManager->IS_TURN_LEFT_PRESSED )
		{
			this->TurnLeft(distance);
			inputManager->ClearTouchState(i, inputManager->TURN_LEFT);
		}
		if ( touchState[i] & inputManager->IS_TURN_RIGHT_PRESSED )
		{
			this->TurnRight(distance);
			inputManager->ClearTouchState(i, inputManager->TURN_RIGHT);
		}
		if ( touchState[i] & inputManager->IS_TURN_UP_PRESSED )
		{
			this->TurnUp(distance);
			inputManager->ClearTouchState(i, inputManager->TURN_UP);
		}
		if ( touchState[i] & inputManager->IS_TURN_DOWN_PRESSED )
		{
			this->TurnDown(distance);
			inputManager->ClearTouchState(i, inputManager->TURN_DOWN);
		}
	}	
	GameObject::Update(camera);
	dan->Update(camera);
}


void SpaceShip::SpeedUp()
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

void SpaceShip::SpeedDown()
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

bool SpaceShip::MoveLeft(float distance)
{
	//Vector4 moveVector = Vector4(-distance, 0.0f, 0.0f, 1.0f);
	//moveVector = moveVector * *this->worldMatrix;
	//this->position = Vector3(moveVector.x, moveVector.y, moveVector.z);
	this->position += Vector3(-distance, 0.0f, 0.0f);
	return true;
}

bool SpaceShip::MoveRight(float distance)
{
	//Vector4 moveVector = Vector4(distance, 0.0f, 0.0f, 1.0f);
	//moveVector = moveVector * *this->worldMatrix;
	//this->position = Vector3(moveVector.x, moveVector.y, moveVector.z);
	this->position += Vector3(distance, 0.0f, 0.0f);
	return true;
}

bool SpaceShip::MoveForward(float distance)
{
	//Vector4 moveVector = Vector4(0.0f, distance, 0.0f, 1.0f);
	//moveVector = moveVector * *this->worldMatrix;
	//this->position = Vector3(moveVector.x, moveVector.y, moveVector.z);
	this->position += Vector3(0.0f, distance, 0.0f);
	return true;
}

bool SpaceShip::MoveBackward(float distance)
{
	//Vector4 moveVector = Vector4(0.0f, -distance, 0.0f, 1.0f);
	//moveVector = moveVector * *this->worldMatrix;
	//this->position = Vector3(moveVector.x, moveVector.y, moveVector.z);
	this->position += Vector3(0.0f, -distance, 0.0f);
	return true;
}

bool SpaceShip::MoveUp(float distance)
{
	//Vector4 moveVector = Vector4(0.0f, 0.0f, distance, 1.0f);
	//moveVector = moveVector * *this->worldMatrix;
	//this->position = Vector3(moveVector.x, moveVector.y, moveVector.z);
	this->position += Vector3(0.0f, 0.0f, distance);
	return true;
}

bool SpaceShip::MoveDown(float distance)
{
	//Vector4 moveVector = Vector4(0.0f, 0.0f, -distance, 1.0f);
	//moveVector = moveVector * *this->worldMatrix;
	//this->position = Vector3(moveVector.x, moveVector.y, moveVector.z);
	this->position += Vector3(0.0f, 0.0f, -distance);
	return true;
}

void SpaceShip::Move()
{
	Vector4 tmpVector(this->moveVector);
	//this->position += tmpVector;
	//tmpVector = tmpVector * *this->worldMatrix;
	//this->position = Vector3(tmpVector.x, tmpVector.y, tmpVector.z);
}

void SpaceShip::TurnLeft(float distance)
{
	this->rotation.y += distance;
}

void SpaceShip::TurnRight(float distance)
{
	this->rotation.y -= distance;
}

void SpaceShip::TurnUp(float distance)
{
	this->rotation.x += distance;
}

void SpaceShip::TurnDown(float distance)
{
	this->rotation.x -= distance;
}

void SpaceShip::Shoot(int kind){
	float distance = 5 * Timer::FrameTime();
	InputManager * inputManager = InputManager::GetInstance();
	dan = bullet->bulletObject[kind];
	//dan->SetPosition(this->GetPosition());
	Vector3 pos = dan->GetPosition();
	pos.z -= distance;
	//dan->SetPosition(Vector3(0.9, 0.0, -5.0));
	dan->SetPosition(pos);
	//dan->Update();
}

void SpaceShip::Render(Camera* &camera){
	GameObject::Render(camera);
	dan->GetShader()->EnableState();
	dan->Render(camera);
	dan->GetShader()->DisableState();
}