#pragma once
#include "Defines.h"
#include "GameObject.h"
#include "Camera.h"
#include "Object2D.h"


class SceneManager
{
private:
	int nGameObject;
	int nObject2D;
	GameObject ** gameObject;
	Object2D   ** object2D;
	Camera * camera;

protected:
	static SceneManager * pInstance;
	SceneManager(void);
	virtual ~SceneManager(void);
	SceneManager(const SceneManager &) {}
	SceneManager& operator =(const SceneManager &) {}

public:
	static void CreateInstance();
	static SceneManager * GetInstance();
	static void DestroyInstance();

	virtual void Load(const char * path, const char * fileName);
	virtual void Unload();

	virtual void Update();
	virtual void Render();

	virtual int GetNumberOfGameObject();
	virtual GameObject ** GetGameObject();
	virtual GameObject * GetGameObjectElementAt(int index);
	virtual Camera * GetCamera(int index);

	//--------------------------------
	virtual GameObject * GetGameObject(const char *ID); 
	virtual Object2D   * GetObject2D(const char *ID);

};