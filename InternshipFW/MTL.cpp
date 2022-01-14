#include "MTL.h"
#include "Game.h"


bool loadMTL(const char * fileName, vector<string> &materialNames, vector<string> &texturePaths, vector<Vector4> &color)
{
	FILE * pFile;

	pFile = fopen(fileName, "r");

	if ( !pFile )
	{
		printf("FILE %s NOT FOUND!\n", fileName);
		return false;
	}

	char line[32];
	while (!feof(pFile))
	{
		if ( strcmp(line, "newmtl") != 0 )
		{
			fscanf(pFile, "\n%s", line);
		}
		else
		{
			string tmpStr("");
			Vector4 ambColor, diffuseColor, specularColor;
			float diffWeight = 0.8f;
			char mtlName[256];
			fscanf(pFile, "%s", mtlName);
			while ( !feof(pFile) )
			{
				fscanf(pFile, "\n%s", line);
				if ( strcmp(line, "map_Kd") == 0 )
				{
					char texturePath[256];
					fscanf(pFile, "\n%s", texturePath);

					tmpStr.assign(fileName);
					int pos = tmpStr.find_last_of('/');
					tmpStr.replace(pos + 1, strlen(texturePath), texturePath);
				}
				else if ( strcmp(line, "Ka") == 0 )
				{
					fscanf(pFile, "\n%f %f %f", &ambColor.x, &ambColor.y, &ambColor.z);
					ambColor.w = 1.0f;
				}
				else if ( strcmp(line, "Kd") == 0 )
				{
					fscanf(pFile, "\n%f %f %f", &diffuseColor.x, &diffuseColor.y, &diffuseColor.z);
					diffuseColor.w = 1.0f;
				}
				else if ( strcmp(line, "Ks") == 0 )
				{
					fscanf(pFile, "\n%f %f %f", &specularColor.x, &specularColor.y, &specularColor.z);
					specularColor.w = 1.0f;
				}
				else if ( strcmp(line, "d") == 0 )
				{
					fscanf(pFile, "\n%f", &diffWeight);
				}
				else if ( strcmp(line, "newmtl") == 0 )
				{
					break;
				}
			}
			
			if ( tmpStr.compare("") != 0 )
			{
				ambColor = Vector4(0.0, 0.0, 0.0, 0.0);
				diffuseColor = Vector4(0.0, 0.0, 0.0, 0.0);
				specularColor = Vector4(0.0, 0.0, 0.0, 0.0);
			}
			else
			{
				tmpStr.assign(Game::GetInstance()->resPath);
				tmpStr.append("Textures/black.tga");
			}

			Vector4 tmpColor = ambColor * (1 - diffWeight) + diffuseColor * diffWeight;
			color.push_back(tmpColor);

			texturePaths.push_back(tmpStr);

			tmpStr.assign(mtlName);
			materialNames.push_back(tmpStr);
		}
	}

	fclose(pFile);
	return true;
}