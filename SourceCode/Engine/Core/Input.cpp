#include "Input.h"
#include"DirectXCommon.h"
#include<cassert>
#include <cmath>



Input* Input::GetInstance()
{
	static Input instance;
	return &instance;
}

void Input::Init(DxWindow* win)
{
	GetInstance()->Initialize(win->GetHInstance(),win->GetHwnd());
}

void Input::Initialize(HINSTANCE hinstance, HWND hwnd)
{
#pragma region	キーボード周り
	HRESULT result;

	//DirectInputの初期化
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);	//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

#pragma endregion	キーボード周り初期化
	ZeroMemory(&xInputState, sizeof(XINPUT_STATE));
	DWORD dresult;
	dresult = XInputGetState(0, &xInputState);

	if (result == ERROR_SUCCESS)
	{
		isConnectPad = true;
	}
	else
	{
		isConnectPad = false;
	}

}

void Input::InputUpdate()
{
	Input* instance = GetInstance();

	//キーボード情報の取得開始
	instance->keyboard->Acquire();

	memcpy(instance->oldkey, instance->key, sizeof(key));

	instance->keyboard->GetDeviceState(sizeof(key), instance->key);

	instance->OldxInputState = instance->xInputState;

	DWORD dresult = XInputGetState(0, &instance->xInputState);

	if (dresult == ERROR_SUCCESS) 
	{

		instance->isConnectPad = true;
	}
	else 
	{
		instance->isConnectPad = false;
	}

	if ((instance->xInputState.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		instance->xInputState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(instance->xInputState.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			instance->xInputState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		instance->xInputState.Gamepad.sThumbLX = 0;
		instance->xInputState.Gamepad.sThumbLY = 0;
	}

	if ((instance->xInputState.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		instance->xInputState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(instance->xInputState.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			instance->xInputState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		instance->xInputState.Gamepad.sThumbRX = 0;
		instance->xInputState.Gamepad.sThumbRY = 0;
	}

}

bool Input::GetKey(BYTE _key)
{
	if (GetInstance()->key[_key])
	{
		return true;
	}

	return false;
}

bool Input::GetPressKey(BYTE _key)
{
	if (GetInstance()->key[_key] && !GetInstance()->oldkey[_key])
	{
		return true;
	}
	return false;
}

bool Input::GetReleaseKey(BYTE _key)
{
	if (GetInstance()->key[_key] && !GetInstance()->oldkey[_key])
	{
		return true;
	}
	return false;
}

bool Input::GetPadConnect()
{
	return GetInstance()->isConnectPad;
}

bool Input::GetPadButton(UINT button)
{
	return GetInstance()->xInputState.Gamepad.wButtons == button;
}

bool Input::GetPadButtonUp(UINT button)
{
	return GetInstance()->xInputState.Gamepad.wButtons != button && GetInstance()->OldxInputState.Gamepad.wButtons == button;
}

bool Input::GetPadButtonDown(UINT button)
{
	return GetInstance()->xInputState.Gamepad.wButtons == button && GetInstance()->OldxInputState.Gamepad.wButtons != button;
}

Vector2 Input::GetPadLStick()
{
	SHORT x = GetInstance()->xInputState.Gamepad.sThumbLX;
	SHORT y = GetInstance()->xInputState.Gamepad.sThumbLY;

	return Vector2(static_cast<float>(x) / 32767.0f, static_cast<float>(y) / 32767.0f);
}

Vector2 Input::GetPadRStick()
{
	SHORT x = GetInstance()->xInputState.Gamepad.sThumbRX;
	SHORT y = GetInstance()->xInputState.Gamepad.sThumbRY;

	return Vector2(static_cast<float>(x) / 32767.0f, static_cast<float>(y) / 32767.0f);
}

bool Input::GetLTriggerDown()
{
	if (GetInstance()->OldxInputState.Gamepad.bLeftTrigger < 128 && GetInstance()->xInputState.Gamepad.bLeftTrigger >= 128)
	{
		return true;
	}
	return false;
}

bool Input::GetLTrigger()
{
	if (GetInstance()->OldxInputState.Gamepad.bLeftTrigger > 128)
	{
		return true;
	}
	return false;
}

bool Input::GetRTriggerDown()
{
	if (GetInstance()->OldxInputState.Gamepad.bRightTrigger < 128 && GetInstance()->xInputState.Gamepad.bRightTrigger >= 128)
	{
		return true;
	}
	return false;
}

bool Input::GetRTrigger()
{
	if (GetInstance()->OldxInputState.Gamepad.bRightTrigger > 128)
	{
		return true;
	}
	return false;
}

Vector2 Input::GetLStick(bool useWASD, bool useArrow)
{
	Vector2 pad;
	pad.x = static_cast<float>(GetInstance()->xInputState.Gamepad.sThumbLX) / 32767.0f;
	pad.y = static_cast<float>(GetInstance()->xInputState.Gamepad.sThumbLY) / 32767.0f;

	Vector2 wasd;
	if (useWASD) {
		wasd.x = static_cast<float>(GetInstance()->key[DIK_D] - GetInstance()->key[DIK_A]);
		wasd.y = static_cast<float>(GetInstance()->key[DIK_W] - GetInstance()->key[DIK_S]);
	}

	Vector2 arrow;
	if (useArrow) {
		arrow.x = static_cast<float>(GetInstance()->key[DIK_RIGHT] - GetInstance()->key[DIK_LEFT]);
		arrow.y = static_cast<float>(GetInstance()->key[DIK_UP]	- GetInstance()->key[DIK_DOWN]);
	}

	Vector2 result = pad + wasd + arrow;
	result.Normalize();
	return result;
}

Vector2 Input::GetRStick(bool useWASD, bool useArrow)
{
	Vector2 pad;
	pad.x = static_cast<float>(GetInstance()->xInputState.Gamepad.sThumbRX) / 32767.0f;
	pad.y = static_cast<float>(GetInstance()->xInputState.Gamepad.sThumbRY) / 32767.0f;

	Vector2 wasd;
	if (useWASD) {
		wasd.x = static_cast<float>(GetInstance()->key[DIK_D] - GetInstance()->key[DIK_A]);
		wasd.y = static_cast<float>(GetInstance()->key[DIK_W] - GetInstance()->key[DIK_S]);
	}

	Vector2 arrow;
	if (useArrow) {
		arrow.x = static_cast<float>(GetInstance()->key[DIK_RIGHT] - GetInstance()->key[DIK_LEFT]);
		arrow.y = static_cast<float>(GetInstance()->key[DIK_UP] - GetInstance()->key[DIK_DOWN]);
	}

	Vector2 result = pad + wasd + arrow;
	result.Normalize();
	return result;
}

Vector2 Input::GetMousePostion()
{
	return Vector2();
}

