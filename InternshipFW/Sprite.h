
//-----------------
/*
        class chi chua thong tin Sprite
*/
#pragma once
#include "Defines.h"
#include "Texture.h"
class Sprite
{
private:
	GLint m_Id;
	GLint m_X,m_Y;

	GLint m_Width;
	GLint m_Height;
	char * m_textureId;

public:

	Sprite(GLint id = -1);
	~Sprite(void);
	void Init(void);

	void SetId(GLint id);
	void SetX(GLint x);
	void SetY(GLint y);
	void SetXY(GLint x, GLint y);
	void InitXY(GLint indexSprite,GLint wPrite,GLint  hPrite);

	void SetWidth(GLint width);
	void SetHeight(GLint height);
	void SetWidthHeight(GLint w, GLint h);

	void SetTextureId(const char * textureId);

	GLint GetId(void);
	GLint GetX(void);
	GLint GetY(void);
	GLint GetWidth(void);
	GLint GetHeight(void);
	char * GetTextureId(void);

};
