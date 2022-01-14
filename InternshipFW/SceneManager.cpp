#include "SceneManager.h"
#include "ResourceManager.h"
#include "SpaceShip.h"


SceneManager * SceneManager::pInstance = NULL;

SceneManager::SceneManager(void)
{
	nGameObject = -1;
	gameObject = NULL;
	camera = NULL;

}

SceneManager::~SceneManager(void)
{
	this->Unload();
}

void SceneManager::CreateInstance()
{
	if ( pInstance == NULL )
	{
		pInstance = new SceneManager();
	}
}

SceneManager * SceneManager::GetInstance()
{
	SceneManager::CreateInstance();
	return pInstance;
}

void SceneManager::DestroyInstance()
{
	SAFE_DEL(pInstance);
}


void SceneManager::Load(const char * path, const char * fileName)
{
	FILE * pFile;
	ResourceManager * resMan = ResourceManager::GetInstance();

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

	fscanf(pFile, "\n#Objects: %d", &nGameObject);
	gameObject = new GameObject * [nGameObject];
	for(int i = 0; i < nGameObject; i++)
	{
		char shaderId[255] = "";
		char modelId[255] = "";
		char texture2DId[255] = "";
		char textureCubeId[255] = "";
		int *lightId; 

		int nTexture2D = -1, nTextureCube = -1, nLight = -1;
	//	int * texture2DId = NULL, * textureCubeId = NULL, * lightId = NULL;
		Vector3 position, rotation, scale;
		Texture ** tempTexture;
		char idObject[255] = "";

		fscanf(pFile, "\nID: %s", idObject);  // Them id Object
		if(strcmp(idObject,"SPACESHIP")== 0){
			gameObject[i] = new SpaceShip();
		}
		else{
			gameObject[i] = new GameObject();
		}
	
		fscanf(pFile, "\nShader: %s", &shaderId);
		fscanf(pFile, "\nModel: %s", &modelId);

		fscanf(pFile, "\nTextures: %d", &nTexture2D);
		if(nTexture2D > 0)
		{
			tempTexture = new Texture * [nTexture2D];
			for (int j = 0; j < nTexture2D; j++)
			{
				fscanf(pFile, "\nTextureId: %s", &texture2DId);
				tempTexture[j] = resMan->GetTexture2D(texture2DId);
			}
			gameObject[i]->SetTexture2D(tempTexture, nTexture2D);
			SAFE_DEL_ARRAY(tempTexture);
		}

		fscanf(pFile, "\nCubeTextures: %d", &nTextureCube);
		if(nTextureCube > 0)
		{
			tempTexture = new Texture * [nTextureCube];
			for (int j = 0; j < nTextureCube; j++)
			{
				fscanf(pFile, "\nCubeTextureId: %s", &texture2DId);
				tempTexture[j] = resMan->GetTextureCube(texture2DId);
			}
			gameObject[i]->SetTextureCube(tempTexture, nTextureCube);
			SAFE_DEL_ARRAY(tempTexture);
		}

		fscanf(pFile, "\nLights: %d", &nLight);
		lightId = new int[nLight];
		for (int j = 0; j < nLight; j++)
		{
			fscanf(pFile, "\nLight: %d", &lightId[j]);
		}

		fscanf(pFile, "\nPosition: %f, %f, %f", &position.x, &position.y, &position.z);
		fscanf(pFile, "\nRotation: %f, %f, %f", &rotation.x, &rotation.y, &rotation.z);
		fscanf(pFile, "\nScale: %f, %f, %f", &scale.x, &scale.y, &scale.z);
		gameObject[i]->Init(position, rotation, scale);
		gameObject[i]->SetShader(resMan->GetShader(shaderId));
		gameObject[i]->SetModel(resMan->GetModel(modelId));
		//------------------------
		gameObject[i]->SetId(idObject);

		SAFE_DEL_ARRAY(lightId);
	}

	fscanf(pFile, "\n#Camera");
	float speed, fov, nearPlane, farPlane;
	Vector3 position, rotation, scale;
	fscanf(pFile, "\nSPEED: %f", &speed);
	fscanf(pFile, "\nFOV: %f", &fov);
	fscanf(pFile, "\nNEAR: %f", &nearPlane);
	fscanf(pFile, "\nFAR: %f", &farPlane);
	fscanf(pFile, "\nPosition: %f, %f, %f", &position.x, &position.y, &position.z);
	fscanf(pFile, "\nRotation: %f, %f, %f", &rotation.x, &rotation.y, &rotation.z);
	fscanf(pFile, "\nScale: %f, %f, %f", &scale.x, &scale.y, &scale.z);
	camera = new Camera(speed, fov, nearPlane, farPlane);
	camera->Init(position, rotation, scale);
	//--------------------------------------------
    // LOAD OBJECT 2D
	fscanf(pFile, "\n#2D Objects %d", &nObject2D);
	if( nObject2D > 0)
	{
	   object2D = new Object2D*[nObject2D];
	   char shader2D[255] = "";
	   fscanf(pFile, "\nShader: %s", &shader2D);
	   for(register int i = 0; i < nObject2D; i++)
	   {
		   char id2D[255] = ""; 
		   char type2D[50] = "";

		   object2D[i] = new Object2D();
		   fscanf(pFile, "\nID %s", &id2D);
		   fscanf(pFile, "\nTYPE %s", &type2D);
		   if( strcmp(type2D, "ANIMATION") == 0)
		   {
			    int numberSprite = 0;
		        fscanf(pFile,"\nSPRITES %d", &numberSprite);
				object2D[i]->SetNumberSprite(numberSprite);

				fscanf(pFile, "\nSPRITE");
				int idSprite = -1;
				for(register int j = 0; j < numberSprite; j++)
				{
					idSprite = -1;
					fscanf(pFile, " %d", &idSprite);
					Sprite *spt = resMan->GetSprite(idSprite);
					object2D[i]->AddSprite(spt);
				}
				float speed = 0.0f;	
				fscanf(pFile, "\nSPEED %f", &speed);
				object2D[i]->SetSpeed(speed);
		   }
		   int numberState = 0;
		   fscanf(pFile, "\nSTATES %d", &numberState);
		   for(register int j = 0; j < numberState; j++)
		   {
		        fscanf(pFile, "\nSTATE %*d");
				int dx = 0, dy = 0, w = 0, h = 0, spriteID = 0;
				fscanf(pFile, "\nDxDyWidthHeightSprite %d %d %d %d %d", &dx, &dy, &w, &h, &spriteID);
				
				object2D[i]->SetTexture(spriteID);
				Sprite *spt = resMan->GetSprite(spriteID);
				object2D[i]->AddSprite(spt);
				object2D[i]->AddState(spt, dx, dy, w, h);
		   }

		object2D[i]->SetId(id2D);
		object2D[i]->SetType(type2D);
		Shader *sh = resMan->GetShader(shader2D); 
		object2D[i]->SetShader(sh);
		object2D[i]->Init();
	   }
	}


}

void SceneManager::Unload()
{
	for (int i = 0; i < nGameObject; i++)
	{
		SAFE_DEL(gameObject[i]);
	}
	SAFE_DEL_ARRAY(gameObject);

	SAFE_DEL(camera);
}

void SceneManager::Update()
{
	//Vector3 rotation = gameObject[1]->GetRotation();
	//rotation.y += 0.01f;
	//gameObject[1]->SetRotation(rotation);
	for (int i = 0; i < nGameObject; i++)
	{
		//object[i]->rotation.y += 0.01f;
		gameObject[i]->Update(camera);
	}

	camera->Update();
	for(int i = 0; i < nObject2D; i++)
	{
	   object2D[i]->Update();
	}

	
}

void SceneManager::Render()
{
	for (int i = 0; i < nGameObject; i++)
	{
		gameObject[i]->Render(camera);
	}
	for(int i = 0; i < nObject2D; i++)
	{
	   object2D[i]->Render();
	}
}

int SceneManager::GetNumberOfGameObject()
{
	return this->nGameObject;
}

GameObject ** SceneManager::GetGameObject()
{
	return this->gameObject;
}

GameObject * SceneManager::GetGameObjectElementAt(int index)
{
	return this->gameObject[index];
}

Camera * SceneManager::GetCamera(int index)
{
	return this->camera;
}

//-----------------------------
// Them thuoc tinh char *id;
GameObject * SceneManager::GetGameObject(const char *ID)
{ 
         for(register int i = 0; i < nGameObject; i++)
		 {
			 if(strcmp(gameObject[i]->GetId(), ID) == 0)
			 {
			     return gameObject[i];
			 }
		 }
		 // LOGI("\n Khong tim thay GameObject %s", ID);
		 return NULL;
}
Object2D  * SceneManager::GetObject2D(const char *ID)
{
         for(register int i = 0; i < nObject2D; i++)
		 {
			 if(strcmp(object2D[i]->GetId(), ID) == 0)
			 {
			     return object2D[i];
			 }
		 }
		 LOGI("\n Khong tim thay Object2D: %s", ID);
		 return NULL;
}