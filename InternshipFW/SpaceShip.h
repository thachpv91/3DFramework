#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "InputManager.h"
#include "Timer.h"
#include "Bullet.h"
class SpaceShip : public GameObject
{
private:
	enum KindWeapon{
		MISSILE,
		BULLET,
	};
	enum Item{
		
	};
	float MAX_SPEED;
	float MIN_SPEED;
	float speed;
	Vector3 moveVector;
	Bullet* bullet;
	GameObject* dan;
public:
	virtual void Update(Camera* camera);
	void Shoot(int);
	virtual void SpeedUp();
	virtual void SpeedDown();
	
	virtual void Render(Camera* &came);


	virtual bool MoveLeft(float distance);
	virtual bool MoveRight(float distance);
	virtual bool MoveUp(float distance);
	virtual bool MoveDown(float distance);
	virtual bool MoveBackward(float distance);
	virtual bool MoveForward(float distance);
	virtual void Move();

	virtual void TurnLeft(float distance);
	virtual void TurnRight(float distance);
	virtual void TurnUp(float distance);
	virtual void TurnDown(float distance);
	void OnDestroy();
	void PickItem(unsigned char);

	SpaceShip(void);
	virtual ~SpaceShip(void);
};
