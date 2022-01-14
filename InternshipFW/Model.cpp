#include "Model.h"
#include "Game.h"
#include "MTL.h"
#include "PNG.h"
#include "ResourceManager.h"


Vertex ** verticesData;
int * nVertex;

Model::Model(void)
{
	numberOfParts = 0;
	this->vboId = NULL;
	this->iboId = NULL;
	indicesData = NULL;
	nIndex = NULL;
}

Model::~Model(void)
{
	for (int i = 0; i < numberOfParts; i++)
	{
		if ( this->vboId[i] != 0 )
		{
			glDeleteBuffers(1, &vboId[i]);
			vboId[i] = 0;
		}

		if ( this->iboId[i] != 0 )
		{
			glDeleteBuffers(1, &iboId[i]);
			iboId[i] = 0;
		}

		SAFE_DEL_ARRAY(indicesData[i]);
	}
	SAFE_DEL_ARRAY(vboId);
	SAFE_DEL_ARRAY(iboId);
	SAFE_DEL_ARRAY(indicesData);
	SAFE_DEL_ARRAY(nIndex);
	SAFE_DEL(id);
}

int Model::GetNumberOfParts()
{
	return numberOfParts;
}

GLuint * Model::GetVboId()
{
	return this->vboId;
}

GLuint * Model::GetIboId()
{
	return this->iboId;
}

int * Model::GetIndexSize()
{
	return this->nIndex;
}

void Model::Load(const char * filePath)
{
	if ( strstr(filePath, ".nfg") )
	{
		this->LoadNFG(filePath, verticesData, nVertex, indicesData, nIndex);
	}
	else if ( strstr(filePath, ".obj") )
	{
		this->LoadOBJ(filePath, verticesData, nVertex, indicesData, nIndex);
	}
	else if ( strstr(filePath, ".mdl") )
	{
		this->LoadMDL(filePath, verticesData, nVertex, indicesData, nIndex);
	}

	vboId = new GLuint[numberOfParts];
	iboId = new GLuint[numberOfParts];

	if (strstr(filePath, "Terrain"))
	{
		const float SCALE_FACTOR = 30.0f;

		char tgaFileName[] = "Textures/heightmap.tga";
		char * path = Game::GetInstance()->resPath;

		char * fullFileName = new char[strlen(path) + strlen(tgaFileName) + 1];
		strcpy(fullFileName, path);
		strcpy(fullFileName + strlen(path), tgaFileName);

		int width = 0, height = 0, bpp = 0;
		unsigned char * imageData = NULL;
		if ( strstr(fullFileName, ".tga") )
		{
			imageData = LoadTGA(fullFileName, &width, &height, &bpp);
		}
		else if ( strstr(fullFileName, ".png") )
		{
			vector<unsigned char> imageBuffer = loadPNG(fullFileName, width, height);
			imageData = new unsigned char[imageBuffer.size()];
			memcpy(imageData, &imageBuffer[0], imageBuffer.size());
			bpp = 32;
		}

		for ( int i = 0; i < nVertex[0]; i++ )
		{
			Vector2 uv = verticesData[0][i].uv;
			int wUnit = (int) (width * uv.x * (bpp / 8));
			int hUnit = (int) (height * uv.y);
			verticesData[0][i].position.y += imageData[wUnit * (width - 1) + hUnit] / SCALE_FACTOR;
		}

		SAFE_DEL_ARRAY(fullFileName);
		SAFE_DEL_ARRAY(imageData);
	}

	for (int i = 0; i < numberOfParts; i++)
	{
		glGenBuffers(1, &this->vboId[i]);
		glBindBuffer(GL_ARRAY_BUFFER, this->vboId[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * nVertex[i], verticesData[i], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		SAFE_DEL_ARRAY(verticesData[i]);

		glGenBuffers(1, &this->iboId[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->iboId[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * nIndex[i], indicesData[i], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	SAFE_DEL_ARRAY(verticesData);
	SAFE_DEL_ARRAY(nVertex);
}

void Model::LoadNFG(const char * filePath, Vertex ** &vData, int * &nVertex, GLuint ** &iData, int * &nIndex)
{
	FILE * pFile = fopen(filePath, "r");

	if ( !pFile )
	{
		printf("FILE %s NOT FOUND!\n", filePath);
		return;
	}

	numberOfParts = 1;
	vData = new Vertex * [numberOfParts];
	nVertex = new int[numberOfParts];
	iData = new GLuint * [numberOfParts];
	nIndex = new int[numberOfParts];

	fscanf(pFile, "\nNrVertices: %d", &nVertex[0]);

	//SAFE_DEL_ARRAY(vData);
	vData[0] = new Vertex[nVertex[0]];
	char strFormat[] = "\n%*d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];";
	for (int i = 0; i < nVertex[0]; i++)
	{
		fscanf(pFile, strFormat,
			&vData[0][i].position.x, &vData[0][i].position.y, &vData[0][i].position.z,
			&vData[0][i].normal.x, &vData[0][i].normal.y, &vData[0][i].normal.z,
			&vData[0][i].binormal.x, &vData[0][i].binormal.y, &vData[0][i].binormal.z,
			&vData[0][i].tangent.x, &vData[0][i].tangent.y, &vData[0][i].tangent.z,
			&vData[0][i].uv.x, &vData[0][i].uv.y);
	}

	fscanf(pFile, "\nNrIndices: %d", &nIndex[0]);

	iData[0] = new GLuint[nIndex[0]];
	for (int i = 0; i < nIndex[0]; i += 3)
	{
		fscanf(pFile, "\n%*d. %d, %d, %d", &iData[0][i + 0], &iData[0][i + 1], &iData[0][i + 2]);
	}

	fclose(pFile);
}

void Model::LoadOBJ(const char * filePath, Vertex ** &vData, int * &nVertex, GLuint ** &iData, int * &nIndex)
{
	string mtlFileName(filePath);
	int fileNamePos = mtlFileName.find_last_of('/');
	int folderPos = mtlFileName.find_last_of('/', fileNamePos -1);
	mtlFileName.replace(folderPos + 1, fileNamePos - folderPos - 1, "Textures");
	mtlFileName.replace(mtlFileName.length() - 3, 3, "mtl");

	vector<string> materialNames;
	vector<string> texturePaths;
	vector<Vector4> color;
	loadMTL(mtlFileName.data(), materialNames, texturePaths, color);
	if ( materialNames.size() > 0 )
	{
		numberOfParts = materialNames.size();
	}
	else
	{
		numberOfParts = 1;
	}

	vData = new Vertex * [numberOfParts];
	nVertex = new int[numberOfParts];
	iData = new GLuint * [numberOfParts];
	nIndex = new int[numberOfParts];

	FILE * pFile = fopen(filePath, "r");

	if ( !pFile )
	{
		printf("FILE %s NOT FOUND!\n", filePath);
		return;
	}

	vector< Vector3 > tmpPosition;
	vector< Vector2 > tmpUv;
	vector< Vector3 > tmpNormal;
	vector< vector<GLuint> > positionIndices, uvIndices, normalIndices;
	for ( int i = 0; i < numberOfParts; i++ )
	{
		vector<GLuint> position, uv, normal;
		positionIndices.push_back(position);
		uvIndices.push_back(uv);
		normalIndices.push_back(normal);
	}

	int index = -1;
	while (!feof(pFile))
	{
		char lineHeader[128];
		fscanf(pFile, "\n%s", lineHeader);
		if ( strcmp(lineHeader, "usemtl") == 0 )
		{
			char mtlName[256];
			fscanf(pFile, "\n%s", mtlName);
			for (int i = 0; i < numberOfParts; i++)
			{
				if ( materialNames.size() > 0 )
				{
					if ( strcmp(materialNames[i].data(), mtlName) == 0 )
					{
						index = i;
						break;
					}
				}
			}
			if (index == -1)
			{
				index = 0;
			}
		}
		else if ( strcmp(lineHeader, "v") == 0 )
		{
			Vector3 position;
			fscanf(pFile, "\n%f %f %f", &position.x, &position.y, &position.z);
			tmpPosition.push_back(position);
		}
		else if ( strcmp(lineHeader, "vt") == 0 )
		{
			Vector2 uv;
			fscanf(pFile, "\n%f %f", &uv.x, &uv.y);
			tmpUv.push_back(uv);
		}
		else if ( strcmp(lineHeader, "vn") == 0 )
		{
			Vector3 normal;
			fscanf(pFile, "\n%f %f %f", &normal.x, &normal.y, &normal.z);
			tmpNormal.push_back(normal);
		}
		else if ( strcmp(lineHeader, "f") == 0 )
		{
			int positionIndex[3] = {-1}, uvIndex[3] = {-1}, normalIndex[3] = {-1};
			char buffer[256];
			fgets(buffer, 256, pFile);
			int matches = sscanf(buffer, "%d/%d/%d %d/%d/%d %d/%d/%d",
				&positionIndex[0], &uvIndex[0], &normalIndex[0],
				&positionIndex[1], &uvIndex[1], &normalIndex[1],
				&positionIndex[2], &uvIndex[2], &normalIndex[2]);
			if ( matches != 9 )
			{
				matches = sscanf(buffer, "%d/%d %d/%d %d/%d",
					&positionIndex[0], &uvIndex[0],
					&positionIndex[1], &uvIndex[1],
					&positionIndex[2], &uvIndex[2]);
				if ( matches != 6 )
				{
					matches = sscanf(buffer, "%d//%d %d//%d %d//%d",
						&positionIndex[0], &normalIndex[0],
						&positionIndex[1], &normalIndex[1],
						&positionIndex[2], &normalIndex[2]);
				}
			}
			positionIndices[index].push_back(positionIndex[0]);
			positionIndices[index].push_back(positionIndex[1]);
			positionIndices[index].push_back(positionIndex[2]);
			uvIndices[index].push_back(uvIndex[0]);
			uvIndices[index].push_back(uvIndex[1]);
			uvIndices[index].push_back(uvIndex[2]);
			normalIndices[index].push_back(normalIndex[0]);
			normalIndices[index].push_back(normalIndex[1]);
			normalIndices[index].push_back(normalIndex[2]);
		}
		else
		{
			char tmp[1024];
			fgets(tmp, 1024, pFile);
		}
	}

	fclose(pFile);

	for ( int i = 0; i < numberOfParts; i++ )
	{
		nVertex[i] = nIndex[i] = positionIndices[i].size();
		vData[i] = new Vertex[nVertex[i]];
		iData[i] = new GLuint[nIndex[i]];
		for ( int j = 0; j < nVertex[i]; j++ )
		{
			if ( positionIndices[i].size() > 0 )
			{
				vData[i][j].position = tmpPosition[positionIndices[i][j] - 1];
			}
			if ( color.size() > 0 )
			{
				vData[i][j].color = color[i];
			}
			if ( uvIndices[i].size() > 0 )
			{
				vData[i][j].uv = tmpUv[uvIndices[i][j] - 1];
			}
			if ( normalIndices[i].size() > 0 )
			{
				vData[i][j].normal = tmpNormal[normalIndices[i][j] - 1];
			}
			iData[i][j] = j;
		}
	}
}

void Model::LoadMDL(const char * filePath, Vertex ** &vData, int * &nVertex, GLuint ** &iData, int * &nIndex)
{
	string mtlFileName(filePath);
	int fileNamePos = mtlFileName.find_last_of('/');
	int folderPos = mtlFileName.find_last_of('/', fileNamePos -1);
	mtlFileName.replace(folderPos + 1, fileNamePos - folderPos - 1, "Textures");
	mtlFileName.replace(mtlFileName.length() - 3, 3, "mtl");

	vector<string> materialNames;
	vector<string> texturePaths;
	vector<Vector4> color;
	loadMTL(mtlFileName.data(), materialNames, texturePaths, color);
	if ( materialNames.size() > 0 )
	{
		numberOfParts = materialNames.size();
	}
	else
	{
		numberOfParts = 1;
	}

	vData = new Vertex * [numberOfParts];
	nVertex = new int[numberOfParts];
	iData = new GLuint * [numberOfParts];
	nIndex = new int[numberOfParts];

	FILE * pFile = fopen(filePath, "r");

	if ( !pFile )
	{
		printf("FILE %s NOT FOUND!\n", filePath);
		return;
	}

	for ( int i = 0; i < numberOfParts; i++ )
	{
		fscanf(pFile, "\nusemtl %*s");
		fscanf(pFile, "\nNrVertices: %d", &nVertex[i]);

		//SAFE_DEL_ARRAY(vData);
		vData[i] = new Vertex[nVertex[i]];
		char strFormat[] = "\n%*d. pos:[%f, %f, %f]; color:[%f, %f, %f, %f]; norm:[%f, %f, %f]; uv:[%f, %f];";
		for (int j = 0; j < nVertex[i]; j++)
		{
			fscanf(pFile, strFormat,
				&vData[i][j].position.x, &vData[i][j].position.y, &vData[i][j].position.z,
				&vData[i][j].color.x, &vData[i][j].color.y, &vData[i][j].color.z, &vData[i][j].color.w,
				&vData[i][j].normal.x, &vData[i][j].normal.y, &vData[i][j].normal.z,
				&vData[i][j].uv.x, &vData[i][j].uv.y);
		}

		fscanf(pFile, "\nNrIndices: %d", &nIndex[i]);

		iData[i] = new GLuint[nIndex[i]];
		for (int j = 0; j < nIndex[i]; j += 3)
		{
			fscanf(pFile, "\n%*d. %d, %d, %d", &iData[i][j + 0], &iData[i][j + 1], &iData[i][j + 2]);
		}
	}

	fclose(pFile);
}
//-------------------------------------
// Them thuoc tinh char *id;
char* Model::GetId(void)
{
      return this->id; 
}
void Model::SetId(const char *ID)
{
	this->id = new char[strlen(ID)+1];
	strcpy(this->id, ID);
}
void Model::Init(Sprite *sprite, int dx, int dy, int w, int h)
{
    Vertex *vertiesDataList = new Vertex[4];
    Vector3 pos[4]; // position of rect ABCD
	Vector2 uv[4];

	numberOfParts = 1;

	pos[0] = Vector3( dx * 1.0f     ,  dy * 1.0f     , 0.0f);
	pos[1] = Vector3((dx + w) * 1.0f,  dy * 1.0f     , 0.0f);
	pos[2] = Vector3((dx + w) * 1.0f, (dy + h) * 1.0f, 0.0f);
	pos[3] = Vector3( dx * 1.0f     , (dy + h) * 1.0f, 0.0f);

	float x = sprite->GetX() * 1.0f;
	float y = sprite->GetY() * 1.0f;
	float wSprite = sprite->GetWidth() * 1.0f;
	float hSprite = sprite->GetHeight() * 1.0f;
    
	char *idTexture = sprite->GetTextureId();
	Texture *textureSprite = ResourceManager::GetInstance()->GetTexture2D(idTexture);
	int wI = textureSprite->GetWidthImage();
	int hI = textureSprite->GetHeightImage();;

	uv[2]  = Vector2(  x / wI           , 1.0f - (y / hI));
	uv[1]  = Vector2( (x + wSprite) / wI, 1.0f - (y / hI));
	uv[0]  = Vector2( (x + wSprite) / wI, 1.0f - (y + hSprite) / hI);
	uv[3]  = Vector2(  x / wI           , 1.0f - (y + hSprite) / hI);

	for(int i = 0; i < 4; i++)
	{
		vertiesDataList[i].position = pos[i];
		vertiesDataList[i].uv = uv[i];
	}

	int indices[6] = {0,1,2,0,3,2};
    
	vboId = new GLuint[1];
	iboId = new GLuint[1];
    glGenBuffers(1, &vboId[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vertiesDataList, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboId[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)*6, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//----------------------------------------------
	SAFE_DEL_ARRAY(vertiesDataList);
}
