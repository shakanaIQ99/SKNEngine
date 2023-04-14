#include "Input.h"
#include"DirectXCommon.h"
#include<cassert>



void Input::Initialize(HINSTANCE hinstance, HWND hwnd)
{
#pragma region	�L�[�{�[�h����
	HRESULT result;

	//DirectInput�̏�����
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//�L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);	//�W���`��
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

#pragma endregion	�L�[�{�[�h���菉����

}

void Input::InputUpdate()
{
	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();

	memcpy(oldkey, key, sizeof(key));

	keyboard->GetDeviceState(sizeof(key), key);
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