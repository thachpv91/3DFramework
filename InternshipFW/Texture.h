#pragma once
#include "Defines.h"


class Texture
{
private:
	char *id;

	int numberOfParts;
	GLuint * textureId;

	GLint widthImage;
	GLint heightImage;

public:
	Texture(void);
	~Texture(void);

	virtual int GetNumberOfParts();
	virtual GLuint * GetTextureId();

	virtual void Load2D(const char * fileName, int tiling);
	virtual void LoadCube(char ** fileName, int tiling);
	virtual void LoadCube(char * fileName, int tiling);

	virtual char *GetId(void);
	virtual void SetId(const char *ID);

	
	GLint GetWidthImage();
	GLint GetHeightImage();
};