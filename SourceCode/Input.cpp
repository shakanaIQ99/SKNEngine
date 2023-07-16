#include "Input.h"
#include"DirectXCommon.h"
#include<cassert>
#include <cmath>



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
	//キーボード情報の取得開始
	keyboard->Acquire();

	memcpy(oldkey, key, sizeof(key));

	keyboard->GetDeviceState(sizeof(key), key);

	OldxInputState = xInputState;

	DWORD dresult = XInputGetState(0, &xInputState);

	if (dresult == ERROR_SUCCESS) 
	{

		isConnectPad = true;
	}
	else 
	{
		isConnectPad = false;
	}

	if ((xInputState.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		xInputState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(xInputState.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			xInputState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		xInputState.Gamepad.sThumbLX = 0;
		xInputState.Gamepad.sThumbLY = 0;
	}

	if ((xInputState.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		xInputState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(xInputState.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			xInputState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		xInputState.Gamepad.sThumbRX = 0;
		xInputState.Gamepad.sThumbRY = 0;
	}

}

bool Input::GetKey(BYTE _key) const
{
	if (key[_key])
	{
		return true;
	}

	return false;
}

bool Input::GetPressKey(BYTE _key) const
{
	if (key[_key] && !oldkey[_key])
	{
		return true;
	}
	return false;
}

bool Input::GetReleaseKey(BYTE _key) const
{
	if (key[_key] && !oldkey[_key])
	{
		return true;
	}
	return false;
}

bool Input::GetPadConnect()
{
	return isConnectPad;
}

bool Input::GetPadButton(UINT button)
{
	return xInputState.Gamepad.wButtons == button;
}

bool Input::GetPadButtonUp(UINT button)
{
	return xInputState.Gamepad.wButtons != button && OldxInputState.Gamepad.wButtons == button;
}

bool Input::GetPadButtonDown(UINT button)
{
	return xInputState.Gamepad.wButtons == button && OldxInputState.Gamepad.wButtons != button;
}

XMFLOAT2 Input::GetPadLStick()
{
	SHORT x = xInputState.Gamepad.sThumbLX;
	SHORT y = xInputState.Gamepad.sThumbLY;

	return XMFLOAT2(static_cast<float>(x) / 32767.0f, static_cast<float>(y) / 32767.0f);
}

XMFLOAT2 Input::GetPadRStick()
{
	SHORT x = xInputState.Gamepad.sThumbRX;
	SHORT y = xInputState.Gamepad.sThumbRY;

	return XMFLOAT2(static_cast<float>(x) / 32767.0f, static_cast<float>(y) / 32767.0f);
}

bool Input::GetLTriggerDown()
{
	if (OldxInputState.Gamepad.bLeftTrigger < 128 && xInputState.Gamepad.bLeftTrigger >= 128)
	{
		return true;
	}
	return false;
}

bool Input::GetRTriggerDown()
{
	if (OldxInputState.Gamepad.bRightTrigger < 128 && xInputState.Gamepad.bRightTrigger >= 128)
	{
		return true;
	}
	return false;
}

XMFLOAT2 Input::GetLStick(bool useWASD, bool useArrow)
{
	XMFLOAT2 pad;
	pad.x = static_cast<float>(xInputState.Gamepad.sThumbLX) / 32767.0f;
	pad.y = static_cast<float>(xInputState.Gamepad.sThumbLY) / 32767.0f;

	XMFLOAT2 wasd;
	if (useWASD) {
		wasd.x = static_cast<float>(key[DIK_D] - key[DIK_A]);
		wasd.y = static_cast<float>(key[DIK_W] - key[DIK_S]);
	}

	XMFLOAT2 arrow;
	if (useArrow) {
		arrow.x = static_cast<float>(key[DIK_RIGHT] - key[DIK_LEFT]);
		arrow.y = static_cast<float>(key[DIK_UP]	- key[DIK_DOWN]);
	}

	XMFLOAT2 result = pad + wasd + arrow;
	normalize2(result);
	return result;
}

XMFLOAT2 Input::GetRStick(bool useWASD, bool useArrow)
{
	XMFLOAT2 pad;
	pad.x = static_cast<float>(xInputState.Gamepad.sThumbRX) / 32767.0f;
	pad.y = static_cast<float>(xInputState.Gamepad.sThumbRY) / 32767.0f;

	XMFLOAT2 wasd;
	if (useWASD) {
		wasd.x = static_cast<float>(key[DIK_D] - key[DIK_A]);
		wasd.y = static_cast<float>(key[DIK_W] - key[DIK_S]);
	}

	XMFLOAT2 arrow;
	if (useArrow) {
		arrow.x = static_cast<float>(key[DIK_RIGHT] - key[DIK_LEFT]);
		arrow.y = static_cast<float>(key[DIK_UP] - key[DIK_DOWN]);
	}

	XMFLOAT2 result = pad + wasd + arrow;
	normalize2(result);
	return result;
}

const DirectX::XMFLOAT2 operator+=(DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
	v1.x += v2.x;
	v1.y += v2.y;

	return v1;
}

const DirectX::XMFLOAT2 operator+(const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
	XMFLOAT2 temp(v1);
	return temp += v2;
}

float length2(DirectX::XMFLOAT2& a)
{
	return sqrtf(a.x * a.x + a.y * a.y);
}

void normalize2(DirectX::XMFLOAT2& a)
{
	float len = length2(a);
	if (len != 0)
	{
		a.x /= len;
		a.y /= len;
		return;
	}

	return;
}
