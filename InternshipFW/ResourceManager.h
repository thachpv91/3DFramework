#pragma once
#include "Defines.h"
#include "Shader.h"
#include "Model.h"
#include "Texture.h"
#include "Sprite.h"

class ResourceManager
{
private:
	int index1;
private:
	int nShader;
	int nModel;
	int nTexture2D;
	int nTextureCube;
	int nSprite;
	Shader ** shader;
	Model ** model;
	Texture ** texture2D;
	Texture ** textureCube;
	Sprite  ** sprite;



protected:
	static ResourceManager * pInstance;
	ResourceManager(void);
	virtual ~ResourceManager(void);
	ResourceManager(const ResourceManager &) {}
	ResourceManager& operator =(const ResourceManager &) {}

public:
	static void CreateInstance();
	static ResourceManager * GetInstance();
	static void DestroyInstance();

	virtual void Load(const char * path, const char * fileName);
	virtual void Unload();

	virtual int GetNumberOfShader();
	virtual int GetNumberOfModel();
	virtual int GetNumberOfTexture2D();
	virtual int GetNumberOfTextureCube();

	virtual Shader ** GetShader();
	virtual Model ** GetModel();
	virtual Texture ** GetTexture2D();
	virtual Texture ** GetTextureCube();

	virtual Shader * GetShaderElementAt(int index);
	virtual Model * GetModelElementAt(int index);
	virtual Texture * GetTexture2DElementAt(int index);
	virtual Texture * GetTextureCubeElementAt(int index);
//--------------------
	virtual Shader * GetShader(const char *ID);
	virtual Model * GetModel(const char *ID);
	virtual Texture * GetTexture2D(const char *ID);
	virtual Texture * GetTextureCube(const char *ID);
//---------------------------
	virtual Sprite  * GetSprite(GLint id);
};