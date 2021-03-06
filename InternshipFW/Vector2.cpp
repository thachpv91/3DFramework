
#include "Matrix.h"
#include <math.h>

//Vector2

GLfloat Vector2::Length() const 
{
	return sqrt(x*x + y*y);
}

Vector2 & Vector2::Normalize()
{
	if ( Length() == 0.0f )
	{
		return *this;
	}
	GLfloat lenInv = 1.0f / Length();
	x *= lenInv;
	y *= lenInv;

	return *this;
}
Vector2 & Vector2::operator = (GLfloat* pArg)
{
	x = pArg[0];
	y = pArg[1];
	return *this;
}

Vector2 Vector2::operator + ( const Vector2 & vector) const 
{
	return Vector2(x + vector.x, y + vector.y);
}

Vector2 & Vector2::operator += ( const Vector2 & vector)
{
	x += vector.x;
	y += vector.y;

	return *this;
}

Vector2 Vector2::operator - () const 
{
	return Vector2(-x, -y);
}

Vector2 Vector2::operator - ( const Vector2 & vector) const 
{
	return Vector2(x - vector.x, y - vector.y);
}

Vector2 & Vector2::operator -= ( const Vector2 & vector)
{
	x -= vector.x;
	y -= vector.y;

	return *this;
}

Vector2 Vector2::operator * (GLfloat k) const 
{
	return Vector2(x * k, y * k);
}

Vector2 & Vector2::operator *= (GLfloat k)
{
	x *= k;
	y *= k;

	return *this;
}

Vector2 Vector2::operator / (GLfloat k) const 
{
	GLfloat kInv = 1.0f / k;
	return Vector2(x * kInv, y * kInv);
}

Vector2 & Vector2::operator /= (GLfloat k)
{
	return operator *= (1.0f / k);
}

Vector2 & Vector2::operator = ( const Vector2 & vector)
{
	x = vector.x;
	y = vector.y;

	return *this;
}

GLfloat Vector2::operator [] (unsigned int idx) const 
{
	return (&x)[idx];
}

Vector2 Vector2::Modulate( const Vector2 & vector) const 
{
	return Vector2(x * vector.x, y * vector.y);
}

GLfloat Vector2::Dot( const Vector2 & vector) const 
{
	return x * vector.x + y * vector.y;
}
