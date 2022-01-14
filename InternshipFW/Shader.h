#pragma once
#include "Defines.h"

class Shader 
{
private:
	char *id;
public:
	GLuint program, vertexShader, fragmentShader;
	vector<GLenum> states;
	
	GLint a_positionLoc;
	GLint a_colorLoc;
	GLint u_wvpMatrixLoc;
	GLint a_uvLoc;
	GLint nTexture2DLoc;
	GLint nTextureCubeLoc;
	GLint * u_texture2DLoc;
	GLint * u_textureCubeLoc;
	GLint a_normalLoc;
	GLint u_wMatrixLoc;
	GLint u_camPosWLoc;
	GLint u_timeLoc;
	GLint u_ambColorLoc;
	GLint u_ambWeightLoc;
	GLint nLightLoc;
	GLint nLightProperties;
	GLint ** u_lightLoc;
	GLint a_tangentLoc;
	GLint a_bitangentLoc;

	//Method
	Shader();
	GLuint LoadProgram ( GLuint vertexShader, GLuint fragmentShader ) ;
	GLuint LoadShader ( GLenum type, const char * filename ) ;
	int Init(const char * vertexShaderPath, const char * fragmentShaderPath);
	virtual ~Shader();
	void AddState(GLenum state);
	void EnableState();
	void DisableState();

	 char *GetId(void);
	 void SetId(const char *ID);
};
