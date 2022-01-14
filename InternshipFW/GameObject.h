#pragma once
#include "Object.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

class GameObject : public Object
{
protected:
	Shader * shader;
	Model * model;
	int nTexture2D;
	int nTextureCube;
	Texture ** texture2D;
	Texture ** textureCube;
	int nLight;
	int * lightId;

public:
	GameObject(void);
	virtual ~GameObject(void);

	virtual void AbstractFunction() {}

	virtual void Render(Camera * &camera);
	virtual void Destroy();

	//virtual int GetNumberOfLightId();

	virtual Shader * GetShader();
	//virtual int GetLightIdElementAt(int index);

	virtual void SetShader(Shader * shader);
	virtual void SetModel(Model * model);
	virtual void SetTexture2D(Texture ** newTexture2D, int nTexture2D);
	virtual void SetTextureCube(Texture ** newTextureCube, int nTextureCube);
	virtual void Update(Camera*);
	//virtual void SetLightId(int * newLightId, int nLight);
};
