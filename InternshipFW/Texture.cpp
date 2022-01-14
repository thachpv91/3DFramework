#include "Texture.h"
#include "MTL.h"
#include "PNG.h"


Texture::Texture(void)
{
	numberOfParts = 0;
	this->textureId = NULL;
}

Texture::~Texture(void)
{
	for (int i = 0;  i < numberOfParts; i++)
	{
		if ( this->textureId[i] != 0 )
		{
			glDeleteBuffers(1, &textureId[i]);
			textureId[i] = 0;
		}
	}
	SAFE_DEL_ARRAY(textureId);
	SAFE_DEL_ARRAY(id);
}

int Texture::GetNumberOfParts()
{
	return numberOfParts;
}

GLuint * Texture::GetTextureId()
{
	return this->textureId;
}

void Texture::Load2D(const char * fileName, int tiling)
{
	vector<string> texturePaths;

	if ( strstr(fileName, "tga") || strstr(fileName, "png") )
	{
		numberOfParts = 1;
		string tmpStr;
		tmpStr.assign(fileName);
		texturePaths.push_back(tmpStr);
	}
	else if ( strstr(fileName, "mtl") )
	{
		vector<string> materialNames;
		vector<Vector4> color;
		loadMTL(fileName, materialNames, texturePaths, color);
		numberOfParts = materialNames.size();
	}

	this->textureId = new GLuint[numberOfParts];

	for (int i = 0; i < numberOfParts; i++)
	{
		glGenTextures(1, &this->textureId[i]);
		glBindTexture(GL_TEXTURE_2D, this->textureId[i]);

		if ( texturePaths[i].find(".tga") != string::npos )
		{
			int bpp = 0;
			unsigned char * imageData = LoadTGA(texturePaths[i].data(), &widthImage, &heightImage, &bpp);
			int colorType = 0;
			if ( bpp == 24 )
			{
				colorType = GL_RGB;
			}
			else
			{
				colorType = GL_RGBA;
			}
			glTexImage2D(GL_TEXTURE_2D, 0, colorType, widthImage, heightImage, 0, colorType, GL_UNSIGNED_BYTE, imageData);
			SAFE_DEL_ARRAY(imageData);
		}
		else if ( texturePaths[i].find(".png") != string::npos )
		{
			int width = 0, height = 0;
			vector<unsigned char> imageBuffer = loadPNG(texturePaths[i].data(), widthImage, heightImage);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, &imageBuffer[0]);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tiling);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tiling);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Texture::LoadCube(char ** fileName, int tiling)
{
	numberOfParts = 1;
	this->textureId = new GLuint[numberOfParts];

	glGenTextures(1, &this->textureId[0]);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureId[0]);

	for (int i = 0; i < 6; i++)
	{
		if ( strstr(fileName[i], ".tga") )
		{
			int  bpp = 0;
			unsigned char * imageData = LoadTGA(fileName[i], &widthImage, &heightImage, &bpp);
			int colorType = 0;
			if ( bpp == 24 )
			{
				colorType = GL_RGB;
			}
			else
			{
				colorType = GL_RGBA;
			}
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, colorType, widthImage, heightImage, 0, colorType, GL_UNSIGNED_BYTE, imageData);
			SAFE_DEL_ARRAY(imageData);
		}
		else if ( strstr(fileName[i], ".png") )
		{
			vector<unsigned char> imageBuffer = loadPNG(fileName[i], widthImage, heightImage);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, &imageBuffer[0]);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, tiling);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, tiling);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Texture::LoadCube(char * fileName, int tiling)
{
	numberOfParts = 1;
	this->textureId = new GLuint[numberOfParts];

	glGenTextures(1, &this->textureId[0]);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureId[0]);

	int offset[][2] = {{1, 2}, {1, 0}, {2, 1}, {0, 1}, {1, 1}, {1, 3}};
	int  bpp = 0;
	unsigned char * imageData = NULL;
	if ( strstr(fileName, ".tga") )
	{
		imageData = LoadTGA(fileName, &widthImage, &heightImage, &bpp);
	}
	else if ( strstr(fileName, ".png") )
	{
		vector<unsigned char> imageBuffer = loadPNG(fileName, widthImage, heightImage);
		imageData = new unsigned char[imageBuffer.size()];
		memcpy(imageData, &imageBuffer[0], imageBuffer.size());
		bpp = 32;
	}

	for (int i = 0; i < 6; i++)
	{
		int wUnit = widthImage / 4 * (bpp / 8);
		int hUnit = heightImage / 3;
		unsigned char * imageUnit = new unsigned char[wUnit * hUnit];
		int count = 0;

		for (int h = hUnit * (offset[i][0] + 1) - 1; h > hUnit * offset[i][0] - 1; h--)
		{
			for (int w = wUnit * offset[i][1]; w < wUnit * (offset[i][1] + 1); w++)
			{
				imageUnit[count++] = imageData[(wUnit * 4) * h + w];
			}
		}

		int colorType = 0;
		if ( bpp == 24 )
		{
			colorType = GL_RGB;
		}
		else
		{
			colorType = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, colorType, wUnit / (bpp / 8), hUnit, 0, colorType, GL_UNSIGNED_BYTE, imageUnit);
		SAFE_DEL_ARRAY(imageUnit);
	}
	SAFE_DEL_ARRAY(imageData);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, tiling);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, tiling);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
//-------------------------------------
// Them thuoc tinh char *id;
char* Texture::GetId(void)
{
      return this->id; 
}
void Texture::SetId(const char *ID)
{
	this->id = new char[strlen(ID)+1];
	strcpy(this->id, ID);
}
GLint Texture::GetWidthImage()
{
       return widthImage;
}
GLint Texture::GetHeightImage()
{
	   return heightImage;
}