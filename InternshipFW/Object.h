#pragma once
#include "Vertex.h"
#include "Matrix.h"
#include "Timer.h"

class Object
{
protected:

    char *id;

	Vector3 scale;
	Vector3 rotation;
	Vector3 position;
	Matrix * worldMatrix;
public:
	Object(void);
	virtual ~Object(void);

	virtual void AbstractFunction() = 0;

	virtual void Init(Vector3 position, Vector3 rotation, Vector3 scale);
	virtual void Destroy();

	virtual void SetPosition(Vector3 newPosition);
	virtual void SetRotation(Vector3 newRotation);
	virtual void SetScale(Vector3 newScale);

	virtual Vector3 GetPosition();
	virtual Vector3 GetRotation();
	virtual Vector3 GetScale();

	virtual Matrix * GetWorldMatrix();
   // Them ID
	virtual char *GetId(void);
	virtual void SetId(const char *ID);
};
