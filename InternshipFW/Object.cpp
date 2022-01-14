#include "Object.h"
#include "Shader.h"

Object::Object(void) 
       : id(0)
{
	this->position = Vector3(0.0, 0.0, 0.0);
	this->rotation = Vector3(0.0, 0.0, 0.0);
	this->scale = Vector3(1.0, 1.0, 1.0);
	this->worldMatrix = new Matrix();
	GetWorldMatrix();
}

Object::~Object(void)
{
	this->Destroy();
}

void Object::Init(Vector3 position, Vector3 rotation, Vector3 scale)
{
	this->scale = scale;
	this->rotation = rotation;
	this->position = position;
}


void Object::Destroy()
{
	SAFE_DEL(this->worldMatrix);
	SAFE_DEL(this->id);
}

void Object::SetPosition(Vector3 newPosition)
{
	this->position = newPosition;
}

void Object::SetRotation(Vector3 newRotation)
{
	this->rotation = newRotation;
}

void Object::SetScale(Vector3 newScale)
{
	this->scale = newScale;
}

Vector3 Object::GetPosition()
{
	return this->position;
}

Vector3 Object::GetRotation()
{
	return this->rotation;
}

Vector3 Object::GetScale()
{
	return this->scale;
}

Matrix * Object::GetWorldMatrix()
{
	Matrix scaleMatrix, rotationMatrix, translationMatrix;

	scaleMatrix.SetScale(this->scale);
	rotationMatrix.SetRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z);
	translationMatrix.SetTranslation(this->position);
	*this->worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;
	return this->worldMatrix;
}
//-------------------------------------
// Them thuoc tinh char *id;
char* Object::GetId(void)
{
      return this->id; 
}
void Object::SetId(const char *ID)
{
	if(this->id != NULL)
	{
		SAFE_DEL(id);
	}
	this->id = new char[strlen(ID)+1];
	strcpy(this->id, ID);
}