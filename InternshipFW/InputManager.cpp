#include "InputManager.h"
#include "Game.h"
#include <cmath>


InputManager * InputManager::pInstance = NULL;

InputManager::InputManager()
{
	ClearKeyStateAll();

	maxTouch = 2;
	touchState = new int[maxTouch];
	pointerCurPos = new Vector2[maxTouch];
	for (int i = 0; i < maxTouch; i++)
	{
		ClearTouchPosition(i);
	}

	insensitivity = 5;

	int width = Game::GetInstance()->width;
	int height = Game::GetInstance()->height;
	float padUnit = height / 5 / 3.0f;
	float padLeft = 0.0f;
	float padTop = height / 5 * 4.0f;
	btnLeft = Button(padLeft, padTop, padUnit * 2, padUnit * 3);
	btnRight = Button(padLeft + padUnit, padTop, padUnit * 2, padUnit * 3);
	btnForward = Button(padLeft, padTop, padUnit * 3, padUnit * 2);
	btnBackward = Button(padLeft, padTop + padUnit, padUnit * 3, padUnit * 2);
}

InputManager::~InputManager()
{
	SAFE_DEL_ARRAY(pointerCurPos);
	SAFE_DEL_ARRAY(touchState);
}

void InputManager::CreateInstance()
{
	if ( pInstance == NULL )
	{
		pInstance = new InputManager();
	}
}

InputManager * InputManager::GetInstance()
{
	return pInstance;
}

void InputManager::DestroyInstance()
{
	SAFE_DEL(pInstance);
}

int InputManager::GetKeysState()
{
	return keysState;
}

int InputManager::GetMaxTouch()
{
	return maxTouch;
}

int * InputManager::GetTouchState()
{
	return touchState;
}

Vector2 * InputManager::GetPointerCurrentPosition()
{
	return pointerCurPos;
}

void InputManager::SetKeyState(unsigned char keyCode)
{
	keysState |= (1 << hashingKey(keyCode));
}

void InputManager::ClearKeyState(unsigned char keyCode)
{
	keysState &= ~(1 << hashingKey(keyCode));
}

int InputManager::hashingKey(unsigned char keyCode)
{
	int keyNumber = -1;
	switch (keyCode)
	{
	case VK_MOVE_LEFT:
		keyNumber = MOVE_LEFT;
		break;
	case VK_MOVE_RIGHT:
		keyNumber = MOVE_RIGHT;
		break;
	case VK_MOVE_UP:
		keyNumber = MOVE_UP;
		break;
	case VK_MOVE_DOWN:
		keyNumber = MOVE_DOWN;
		break;
	case VK_MOVE_BACKWARD:
		keyNumber = MOVE_BACKWARD;
		break;
	case VK_MOVE_FORWARD:
		keyNumber = MOVE_FORWARD;
		break;

	case VK_TURN_LEFT:
		keyNumber = TURN_LEFT;
		break;
	case VK_TURN_RIGHT:
		keyNumber = TURN_RIGHT;
		break;
	case VK_TURN_UP:
		keyNumber = TURN_UP;
		break;
	case VK_TURN_DOWN:
		keyNumber = TURN_DOWN;
		break;

	case VK_SPEED_DOWN:
		keyNumber = SPEED_DOWN;
		break;
	case VK_SPEED_UP:
		keyNumber = SPEED_UP;
		break;

	default:
		keyNumber = -1;
	}

	return keyNumber;
}

void InputManager::SetTouchState(int index, unsigned char touchCode)
{
	if (index < maxTouch)
	{
		touchState[index] |= (1 << touchCode);
	}
}

void InputManager::ClearTouchState(int index, unsigned char touchCode)
{
	if (index < maxTouch)
	{
		touchState[index] &= ~(1 << touchCode);
	}
}

void InputManager::SetTouchCurrentPosition(int index, float x, float y)
{
	if (index < maxTouch)
	{
		pointerCurPos[index].x = x;
		pointerCurPos[index].y = y;

		bool isClickLeft = (pointerCurPos[index].x > btnLeft.left && pointerCurPos[index].x < btnLeft.left + btnLeft.width) && (pointerCurPos[index].y > btnLeft.top && pointerCurPos[index].y < btnLeft.top + btnLeft.height);
		bool isClickRight = (pointerCurPos[index].x > btnRight.left && pointerCurPos[index].x < btnRight.left + btnRight.width) && (pointerCurPos[index].y > btnRight.top && pointerCurPos[index].y < btnRight.top + btnRight.height);
		bool isClickForward = (pointerCurPos[index].x > btnForward.left && pointerCurPos[index].x < btnForward.left + btnForward.width) && (pointerCurPos[index].y > btnForward.top && pointerCurPos[index].y < btnForward.top + btnForward.height);
		bool isClickBackward = (pointerCurPos[index].x > btnBackward.left && pointerCurPos[index].x < btnBackward.left + btnBackward.width) && (pointerCurPos[index].y > btnBackward.top && pointerCurPos[index].y < btnBackward.top + btnBackward.height);
		
		if (isClickLeft)
		{
			SetTouchState(index, MOVE_LEFT);
		}
		else
		{
			ClearTouchState(index, MOVE_LEFT);
		}
		if (isClickRight)
		{
			SetTouchState(index, MOVE_RIGHT);
		}
		else
		{
			ClearTouchState(index, MOVE_RIGHT);
		}
		if (isClickForward)
		{
			SetTouchState(index, MOVE_FORWARD);
		}
		else
		{
			ClearTouchState(index, MOVE_FORWARD);
		}
		if (isClickBackward)
		{
			SetTouchState(index, MOVE_BACKWARD);
		}
		else
		{
			ClearTouchState(index, MOVE_BACKWARD);
		}
	}
}

void InputManager::SetTouchDifferentPosition(int index, float x, float y)
{
	if (index < maxTouch)
	{
		if ( x - pointerCurPos[index].x > insensitivity )
		{
			SetTouchState(index, TURN_LEFT);
			SetTouchCurrentPosition(index, x, pointerCurPos[index].y);
		}
		if ( x - pointerCurPos[index].x < -insensitivity )
		{
			SetTouchState(index, TURN_RIGHT);
			SetTouchCurrentPosition(index, x, pointerCurPos[index].y);
		}
		if ( y - pointerCurPos[index].y > insensitivity  )
		{
			SetTouchState(index, TURN_UP);
			SetTouchCurrentPosition(index, pointerCurPos[index].x, y);
		}
		if ( y - pointerCurPos[index].y < -insensitivity )
		{
			SetTouchState(index, TURN_DOWN);
			SetTouchCurrentPosition(index, pointerCurPos[index].x, y);
		}
	}
}

void InputManager::ClearKeyStateAll()
{
	this->keysState = 0;
}

void InputManager::ClearTouchPosition(int index)
{
	if (index < maxTouch)
	{
		this->touchState[index] = 0;
		pointerCurPos[index].x = pointerCurPos[index].y = -1;
	}
}
