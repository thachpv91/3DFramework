#include "ResourceManager.h"


ResourceManager * ResourceManager::pInstance = NULL;

ResourceManager::ResourceManager(void)
{
	nShader = 0;
	nModel = 0;
	nTexture2D = 0;
	shader = NULL;
	model = NULL;
	texture2D = NULL;
	index1 = 0;
	nSprite = 0;
}

ResourceManager::~ResourceManager(void)
{
	this->Unload();
}

void ResourceManager::CreateInstance()
{
	if ( pInstance == NULL )
	{
		pInstance = new ResourceManager();
	}
}

ResourceManager * ResourceManager::GetInstance()
{
	if ( pInstance == NULL )
	{
		pInstance = new ResourceManager();
	}
	return pInstance;
}

void ResourceManager::DestroyInstance()
{
	SAFE_DEL(pInstance);
}

void ResourceManager::Load(const char * path, const char * fileName)
{
	FILE * pFile;

	int pathLength = strlen(path);
	int fileNameLength = strlen(fileName);
	char * fullPath = new char[pathLength + fileNameLength + 1];
	strcpy(fullPath, path);
	strcpy(fullPath + pathLength, fileName);

	pFile = fopen(fullPath, "r");
	SAFE_DEL_ARRAY(fullPath);

	if ( !pFile )
	{
		printf("FILE %s NOT FOUND!\n", fullPath);
		return;
	}

	fscanf(pFile, "\n#SHADERS: %d", &nShader);
	shader = new Shader * [nShader];
	for (int i = 0; i < nShader; i++)
	{
		char shaderId[255]= "";
		fscanf(pFile, "\nID: %s", &shaderId);

		char vsPath[100], fsPath[100], tmp[100], ** state;
		int nState;
		fscanf(pFile, "\nVS SOURCE: %s", tmp);
		strcpy(vsPath, path);
		strcpy(vsPath + pathLength, tmp);
		fscanf(pFile, "\nFS SOURCE: %s", tmp);
		strcpy(fsPath, path);
		strcpy(fsPath + pathLength, tmp);
		fscanf(pFile, "\nSTATES: %d", &nState);

		state = new char * [nState];
		for ( int j = 0; j < nState; j++ )
		{
			state[j] = new char[20];
			fscanf(pFile, "\nSTATE: %s", state[j]);
		}

		shader[i] = new Shader();
		shader[i]->Init(vsPath, fsPath);
		shader[i]->SetId(shaderId);

		for ( int j = 0; j < nState; j++ )
		{
			if ( strcmp(state[j], "CULLING") == 0 )
			{
				shader[i]->AddState(GL_CULL_FACE);
			}
			else if ( strcmp(state[j], "DEPTH_TEST") == 0 )
			{
				shader[i]->AddState(GL_DEPTH_TEST);
			}
			else if ( strcmp(state[j], "ALPHA") == 0 )
			{
				shader[i]->AddState(GL_BLEND);
			}
			SAFE_DEL_ARRAY(state[j]);
		}
		SAFE_DEL_ARRAY(state);
	}

	fscanf(pFile, "\n#MODELS: %d", &nModel);
	model = new Model * [nModel];
	for (int i = 0; i < nModel; i++)
	{
		char modelId[255] = "";
		fscanf(pFile, "\nID: %s", &modelId);

		char modelPath[100], tmp[100];
		fscanf(pFile, "\nSOURCE: %s", tmp);
		strcpy(modelPath, path);
		strcpy(modelPath + pathLength, tmp);

		model[i] = new Model();
		model[i]->Load(modelPath);
		model[i]->SetId(modelId);
	}

	fscanf(pFile, "\n#2D TEXTURES: %d", &nTexture2D);
	texture2D = new Texture * [nTexture2D];
	for (int i = 0; i < nTexture2D; i++)
	{
		char texture2DId[255] = "";
		fscanf(pFile, "\nID: %s", &texture2DId);
		LOGI("\n Texture ID: %s", texture2DId);

		char texturePath[100], tilingType[20], tmp[100];
		int tiling;
		fscanf(pFile, "\nSOURCE: %s", tmp);
		strcpy(texturePath, path);
		strcpy(texturePath + pathLength, tmp);
		fscanf(pFile, "\nTILING: %s", tilingType);

		if ( strcmp(tilingType, "CLAMP") == 0 )
		{
			tiling = GL_CLAMP_TO_EDGE;
		}
		else if ( strcmp(tilingType, "REPEAT") == 0 )
		{
			tiling = GL_REPEAT;
		}
		else
		{
			tiling = GL_MIRRORED_REPEAT;
		}

		texture2D[i] = new Texture();
		texture2D[i]->Load2D(texturePath, tiling);
		texture2D[i]->SetId(texture2DId);
	}

	fscanf(pFile, "\n#CUBE TEXTURES: %d", &nTextureCube);
	textureCube = new Texture * [nTextureCube];
	for ( int i = 0; i < nTextureCube; i++ )
	{
		char textureCubeId[255] = "";
		char ** texturePath, tilingType[20], tmp[100];
		int nCubeSrc = 0, tiling = 0;
		fscanf(pFile, "\nID: %s", &textureCubeId);
		LOGI("\n Textute ID: %s", textureCubeId);
		fscanf(pFile, "\nTEXTURES: %d", &nCubeSrc);
		texturePath = new char * [nCubeSrc];
		for ( int j = 0; j < nCubeSrc; j++ )
		{
			texturePath[j] = new char[100];
			fscanf(pFile, "\nSOURCE: %s", tmp);
			strcpy(texturePath[j], path);
			strcpy(texturePath[j] + pathLength, tmp);
		}
		fscanf(pFile, "\nTILING: %s", tilingType);

		if ( strcmp(tilingType, "CLAMP") == 0 )
		{
			tiling = GL_CLAMP_TO_EDGE;
		}
		else if ( strcmp(tilingType, "REPEAT") == 0 )
		{
			tiling = GL_REPEAT;
		}
		else if ( strcmp(tilingType, "MIRROR") == 0 )
		{
			tiling = GL_MIRRORED_REPEAT;
		}

		textureCube[i] = new Texture();
		textureCube[i]->SetId(textureCubeId);
		if (nCubeSrc == 6)
		{
			textureCube[i]->LoadCube(texturePath, tiling);
		}
		else if (nCubeSrc == 1)
		{
			textureCube[i]->LoadCube(texturePath[0], tiling);
		}
		
		for ( int j = 0; j < nCubeSrc; j++ )
		{
			SAFE_DEL_ARRAY(texturePath[j]);
		}
		SAFE_DEL_ARRAY(texturePath);
	}
	//---------------------------------------
	//  LOAD SPRITE
	//--------------------------------------
	
	fscanf(pFile, "\n#Sprite: %d", &nSprite);
	if(nSprite > 0)
	{
	    sprite = new Sprite*[nSprite];
	}

	int nTextureSprite = 0;
	fscanf(pFile, "\nTEXTURES %d", &nTextureSprite);
  
	char texSpriteID[255] = "";
	int numberSprite = 0;
	char typeSprite[20] = "";
    int idSprite = 0;
	int dx = 0, dy = 0, w =0, h =0;
	int posSprite = 0;
	for(int i = 0; i < nTextureSprite; i++)
	{
		fscanf(pFile, "\n TEXTURE ID %s", &texSpriteID);
		fscanf(pFile, "\n SPRITES %d", &numberSprite);
		fscanf(pFile, "\n TYPE %s", &typeSprite);
		if( strcmp( typeSprite, "USYN") == 0)
		{
			for(int j = 0; j < numberSprite; j++)
			{
			   sprite[posSprite] = new Sprite(); 
			   fscanf(pFile, "\nID %d", &idSprite);
			   fscanf(pFile, "\nX Y Width Height  %d %d %d %d", &dx, &dy, &w, &h);
			   sprite[posSprite]->SetId(idSprite);
			   sprite[posSprite]->SetTextureId(texSpriteID);
			   sprite[posSprite]->SetXY(dx, dy);
			   sprite[posSprite]->SetWidthHeight(w, h);
			   posSprite++;
			}
		}else
		{
			int idFirt = -1, idLast = -1;
			
			fscanf(pFile, "\nIDS %d -> %d", &idFirt, &idLast);
			fscanf(pFile, "\nWidthHeight %d %d", &w, &h);
			for(int j = idFirt; j <= idLast; j++)
			{
			        sprite[posSprite] = new Sprite();
				    sprite[posSprite]->SetId(j);
				    sprite[posSprite]->SetTextureId(texSpriteID);
				    sprite[posSprite]->InitXY((j - idFirt), w, h);
					posSprite++;
			}

		}
	    
	}

}

void ResourceManager::Unload()
{	
	for (int i = 0; i < nSprite; i++)
	{
		SAFE_DEL(sprite[i]);
	}
	SAFE_DEL_ARRAY(sprite);
	for (int i = 0; i < nShader; i++)
	{
		SAFE_DEL(shader[i]);
	}
	SAFE_DEL_ARRAY(shader);
	
	for (int i = 0; i < nModel; i++)
	{
		SAFE_DEL(model[i]);
	}
	SAFE_DEL_ARRAY(model);
	
	for (int i = 0; i < nTexture2D; i++)
	{
		SAFE_DEL(texture2D[i]);
	}
	SAFE_DEL_ARRAY(texture2D);
	
	for (int i = 0; i < nTextureCube; i++)
	{
		SAFE_DEL(textureCube[i]);
	}
	SAFE_DEL_ARRAY(textureCube);
}


int ResourceManager::GetNumberOfShader()
{
	return nShader;
}

int ResourceManager::GetNumberOfModel()
{
	return nModel;
}

int ResourceManager::GetNumberOfTexture2D()
{
	return nTexture2D;
}

int ResourceManager::GetNumberOfTextureCube()
{
	return nTextureCube;
}


Shader ** ResourceManager::GetShader()
{
	return this->shader;
}

Model ** ResourceManager::GetModel()
{
	return this->model;
}

Texture ** ResourceManager::GetTexture2D()
{
	return this->texture2D;
}

Texture ** ResourceManager::GetTextureCube()
{
	return this->textureCube;
}


Shader * ResourceManager::GetShaderElementAt(int index)
{
	return this->shader[index];
}

Model * ResourceManager::GetModelElementAt(int index)
{
	return this->model[index];
}

Texture * ResourceManager::GetTexture2DElementAt(int index)
{
	return this->texture2D[index];
}

Texture * ResourceManager::GetTextureCubeElementAt(int index)
{
	return this->textureCube[index];
}
//------------------------------------------
Shader * ResourceManager::GetShader(const char *ID)
{	     
        for(register int i = 0; i < nShader; i++)
		{        
			if(strcmp(shader[i]->GetId(), ID) == 0)
			{
			      return shader[i];
			}
		}
		LOGI("\n Khong tim thay Shader %s", ID);
		return NULL;
}
Model * ResourceManager::GetModel(const char *ID)
{
        for(register int i = 0; i < nModel; i++)
		{        
			if(strcmp(model[i]->GetId(), ID) == 0)
			{
			      return model[i];
			}
		}
		LOGI("\n Khong tim thay Model %s", ID);
		return NULL;
}
Texture * ResourceManager::GetTexture2D(const char *ID)
{
        for(register int i = 0; i < nTexture2D; i++)
		{         
			if(strcmp(texture2D[i]->GetId(), ID) == 0)
			{
			      return texture2D[i];
			}
		}
		LOGI("\n Khong tim thay Texture2D %s", ID);
		return NULL;
}
Texture * ResourceManager::GetTextureCube(const char *ID)
{
        for(register int i = 0; i < nTextureCube; i++)
		{         
			if(strcmp(textureCube[i]->GetId(), ID) == 0)
			{
			      return textureCube[i];
			}
		}
		LOGI("\n Khong tim thay TextureCUbe %s", ID);
		return NULL;
}
//----------------------------------------
Sprite  * ResourceManager::GetSprite(GLint id)
{
		for(register int i = 0; i < nSprite; i++)
		{
			if( sprite[i]->GetId() == id)
			{
			      return sprite[i];
			}
		}
		LOGI("\n Khong tim thay Sprite: %d", id);
		return NULL;
}
