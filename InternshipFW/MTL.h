#pragma once
#include "Defines.h"
#include "Vector4.h"

bool loadMTL(const char * fileName, vector<string> &materialNames, vector<string> &texturePaths, vector<Vector4> &color);