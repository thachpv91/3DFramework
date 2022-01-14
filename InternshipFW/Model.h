#pragma once
#include "Defines.h"
#include "Vertex.h"
#include "Sprite.h"


class Model
{
private:
    char  *id;

	int numberOfParts;
	GLuint * vboId;
	GLuint * iboId;
	GLuint ** indicesData;
	int * nIndex;

	virtual void LoadNFG(const char * filePath, Vertex ** &vData, int * &nVertex, GLuint ** &iData, int * &nIndex);
	virtual void LoadOBJ(const char * filePath, Vertex ** &vData, int * &nVertex, GLuint ** &iData, int * &nIndex);
	virtual void LoadMDL(const char * filePath, Vertex ** &vData, int * &nVertex, GLuint ** &iData, int * &nIndex);

public:
	Model(void);
	~Model(void);

	virtual int GetNumberOfParts();
	virtual GLuint * GetVboId();
	virtual GLuint * GetIboId();
	virtual int * GetIndexSize();
	virtual void Load(const char * filePath);

	virtual char *GetId(void);
	virtual void SetId(const char *ID);

    void Init(Sprite *sprite, int dx, int dy, int w, int h);
};