#include "Defines.h"
#include "Game.h"
#include "Timer.h"
#include "InputManager.h"


bool Game::IsRuning()
{
	return isRuning;
}
void Game::ProcessState()
{
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	if (currentState != NULL )
	{
		currentState->Update();
		currentState->Render();
	}
	
	SAFE_DEL(previousState);
}
void Game::SetState(GameState * newState)
{
	previousState = currentState;
	currentState = newState;
	newState->Init(resPath);
}
GameState* Game::GetState()
{
	return currentState;
}
void Game::OnKey(unsigned char keyCode, bool keyPressed)
{
	if ( keyPressed )
	{
		InputManager::GetInstance()->SetKeyState(keyCode);
		LOGI("\nKey Pressed: %d", keyCode);
	}
	else
	{
		InputManager::GetInstance()->ClearKeyState(keyCode);
		LOGI("\nKey Released: %d", keyCode);
	}
}

void Game::OnPointer(int e, int x, int y, int x2, int y2)
{
	if ( e == POINTER_DOWN )
	{
		InputManager::GetInstance()->SetTouchCurrentPosition(0, (float) x, (float) y);
		LOGI("\nPOINTER_DOWN: (%d, %d)", x, y);
		InputManager::GetInstance()->SetTouchCurrentPosition(1, (float) x2, (float) y2);
		LOGI("\nPOINTER_DOWN: (%d, %d)", x2, y2);
	}	
	if ( e == POINTER_UP)
	{
		InputManager::GetInstance()->ClearTouchPosition(0);
		LOGI("\nPOINTER_UP: (%d, %d)", x, y);
		InputManager::GetInstance()->ClearTouchPosition(1);
		LOGI("\nPOINTER_DOWN: (%d, %d)", x2, y2);
	}	
	if ( e == POINTER_DRAGGED )
	{
		InputManager::GetInstance()->SetTouchDifferentPosition(0, (float) x, (float) y);
		LOGI("\nPOINTER_DRAGGED: (%d, %d)", x, y);
		InputManager::GetInstance()->SetTouchDifferentPosition(1, (float) x2, (float) y2);
		LOGI("\nPOINTER_DRAGGED: (%d, %d)", x2, y2);
	}	
}

Game::~Game(void)
{
	SAFE_DEL_ARRAY(resPath);
	SAFE_DEL(currentState);
	SAFE_DEL(previousState);
	isRuning = false;
}




//DON'T CARE

Game::Game(void)
{
	this->width = 0;
	this->height = 0;
	isRuning = false;
	currentState = NULL;
	previousState = NULL;
}
Game * Game::Init( const char * resourcesPath, GLint width, GLint height )
{
	resPath = new char[strlen(resourcesPath) + 1];
	strcpy(resPath, resourcesPath);
	this->width = width;
	this->height = height;
#ifdef _WIN32
	CreateWin( width, height );	
#endif
	isRuning = true;
	return this;
}

void Game::Update()
{
	Timer::Tick();
#ifdef _WIN32
	MSG msg = {0};

	if  (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
	{
		if (msg.message == WM_QUIT)
		{
			isRuning = false; 
		}
		else
		{
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}
	}
#endif		
	ProcessState();
#ifdef _WIN32
	eglSwapBuffers(eglDisplay, eglSurface);				
#endif
}
#ifdef _WIN32
//Process Input
LRESULT CALLBACK WndProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT  lRet = 1; 
	int x = LOWORD(lParam); 
	int y = HIWORD(lParam);
	static bool mouseDown = false;

	switch (Msg) 
	{ 
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(WM_QUIT);
		break; 

	case WM_KEYDOWN:
		Game::GetInstance()->OnKey((unsigned char) wParam, true);
		break;
	case WM_KEYUP:
		Game::GetInstance()->OnKey((unsigned char) wParam, false);
		break;
	case WM_LBUTTONDOWN:		
	case WM_RBUTTONDOWN:		
		mouseDown = true;		
		Game::GetInstance()->OnPointer(POINTER_DOWN, x, y, -1, -1);
		break;

	case WM_LBUTTONUP:	
	case WM_RBUTTONUP:
		mouseDown = false;
		Game::GetInstance()->OnPointer(POINTER_UP, x, y, -1, -1);
		break;
	case WM_MOUSEMOVE:		
		if (mouseDown)
		{			
			Game::GetInstance()->OnPointer(POINTER_DRAGGED, x, y, -1, -1);
		}		
		break;
	default: 
		lRet = DefWindowProc (hWnd, Msg, wParam, lParam); 
		break; 
	} 

	return lRet; 
}
void Game::CleanWin()
{
	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

	eglTerminate(eglDisplay);

	if (hDC) 
		ReleaseDC(hWnd, hDC);

	if (hWnd) 
		DestroyWindow(hWnd);
}


bool Game::CreateWin(GLuint width, GLuint height)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	// Register the windows class
	WNDCLASS sWC;
	sWC.style = CS_HREDRAW | CS_VREDRAW;
	sWC.lpfnWndProc = WndProcedure; //Procedute Callback Event
	sWC.cbClsExtra = 0;
	sWC.cbWndExtra = 0;
	sWC.hInstance = hInstance;
	sWC.hIcon = 0;
	sWC.hCursor = 0;
	sWC.lpszMenuName = 0;
	sWC.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	sWC.lpszClassName = "OpenGLES2.0";


	ATOM registerClass = RegisterClass(&sWC);
	if (!registerClass)
	{
		MessageBox(0, ("Failed to register the window class"), ("Error"), MB_OK | MB_ICONEXCLAMATION);
	}
	// Create the eglWindow
	RECT	sRect;
	SetRect(&sRect, 0, 0, width, height);
	AdjustWindowRectEx(&sRect, WS_CAPTION | WS_SYSMENU, false, 0);
	hWnd = CreateWindow( "OpenGLES2.0", "Nguyen Van Phat", WS_VISIBLE | WS_SYSMENU,
		0, 0, sRect.right - sRect.left, sRect.bottom - sRect.top, NULL, NULL, hInstance, NULL);
	eglWindow = hWnd;

	// Get the associated device context
	hDC = GetDC(hWnd);
	if (!hDC)
	{
		MessageBox(0, "Failed to create the device context", "Error", MB_OK|MB_ICONEXCLAMATION);
		CleanWin();
		return false;
	}

	eglDisplay = eglGetDisplay(hDC);

	if(eglDisplay == EGL_NO_DISPLAY)
		eglDisplay = eglGetDisplay((EGLNativeDisplayType) EGL_DEFAULT_DISPLAY);

	EGLint iMajorVersion, iMinorVersion;
	if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion))
	{
		MessageBox(0, ("eglInitialize() failed."), ("Error"), MB_OK|MB_ICONEXCLAMATION);
		CleanWin();
		return false;
	}

	const EGLint pi32ConfigAttribs[] =
	{
		EGL_LEVEL,				0,
		EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
		EGL_NATIVE_RENDERABLE,	EGL_FALSE,		
		EGL_DEPTH_SIZE,			8,
		EGL_STENCIL_SIZE, 8,
		EGL_SAMPLE_BUFFERS, 1,
		EGL_NONE
	};


	int iConfigs;
	if (!eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs) || (iConfigs != 1))
	{
		MessageBox(0, ("eglChooseConfig() failed."), ("Error"), MB_OK|MB_ICONEXCLAMATION);
		CleanWin();
		return false;
	}


	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);

	if(eglSurface == EGL_NO_SURFACE)
	{
		eglGetError(); // Clear error
		eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, NULL, NULL);
	}

	if (!TestEGLError(hWnd, "eglCreateWindowSurface"))
	{
		CleanWin();
		return false;
	}


	// Bind the API (It could be OpenGLES or OpenVG)
	eglBindAPI(EGL_OPENGL_ES_API);
	EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, ai32ContextAttribs);

	if (!TestEGLError(hWnd, "eglCreateContext"))
	{
		CleanWin();
		return false;
	}


	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);

	if (!TestEGLError(hWnd, "eglMakeCurrent"))
	{
		CleanWin();
		return false;
	}

	return true;
}
bool Game::TestEGLError(HWND hWnd, char* pszLocation)
{	
	EGLint iErr = eglGetError();
	if (iErr != EGL_SUCCESS)
	{
		TCHAR pszStr[256];
		sprintf_s(pszStr, ("%s failed (%d).\n"), pszLocation, iErr);
		MessageBox(hWnd, pszStr, ("Error"), MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}
#endif


//Singleton pattern
Game* Game::instance = NULL;
Game* Game::GetInstance()
{
	if ( instance == NULL )
	{
		instance = new Game();
	}
	return instance;
}

void Game::FreeInstance()
{
	if ( instance != NULL )
	{
		delete instance;
		instance = NULL;
	}
}
//End singleton pattern