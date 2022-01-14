#pragma once
//Vector4
#include "Vector3.h"

class Matrix;

class Vector4
{
public:
	//Constructors
	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4(GLfloat _x, GLfloat _y, GLfloat _z) : x(_x), y(_y), z(_z), w(1.0f){}
	Vector4(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _w) : x(_x), y(_y), z(_z), w(_w) {}
	Vector4(GLfloat * pArg) : x(pArg[0]), y(pArg[1]), z(pArg[2]), w(pArg[3]) {}
	Vector4( const Vector3 & vector) : x(vector.x), y(vector.y), z(vector.z), w(1.0f) {}
	Vector4( const Vector3 & vector, GLfloat _w) : x(vector.x), y(vector.y), z(vector.z), w(_w) {}
	Vector4( const Vector4 & vector) : x(vector.x), y(vector.y), z(vector.z), w(vector.w) {}

	//Vector's operations
	GLfloat Length() const ;
	Vector4 & Normalize();
	Vector4 operator + ( const Vector4 & vector) const ;
	Vector4 & operator += ( const Vector4 & vector);
	Vector4 operator - () const ;
	Vector4 operator - ( const Vector4 & vector) const ;
	Vector4 & operator -= ( const Vector4 & vector);
	Vector4 operator * (GLfloat k) const ;
	Vector4 & operator *= (GLfloat k);
	Vector4 operator / (GLfloat k) const ;
	Vector4 & operator /= (GLfloat k);
	Vector4 & operator = (GLfloat* pArg);
	Vector4 & operator = ( const Vector4 & vector);
	bool operator == (const Vector4 & vector) const;
	Vector4 Modulate( const Vector4 & vector) const ;
	GLfloat Dot( const Vector4 & vector) const ;

	//matrix multiplication
	Vector4 operator * ( const  Matrix & m ) const ;

	//access to elements
	GLfloat operator [] (unsigned int idx) const ;

	//data members
	union
	{
		struct
		{
			GLfloat x;
			GLfloat y;
			GLfloat z;
			GLfloat w;
		};
		struct
		{
			GLfloat left;
			GLfloat top;
			GLfloat width;
			GLfloat height;
		};
		struct
		{
			GLfloat xyz[3];
		};
		struct
		{
			GLfloat xy[2];
		};
	};
};

