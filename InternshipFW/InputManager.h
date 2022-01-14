#pragma once
#include "Defines.h"
#include "Vector2.h"
#include "Vector4.h"


typedef Vector4 Button;

class InputManager
{
private:
	int keysState;
	int hashingKey(unsigned char keyCode);

	int maxTouch;
	int * touchState;
	Vector2 * pointerCurPos;
	Button btnLeft, btnRight, btnForward, btnBackward;

protected:
	static InputManager * pInstance;
	InputManager();
	~InputManager();
	InputManager(const InputManager &) {}
	InputManager& operator =(const InputManager &) {}

public:
	int insensitivity;
	enum
	{
		VK_MOVE_LEFT = 0x41,
		VK_MOVE_RIGHT = 0x44,
		VK_MOVE_UP,
		VK_MOVE_DOWN,
		VK_MOVE_BACKWARD = 0x53,
		VK_MOVE_FORWARD = 0x57,

		VK_TURN_LEFT = 0x25,
		VK_TURN_RIGHT = 0x27,
		VK_TURN_UP = 0x26,
		VK_TURN_DOWN = 0x28,

		VK_SPEED_DOWN = 0xBC,
		VK_SPEED_UP = 0xBE
	};
	enum
	{
		MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN, MOVE_BACKWARD, MOVE_FORWARD,
		TURN_LEFT, TURN_RIGHT, TURN_UP, TURN_DOWN,
		SPEED_DOWN, SPEED_UP
	};
	enum
	{
		IS_MOVE_LEFT_PRESSED = 1 << MOVE_LEFT,
		IS_MOVE_RIGHT_PRESSED = 1 << MOVE_RIGHT,
		IS_MOVE_UP_PRESSED = 1 << MOVE_UP,
		IS_MOVE_DOWN_PRESSED = 1 << MOVE_DOWN,
		IS_MOVE_BACKWARD_PRESSED = 1 << MOVE_BACKWARD,
		IS_MOVE_FORWARD_PRESSED = 1 << MOVE_FORWARD,

		IS_TURN_LEFT_PRESSED = 1 << TURN_LEFT,
		IS_TURN_RIGHT_PRESSED = 1 << TURN_RIGHT,
		IS_TURN_UP_PRESSED = 1 << TURN_UP,
		IS_TURN_DOWN_PRESSED = 1 << TURN_DOWN,

		IS_SPEED_DOWN = 1 << SPEED_DOWN,
		IS_SPEED_UP = 1 << SPEED_UP
	};

	static void CreateInstance();
	static InputManager * GetInstance();
	static void DestroyInstance();

	virtual int GetKeysState();
	virtual int GetMaxTouch();
	virtual int * GetTouchState();
	virtual Vector2 * GetPointerCurrentPosition();

	virtual void SetKeyState(unsigned char keyCode);
	virtual void ClearKeyState(unsigned char keyCode);
	virtual void SetTouchState(int index, unsigned char touchCode);
	virtual void ClearTouchState(int index, unsigned char touchCode);
	virtual void SetTouchCurrentPosition(int index, float x, float y);
	virtual void SetTouchDifferentPosition(int index, float x, float y);

	virtual void ClearKeyStateAll();
	virtual void ClearTouchPosition(int index);
};
