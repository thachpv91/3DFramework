#include "MainObject.h"

MainObject::MainObject(void)
{
	bullet = new Bullet();
}

MainObject::~MainObject(void)
{
}

void MainObject::SetBullet(Bullet *bullet){
	this->bullet = bullet;
}

// Ban dan
void MainObject::Shoot(){
	bullet->Shoot(this->kindBullet);
}

// Set Loai dan de ban
void MainObject::SetKindBullet(int i){
	this->kindBullet = i;
}

void MainObject::Update(Camera* &camera){
	GameObject::Update(camera);
}
void MainObject::Render(Camera* &camera){
	GameObject::Render(camera);
}

void MainObject::Destroy(){
	// code to detroy
}