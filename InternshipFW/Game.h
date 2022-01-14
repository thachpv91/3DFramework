#pragma once
#include "Defines.h"
#include "GameState.h"
#include "Timer.h"

enum PointerEvent
{
    POINTER_DOWN,
    POINTER_UP,
    POINTER_DRAGGED,
	POINTER_MAX
};

class Game
{
public:
	GameState* currentState;
	GameState* previousState;
	bool isRuning;
	GLint width, height;
	char * resPath;
public:
	static Game* GetInstance();
	static void FreeInstance();
	Game * Init(const char * resourcesPath, GLint width, GLint height );
	void Update();
	void ProcessState();
	bool IsRuning();
	void Destroy();
	void OnKey(unsigned char keyCode, bool keyPressed);
	void OnPointer(int e, int x, int y, int x2, int y2);
	void SetState(GameState * newState);
	GameState* GetState();
#ifdef _WIN32
	// EGL variables
	EGLDisplay			eglDisplay;
	EGLConfig			eglConfig;
	EGLSurface			eglSurface;
	EGLContext			eglContext;
	EGLNativeWindowType	eglWindow;
	// Windows variables
	HWND hWnd;
	HDC	 hDC;
	void CleanWin();
	bool TestEGLError(HWND hWnd, char* pszLocation);
	bool CreateWin(GLuint width, GLuint height);
#endif
protected:
	Game(void);
	virtual ~Game(void);
private:
	static Game* instance;
};
