#pragma once
#include "GameObject.h"
#include "ResourceManager.h"
class Bullet
{
public:
	void Load(const char* path, const char* fname);
	void Shoot(int );
	Bullet();
	~Bullet();

public:
	GameObject** bulletObject;
	int nGameObject;
};
