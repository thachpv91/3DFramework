#pragma once
#include "Object.h"
#include "Sprite.h"
#include "Shader.h"
#include "Model.h"
#include "Texture.h"
#define NONE 0
#define ANIMATION 1

typedef struct StateStruct
{
	int width, height;
	int dx, dy;
	Sprite *sprite;
}State;
class Object2D : public Object
{

private: 
	GLuint index1;
	GLuint index3;
	GLuint posSprite;
private:  // is a object 2D

    GLint m_typeObject2D;
	GLint m_dx, m_dy;
	GLint m_height, m_width;

	GLint m_currentState;
	vector<State> m_state;
	GLint m_numberState;

	Sprite  **m_sprite;
	GLuint m_numberSprite;
	float m_speed;

private:  // comom 
    Shader  *m_shader;
	Model   *m_model;
	Texture *m_texture;
	
public:
	bool  isSelect;

	Object2D(void);
	virtual ~Object2D(void);

	virtual void AbstractFunction() {}

	virtual void Update();
    virtual void Render();
	virtual void Init();
    virtual void Destroy();

	 void SetNumberSprite(GLuint numberSprite);
	 void AddSprite(Sprite *sprite);
	 Sprite *GetSprite(int index);

	 void NextSprite();
	 void PreviousSprite();

	 void InitModel();
	 void InitModelByState(GLint index = 0);
     void InitModelAnimation(GLint index = 0);

	 void SetDxDy(GLint dx, GLint dy);
	 void SetWidthHeight(GLint width, GLint height);

     void SetDx(GLint dx);
     void SetDy(GLint dy);
     void SetWidth(GLint width);
     void SetHeight(GLint height);
	 void SetSpeed(float speed);

	 void  SetType(GLint type);
	 void  SetType(const char *type);
	 GLint GetType();

	 void ScreenToPos(GLint x, GLint y, float &xPos, float &yPos);
	 bool OnSelect(GLint x, GLint y);

	 void NextState();
	 void PreviousState();

	 void SetCurrentState(GLint currentState);

	 void Animation();
	 void AddState(Sprite *sprite, int x, int y, int w, int h);


	 void SetShader(Shader* _shader);
	 void SetTexture(Texture *texture);
	 void SetTexture(GLint idSprite);


};
