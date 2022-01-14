#pragma once
#include "GameState.h"
#include "GameObject.h"
#include "Camera.h"

class HelloTriangles : public GameState
{
private:
	//int nObject;
	//GameObject **object;
	//Camera *camera;

public:
	HelloTriangles(void);
	virtual ~HelloTriangles(void);

	virtual void Init(const char *path);
	virtual void Update();
	virtual void Render();
	virtual void Destroy();
};
