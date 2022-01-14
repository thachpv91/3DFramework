#include "Bullet.h"
Bullet::Bullet(){
	bulletObject = NULL;
	Load("../Resources/","bullet.txt");
}
Bullet::~Bullet(){
	for ( int i = 0; i < nGameObject; i++ )
	{
		SAFE_DEL(bulletObject[i]);
	}
	SAFE_DEL_ARRAY(bulletObject);
	//destructor;
}
void Bullet::Load(const char *path, const char *fileName){
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
	bulletObject = new GameObject * [nGameObject];
	for(int i = 0; i < nGameObject; i++)
	{
		
		char shaderId[255] = "";
		char modelId[255] = "";
		char texture2DId[255] = "";
		char textureCubeId[255] = "";
		int nTexture2D = -1, nTextureCube = -1, nLight = -1;
	//	int * texture2DId = NULL, * textureCubeId = NULL, * lightId = NULL;
		Vector3 position, rotation, scale;
		Texture ** tempTexture;
		char idObject[255] = "";

		fscanf(pFile, "\nID: %s", idObject);  // Them id Object
		bulletObject[i] = new GameObject();
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
		   bulletObject[i]->SetTexture2D(tempTexture, nTexture2D);
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
		  bulletObject[i]->SetTextureCube(tempTexture, nTextureCube);
		  tempTexture = 0;
		}
		fscanf(pFile, "\nPosition: %f, %f, %f", &position.x, &position.y, &position.z);
		fscanf(pFile, "\nRotation: %f, %f, %f", &rotation.x, &rotation.y, &rotation.z);
		fscanf(pFile, "\nScale: %f, %f, %f", &scale.x, &scale.y, &scale.z);
		bulletObject[i]->Init(position, rotation, scale);
		bulletObject[i]->SetShader(resMan->GetShader(shaderId));
		bulletObject[i]->SetModel(resMan->GetModel(modelId));
		//------------------------
		bulletObject[i]->SetId(idObject);
	}

}

void Bullet::Shoot(int Kind){
}