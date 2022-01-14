#include "Object2D.h"
#include "Game.h"
#include "ResourceManager.h"
int xyz = 0;

Object2D::Object2D(void)
{
	isSelect = false;
	 index1 = 0;
	 index3 = 0;
	 posSprite = 0;
	 m_shader = 0;
	 m_model = 0;
	 m_texture = 0;
	 m_sprite = 0;

	 m_typeObject2D = 0;
     m_dx = 0;
	 m_dy = 0;
     m_height = 0;
	 m_width  = 0;

	 m_currentState = 0;

	 m_numberSprite = 0;
	 m_speed = 0.0f;

	 m_numberState = 0;
}

Object2D::~Object2D(void)
{
}
void Object2D::Init()
{
		Vector3 pos  = Vector3(-1.0f, 1.0f, 0.0f);
		Vector3 rota = Vector3(0.0f, 0.0f, -3.14f/2.0f);
		int widthScreen = Game::GetInstance()->width;
		int   heightScreen = Game::GetInstance()->height;
        float scaleX = 2.0f / widthScreen;
		float scaleY = 2.0f / heightScreen;
		Vector3 scaleVec = Vector3( scaleX, scaleY, 1.0f);
		Object::SetPosition(pos);
		Object::SetRotation(rota);
		Object::SetScale(scaleVec);
		m_dx = m_state[m_currentState].dx;
		m_dy = m_state[m_currentState].dy;
		m_width  = m_state[m_currentState].width;
		m_height = m_state[m_currentState].height;

		InitModel();		
}
void Object2D::Render()
{
	glUseProgram(m_shader->program);
	glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindBuffer(GL_ARRAY_BUFFER,m_model->GetVboId()[0]);
	if(m_shader->a_positionLoc != -1 )
		{
			glEnableVertexAttribArray(m_shader->a_positionLoc);
			glVertexAttribPointer(m_shader->a_positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));
		}

		if(m_shader->a_uvLoc != -1 )
		{
			glEnableVertexAttribArray(m_shader->a_uvLoc);
			glVertexAttribPointer(m_shader->a_uvLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, uv));
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture->GetTextureId()[0]);
		if (m_shader->u_texture2DLoc[0] != -1 )
		{
				glUniform1i(m_shader->u_texture2DLoc[0], 0);
		}

		if (m_shader->u_wMatrixLoc != -1 )
		{
			GetWorldMatrix();
			glUniformMatrix4fv(m_shader->u_wMatrixLoc, 1, GL_FALSE, (float *) this->worldMatrix);
		}
		//glVertexAttribPointer: pass all properties of each vertex of model to OpenGL method's parameters

		//glDrawArrays(GL_TRIANGLES, 0, 3);	//Send Vertex from RAM to VRAM (or buffer if already bind it)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_model->GetIboId()[0]);
		glDrawElements(GL_TRIANGLES, 6 , GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisable(GL_BLEND);
}
void Object2D::Destroy()
{

}
void Object2D::Update()
{
		 if( GetType() == ANIMATION )
		 {
                  Animation();          	 
		 }
		 if( GetType() == NONE)
		 {
			     
		 }
}
void Object2D::SetNumberSprite(GLuint numberSprite)
{
	   index3 = 0;
       m_numberSprite = numberSprite;
	   m_sprite = new Sprite*[m_numberSprite];
}
void Object2D::AddSprite(Sprite *sprite)
{
	for(int i = 0; i < index3; i++)
	{
		if( m_sprite[i]->GetId() == sprite->GetId() )
		{
		     return;
		}
	   
	}

    if(index3 < m_numberSprite)
	{
		m_sprite[index3++] = sprite;
	}      
}
Sprite *Object2D::GetSprite(int index)
{
	if( (index < (int)m_numberSprite) && (index >= 0) )
	{
	         return m_sprite[index];
	}
	return NULL;
}
void Object2D::NextSprite()
{
      posSprite++;
	  if(posSprite >= m_numberSprite)
	  {
	    posSprite = 0;
	  }
	  LOGI("\n SPREITE: %d", posSprite);
	  m_model->Init(m_sprite[posSprite], m_dx, m_dy, m_width, m_height);
}
void Object2D::PreviousSprite()
{
      posSprite--;
	  if(posSprite < 0)
	  {
	    posSprite = m_numberSprite - 1;
	  }

	  m_model->Init(m_sprite[posSprite], m_dx, m_dy, m_width, m_height);
}
void Object2D::SetDxDy(GLint dx, GLint dy)
{
       SetDx(dx);
	   SetDy(dy);
}
void Object2D::SetWidthHeight(GLint width, GLint height)
{
       SetWidth(width);
	   SetHeight(height);
}
void Object2D::SetDx(GLint dx)
{
       m_dx = dx;
}
void Object2D::SetDy(GLint dy)
{
	   m_dy = dy;
}
void Object2D::SetWidth(GLint width)
{
        m_width = width; 
}
void Object2D::SetHeight(GLint height)
{
		m_height = height;
}
void Object2D::ScreenToPos(GLint x, GLint y, float &xPosOut, float &yPosOut)
{
    
	GLint screenWidth  = Game::GetInstance()->width;
	GLint screenHeight = Game::GetInstance()->height;

	xPosOut = (float)x / ((float)screenWidth/2.0f) -1.0f;
    yPosOut = 1.0f - (float)y/((float)(screenHeight)/2.0f);
}
bool Object2D::OnSelect(GLint x, GLint y)
{
         float xPos = 0, yPos = 0;
		 ScreenToPos(x, y, xPos, yPos);
          
		 if( ( x > m_dx ) && (x < (m_dx + m_width)) &&
			 ( y > m_dy ) && (y < (m_dy + m_height)))
		 {
		       isSelect = true;
		 }
		 else
		 {
			 isSelect = false;
		 }
		 return isSelect;
}
 void  Object2D::SetType(GLint type)
 {
        m_typeObject2D = type;
 }
 void  Object2D::SetType(const char *type)
 {
		if(strcmp(type, "NONE") == 0)
		{
		     SetType(NONE);
		}else
		{
			SetType(ANIMATION);
		}
 }
GLint Object2D::GetType()
{
		return m_typeObject2D;
}
void Object2D::NextState()
{
	m_currentState++;
	if(m_currentState >= m_numberState)
	{
		m_currentState = 0;
	}
	InitModelByState(m_currentState);
}
void Object2D::PreviousState()
{
	m_currentState--;
	if(m_currentState < 0)
	{
		m_currentState = m_numberState - 1;
	}
	InitModelByState(m_currentState);
}
void Object2D::SetCurrentState(GLint currentState)
{
	m_currentState = currentState;
	InitModelByState(m_currentState);
}
void Object2D::SetSpeed(float speed)
{
     m_speed = speed;
}

void Object2D::Animation()
{
        //(sprite/ secon)
	float gameTime  = Timer::GameTime();
	int   tempGameTime = (int)(gameTime * 10);
	int tempSpeed = (int)(m_speed * 10);
	
	//LOGI("\n Gametime % speed: %d % %d = %d", tempGameTime, tempSpeed, (tempGameTime % tempSpeed));
	if( (tempGameTime % tempSpeed)  == 0)
	{
		//LOGI("\n ----------> Doi Sprite");
		//NextSprite();
	}

	xyz++;
	if(xyz % 30 == 0)
	{
	  NextSprite();
	}
}
 void Object2D::AddState(Sprite *sprite, int x, int y, int w, int h)
 {
            State state;
			state.sprite = sprite; 
			state.dx = x;    state.dy = y;
			state.width = w; state.height = h;

			m_state.push_back(state);
			m_numberState = m_state.size() - 1;
 }
 void Object2D::InitModelAnimation(GLint indexSprite)
{
     m_model = 0;
	 m_model = new Model();
	 
	 if(indexSprite >= 0 && indexSprite < (int)m_numberSprite)
	 {
		 Sprite *sprite = GetSprite(indexSprite);
		 m_model->Init(sprite, m_dx, m_dy, m_width, m_height);
	 }
}
 void Object2D::InitModelByState(GLint indexState)
{
     m_model = 0;
	 m_model = new Model();
	 
	 if(indexState >= 0 && indexState < (int)m_numberState)
	 {
		 State state = m_state[indexState];
		 Sprite *sprite = state.sprite;
		 m_model->Init(sprite, state.dx , state.dy, state.width, state.height);
	 }
}
void Object2D::InitModel()
{
     if(GetType() == NONE)
	 {
	      InitModelByState();
	 }else

	 if(GetType() == ANIMATION)
	 {
	      InitModelAnimation();
	 }
    
}
void Object2D::SetShader(Shader *_shader)
{
	 this->m_shader = _shader;
}
void Object2D::SetTexture(Texture *texture)
{
		m_texture = texture;
}
void Object2D::SetTexture(GLint idSprite)
{
	Sprite *sptTemp = ResourceManager::GetInstance()->GetSprite(idSprite);
		char *idTexSprite;
		idTexSprite = 	sptTemp->GetTextureId();
		this->m_texture = ResourceManager::GetInstance()->GetTexture2D(idTexSprite);
}