#pragma once
#include "GameObject.h"
#include "Bullet.h"
class MainObject : public GameObject
{
private:
	/*
		Dinh nghia cac loai dan
	*/
	Bullet* bullet;// chua loai dan
	int kindBullet;// loai dan
public:
	// Shoot dua vao 1 gameibject de nhan loai dan de ban	
	virtual void Shoot();
	// Destroy pha huy khi gap dan
	virtual void Destroy();
	virtual void SetBullet(Bullet* );
	virtual void Render(Camera* &camera);
	virtual void Update(Camera* &camera);
	virtual void SetKindBullet(int );
	MainObject(void);
	~MainObject(void);
};