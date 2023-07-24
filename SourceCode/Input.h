#pragma once

#define DIRECTINPUT_VERSION	0x0800
#include<dinput.h>
#include<Xinput.h>

#include<wrl.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment (lib, "xinput.lib")

#include <DirectXMath.h>

using namespace DirectX;

class Input
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


public:

	void Initialize(HINSTANCE hinstance, HWND hwnd);

	//Input(WNDCLASSEX w, HWND hwnd);

	void InputUpdate();

	bool GetKey(BYTE _key) const;

	bool GetPressKey(BYTE _key) const;

	bool GetReleaseKey(BYTE _key) const;

	//パッドに接続されてるか
	bool GetPadConnect();

	//パッドのボタンが押されているか
	bool GetPadButton(UINT button);

	//パッドのボタンが離れた瞬間か
	bool GetPadButtonUp(UINT button);

	//パッドのボタンが押された瞬間か
	bool GetPadButtonDown(UINT button);

	//パッドの左スティック
	XMFLOAT2 GetPadLStick();

	//パッドの右スティック
	XMFLOAT2 GetPadRStick();

	//左トリガーを押し込んだ瞬間か
	bool GetLTriggerDown();

	//右トリガーを押し込んだ瞬間か
	bool GetRTriggerDown();

	/// <summary>
	/// 左スティック入力を得る
	/// </summary>
	/// <param name="useWASD">WASDキーもスティックに見立てて処理する</param>
	/// <param name="useArrow">矢印キーもスティックに見立てて処理する</param>
	/// <returns>入力量</returns>
	XMFLOAT2 GetLStick(bool useWASD, bool useArrow);

	/// <summary>
	/// 右スティック入力を得る
	/// </summary>
	/// <param name="useWASD">WASDキーもスティックに見立てて処理する</param>
	/// <param name="useArrow">矢印キーもスティックに見立てて処理する</param>
	/// <returns>入力量</returns>
	XMFLOAT2 GetRStick(bool useWASD, bool useArrow);



private:

	ComPtr<IDirectInput8>directInput;
	ComPtr<IDirectInputDevice8>keyboard;

	XINPUT_STATE xInputState;
	XINPUT_STATE OldxInputState;

	bool isConnectPad = false;

	BYTE key[256] = {};
	BYTE oldkey[256] = {};

};

const DirectX::XMFLOAT2 operator+= (DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2);

const DirectX::XMFLOAT2 operator+(const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2);

float length2(DirectX::XMFLOAT2& a);
void normalize2(DirectX::XMFLOAT2& a);