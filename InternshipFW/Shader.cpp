#include "Shader.h"
#include "Game.h"
Shader::Shader()
{
	states.clear();
	a_positionLoc = -1;
	a_colorLoc = -1;
	u_wvpMatrixLoc = -1;
	a_uvLoc = -1;

	nTexture2DLoc = 5;
	u_texture2DLoc = new GLint[nTexture2DLoc];
	for ( int i = 0; i < nTexture2DLoc; i++ )
	{
		u_texture2DLoc[i] = -1;
	}

	nTextureCubeLoc = 1;
	u_textureCubeLoc = new GLint[nTextureCubeLoc];
	for ( int i = 0; i < nTextureCubeLoc; i++ )
	{
		u_textureCubeLoc[i] = -1;
	}

	a_normalLoc = -1;
	u_wMatrixLoc = -1;
	u_camPosWLoc = -1;
	u_timeLoc = -1;
	
	u_ambColorLoc = -1;
	u_ambWeightLoc = -1;
	
	nLightLoc = 5;
	nLightProperties = 3;	//2 properties
	u_lightLoc = new GLint * [nLightLoc];
	for (int i = 0; i < nLightLoc; i++)
	{
		u_lightLoc[i] = new GLint[nLightProperties];
		for (int j = 0; j < nLightProperties; j++)
		{
			u_lightLoc[i][j] = -1;
		}
	}
	
	a_tangentLoc = -1;
	a_bitangentLoc = -1;
}
int Shader::Init(const char * vertexShaderPath, const char * fragmentShaderPath)
{
	vertexShader = LoadShader(GL_VERTEX_SHADER, vertexShaderPath);
	if ( vertexShader == 0 )
	{
		LOGI("\nLoading %s fail", vertexShaderPath);
		return -1;
	}
	fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

	if ( fragmentShader == 0 )
	{
		LOGI("\nLoading %s fail", fragmentShaderPath);
		glDeleteShader( vertexShader );
		return -2;
	}
	program = LoadProgram(vertexShader, fragmentShader);

	//finding location of uniforms / attributes
	a_positionLoc = glGetAttribLocation(program, "a_positionL");
	a_colorLoc = glGetAttribLocation(program, "a_color");
	u_wvpMatrixLoc = glGetUniformLocation(program, "u_wvpMatrix");
	a_uvLoc = glGetAttribLocation(program, "a_uv");
	for ( int i = 0; i < nTexture2DLoc; i++ )
	{
		char * indexName = NULL;
		char * texUniformName = NULL;
		char prefix[] = "u_texture";
		if ( i < 10 )
		{
			indexName = new char[2];
			texUniformName = new char[strlen(prefix) + 2];
		}
		else
		{
			indexName = new char[3];
			texUniformName = new char[strlen(prefix) + 3];
		}
		strcpy(texUniformName, prefix);
		sprintf(indexName, "%d", i + 1);
		u_texture2DLoc[i] = glGetUniformLocation(program, strcat(texUniformName, indexName));
		SAFE_DEL_ARRAY(indexName);
		SAFE_DEL_ARRAY(texUniformName);
	}

	for ( int i = 0; i < nTextureCubeLoc; i++ )
	{
		char * indexName = NULL;
		char * texUniformName = NULL;
		char prefix[] = "u_cubeMap";
		if ( i < 10 )
		{
			indexName = new char[2];
			texUniformName = new char[strlen(prefix) + 2];
		}
		else
		{
			indexName = new char[3];
			texUniformName = new char[strlen(prefix) + 3];
		}
		strcpy(texUniformName, prefix);
		sprintf(indexName, "%d", i + 1);
		u_textureCubeLoc[i] = glGetUniformLocation(program, strcat(texUniformName, indexName));
		SAFE_DEL_ARRAY(indexName);
		SAFE_DEL_ARRAY(texUniformName);
	}

	a_normalLoc = glGetAttribLocation(program, "a_normalL");
	u_wMatrixLoc = glGetUniformLocation(program, "u_wMatrix");
	u_camPosWLoc = glGetUniformLocation(program, "u_camPosW");
	u_timeLoc = glGetUniformLocation(program, "u_time");
	
	u_ambColorLoc = glGetUniformLocation(program, "u_ambient.color");
	u_ambWeightLoc = glGetUniformLocation(program, "u_ambient.weight");

	for (int i = 0; i < nLightLoc; i++)
	{
		char suffix[][20] = {"].position", "].type", "].color"};
		for (int j = 0; j < nLightProperties; j++)
		{
			char * indexName = NULL;
			char * texUniformName = NULL;
			char prefix[] = "u_light[";
			if ( i < 10 )
			{
				indexName = new char[2];
				texUniformName = new char[strlen(prefix) + strlen(suffix[j]) + 2];
			}
			else
			{
				indexName = new char[3];
				texUniformName = new char[strlen(prefix) + strlen(suffix[j]) + 3];
			}
			strcpy(texUniformName, prefix);
			sprintf(indexName, "%d", i);
			u_lightLoc[i][j] = glGetUniformLocation(program, strcat(strcat(texUniformName, indexName), suffix[j]));
			SAFE_DEL_ARRAY(indexName);
			SAFE_DEL_ARRAY(texUniformName);
		}
	}
	
	a_tangentLoc = glGetAttribLocation(program, "a_tangentL");
	a_bitangentLoc = glGetAttribLocation(program, "a_bitangentL");;
	
	return true;
}
void Shader::AddState(GLenum state)
{
	states.push_back( state );
}
void Shader::EnableState()
{
	for( GLuint i = 0; i < states.size(); i++ )
	{
		glEnable( states[i] );
		if ( states[i] == GL_BLEND )
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}
	}
}
void Shader::DisableState()
{
	for( GLuint i = 0; i < states.size(); i++ )
	{
		glDisable( states[i] );
	}
}
/// \brief Load a shader, check for compile errors, print error messages to output log
/// \param type Type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
/// \param Shaderrc Shader source string
/// \return A new shader object on success, 0 on failure
//
GLuint Shader::LoadShader ( GLenum type, const char * filename )
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader ( type );

	if ( shader == 0 )
	return 0;
	
	FILE * pf = fopen( filename, "rb" );
	if ( pf == 0 )
	{
		return 0;
	}
	fseek(pf, 0, SEEK_END);
	long size = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	char * buffer = new char[size + 1];
	fread(buffer, sizeof(char), size, pf);
	buffer[size] = 0;
	fclose(pf);

	glShaderSource ( shader, 1, (const char **) &buffer, NULL );
	delete [] buffer;
	buffer = NULL;
	// Compile the shader
	glCompileShader ( shader );

	// Check the compile status
	glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

	if ( !compiled ) 
	{
		GLint infoLen = 0;

		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char * infoLog = new char  [infoLen];


			glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
			LOGI ( "\nError compiling shader:\n%s\n", infoLog );            

			delete [] infoLog;
		}

		glDeleteShader ( shader );
		return 0;
	}
	return shader;
}


GLuint Shader::LoadProgram ( GLuint vertexShader, GLuint fragmentShader )
{
	GLuint programObject;
	GLint linked;

	// Create the program object
	programObject = glCreateProgram ( );

	if ( programObject == 0 )
		return 0;

	glAttachShader ( programObject, vertexShader );
	glAttachShader ( programObject, fragmentShader );

	// Link the program
	glLinkProgram ( programObject );

	// Check the link status
	glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

	if ( !linked ) 
	{
		GLint infoLen = 0;

		glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char * infoLog = new char[sizeof(char) * infoLen];

			glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
			LOGI ( "\nError linking program:\n%s\n", infoLog );            

			delete infoLog;
		}

		glDeleteProgram ( programObject );
		return 0;
	}

	return programObject;
}
Shader::~Shader()
{
	if ( program )
	{
		glDeleteProgram(program);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	SAFE_DEL_ARRAY(u_texture2DLoc);
	SAFE_DEL_ARRAY(u_textureCubeLoc);

	for (int i = 0; i < nLightLoc; i++)
	{
		SAFE_DEL_ARRAY(u_lightLoc[i]);
	}
	SAFE_DEL_ARRAY(u_lightLoc);

	SAFE_DEL(id);
}
//-------------------------------------
// Them thuoc tinh char *id;
char* Shader::GetId(void)
{
      return this->id; 
}
void Shader::SetId(const char *ID)
{

	this->id = new char[strlen(ID)+1];
	strcpy(this->id, ID);
}