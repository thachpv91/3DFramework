#pragma once
#include <cstdio>
#ifndef WIN32
#define _ANDROID
#endif

#ifdef _ANDROID

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <android/log.h>
#define  LOG_TAG    "GAME"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#else

#ifdef _WIN32
#include <GLES2\gl2.h>
#include <windows.h>
#include <EGL\egl.h>
#define LOGI(...) printf(__VA_ARGS__)
#define LOGE(...) printf(__VA_ARGS__)
#define USE_MEMDGB 1

#define WIDTH 1024
#define HEIGHT 768

#endif
#endif
//if use STL include here

#include <vector>
#include <string>
using namespace std;

#include "TGA.h"

#define SAFE_DEL(p) if (p) delete p; p = 0;
#define SAFE_DEL_ARRAY(p) if (p) delete []p; p = 0;

