#include "Sprite.h"
#include "ResourceManager.h"
Sprite::Sprite(GLint id)
       : m_Id(id)
{
	m_textureId = 0;
}

Sprite::~Sprite(void)
{
	SAFE_DEL(m_textureId);
}
void Sprite::SetId(GLint id)
{
       m_Id = id;
}
void Sprite::SetX(GLint x)
{
      m_X = x;
}
void Sprite::SetY(GLint y)
{
	 m_Y = y;
}
void Sprite::SetWidth(GLint width)
{
	m_Width = width;
}
void Sprite::SetHeight(GLint height)
{
	m_Height = height;
}
void Sprite::SetTextureId(const char * textureId)
{
	if(this->m_textureId != NULL)
	{
		SAFE_DEL(m_textureId);
	}
	m_textureId = new char[ strlen(textureId) + 1];
	strcpy(m_textureId, textureId);
}
void Sprite::SetXY(GLint x, GLint y)
{
	SetX(x);
	SetY(y);
}
void Sprite::InitXY(GLint index,GLint wSPrite0,GLint hSPrite0)
{
	
	m_Width  = wSPrite0;
	m_Height = hSPrite0;

	Texture *texSprite = ResourceManager::GetInstance()->GetTexture2D(m_textureId);

	int wI = texSprite->GetWidthImage();
	int hI = texSprite->GetHeightImage();

	int col = hI / hSPrite0; 
	int row = wI / wSPrite0;
    
	int tempIndex = -1;
	bool loop = true;
	for(int i = 0; i < row; i++)
	{
	     for(int j = 0; j < col; j++)
		 {
			   tempIndex = i * row + j;
			   if( tempIndex == index)
			   {
					m_X = wSPrite0 * j;
					m_Y = hSPrite0 * i;

					loop = false;
					break;
			   }
		 }
		 if( !loop )  break;

	}
	LOGI("\n[%d, %d] index: %d Sprite ID: %d (%d, %d) (%d %d)", row, col, index, m_Id, m_X, m_Y, m_Width, m_Height);
}
void Sprite::SetWidthHeight(GLint w, GLint h)
{
	SetWidth(w);
	SetHeight(h);
}

GLint Sprite::GetId(void)
{
	return m_Id;
}
GLint Sprite::GetX(void)
{
	return m_X;
}
GLint Sprite::GetY(void)
{
	return m_Y;
}
GLint Sprite::GetWidth(void)
{
	return m_Width;
}
GLint Sprite::GetHeight(void)
{
	return m_Height;
}
char * Sprite::GetTextureId(void)
{
	return m_textureId;
}
