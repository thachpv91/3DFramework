#include "GameObject.h"
#include "Vertex.h"
#include "Matrix.h"
#include "Timer.h"
#include "SceneManager.h"


GameObject::GameObject(void) : Object()
{
	shader = NULL;
	model = NULL;
	nTexture2D = -1;
	nTextureCube = -1;
	texture2D = NULL;
	textureCube = NULL;
	nLight = -1;
	lightId = NULL;
}

GameObject::~GameObject(void)
{
	this->Destroy();
}

void GameObject::Render(Camera * &camera)
{
	glUseProgram(shader->program);
    GetShader()->EnableState();
	for (int i = 0; i < model->GetNumberOfParts(); i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, model->GetVboId()[i]);

		if( shader->a_positionLoc != -1 )
		{
			glEnableVertexAttribArray(shader->a_positionLoc);
			glVertexAttribPointer(shader->a_positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));
		}

		if( shader->a_colorLoc != -1 )
		{
			glEnableVertexAttribArray(shader->a_colorLoc);
			glVertexAttribPointer(shader->a_colorLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, color));
		}

		if ( shader->u_wvpMatrixLoc != -1 )
		{
			Matrix wvpMatrix = (*this->worldMatrix) * (* camera->GetViewMatrix()) * (* camera->GetProjectionMatrix());
			glUniformMatrix4fv(shader->u_wvpMatrixLoc, 1, GL_FALSE, (float *) &wvpMatrix);
		}

		if( shader->a_uvLoc != -1 )
		{
			glEnableVertexAttribArray(shader->a_uvLoc);
			glVertexAttribPointer(shader->a_uvLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, uv));
		}

		for ( int textureUnit = 0; textureUnit < nTexture2D; textureUnit++ )
		{
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, texture2D[textureUnit]->GetTextureId()[i]);

			if ( shader->u_texture2DLoc[textureUnit] != -1 )
			{
				glUniform1i(shader->u_texture2DLoc[textureUnit], textureUnit);
			}
		}

		for ( int textureUnit = 0; textureUnit < nTextureCube; textureUnit++ )
		{
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureCube[textureUnit]->GetTextureId()[0]);

			if ( shader->u_textureCubeLoc[textureUnit] != -1 )
			{
				glUniform1i(shader->u_textureCubeLoc[textureUnit], textureUnit);
			}
		}

		if( shader->a_normalLoc != -1 )
		{
			glEnableVertexAttribArray(shader->a_normalLoc);
			glVertexAttribPointer(shader->a_normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));
		}

		if ( shader->u_wMatrixLoc != -1 )
		{
			glUniformMatrix4fv(shader->u_wMatrixLoc, 1, GL_FALSE, (float *) this->worldMatrix);
		}

		if (shader->u_camPosWLoc != -1)
		{
			Vector3 cameraPosW = camera->GetPosition();
			glUniform3fv(shader->u_camPosWLoc, 1, (float *) &cameraPosW);
		}

		if (shader->u_timeLoc != -1)
		{
			glUniform1f(shader->u_timeLoc, Timer::GameTime());
		}
		if (shader->a_tangentLoc != -1)
		{
			glEnableVertexAttribArray(shader->a_tangentLoc);
			glVertexAttribPointer(shader->a_tangentLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, tangent));
		}

		if (shader->a_bitangentLoc != -1)
		{
			glEnableVertexAttribArray(shader->a_bitangentLoc);
			glVertexAttribPointer(shader->a_bitangentLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, binormal));
		}

		//glVertexAttribPointer: pass all properties of each vertex of model to OpenGL method's parameters

		//glDrawArrays(GL_TRIANGLES, 0, 3);	//Send Vertex from RAM to VRAM (or buffer if already bind it)

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->GetIboId()[i]);
		glDrawElements(GL_TRIANGLES, model->GetIndexSize()[i], GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	GetShader()->DisableState();
}

void GameObject::Destroy()
{
	Object::Destroy();

	SAFE_DEL_ARRAY(texture2D);
	SAFE_DEL_ARRAY(textureCube);
	SAFE_DEL_ARRAY(lightId);
}
/*
int GameObject::GetNumberOfLightId()
{
	return this->nLight;
}
*/
Shader * GameObject::GetShader()
{
	return shader;
}
/*
int GameObject::GetLightIdElementAt(int index)
{
	return lightId[index];
}
*/
void GameObject::SetShader(Shader * shader)
{
	this->shader = shader;
}

void GameObject::SetModel(Model * newModel)
{
	//SAFE_DEL(model);
	model = newModel;
}

void GameObject::SetTexture2D(Texture ** newTexture2D, int nTexture2D)
{
	SAFE_DEL_ARRAY(this->texture2D);

	this->nTexture2D = nTexture2D;
	this->texture2D = new Texture * [nTexture2D];
	for ( int i = 0; i < nTexture2D; i++ )
	{
		this->texture2D[i] = newTexture2D[i];
	}
}

void GameObject::SetTextureCube(Texture ** newTextureCube, int nTextureCube)
{
	SAFE_DEL_ARRAY(this->textureCube);

	this->nTextureCube = nTextureCube;
	this->textureCube = new Texture * [nTextureCube];
	for ( int i = 0; i < nTextureCube; i++ )
	{
		this->textureCube[i] = newTextureCube[i];
	}
}

void GameObject::Update(Camera* camera)
{
	//LOGI("\nFrame time: %f (s)", Timer::FrameTime() );
	camera->Update();
	GetWorldMatrix();
}
/*
void GameObject::SetLightId(int * newLightId, int nLight)
{
	SAFE_DEL_ARRAY(this->lightId);
	this->nLight = nLight;
	this->lightId = new int[nLight];
	for (int i = 0; i < nLight; i++)
	{
		this->lightId[i] = newLightId[i];
	}
}
*/