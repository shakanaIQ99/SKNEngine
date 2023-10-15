#pragma once
//入力関係
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
	/// <summary>
	/// インスタンス
	/// </summary>
	/// <returns></returns>
	static Input* GetInstance();

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="hinstance"></param>
	/// <param name="hwnd"></param>
	static void Init(HINSTANCE hinstance, HWND hwnd);

	/// <summary>
	/// 静的更新
	/// </summary>
	static void InputUpdate();

	/// <summary>
	/// 押されているか
	/// </summary>
	/// <param name="_key"></param>
	/// <returns></returns>
	static bool GetKey(BYTE _key);

	/// <summary>
	/// 押した瞬間か
	/// </summary>
	/// <param name="_key"></param>
	/// <returns></returns>
	static bool GetPressKey(BYTE _key);

	/// <summary>
	/// 離した瞬間か
	/// </summary>
	/// <param name="_key"></param>
	/// <returns></returns>
	static bool GetReleaseKey(BYTE _key);

	//パッドに接続されてるか
	static bool GetPadConnect();

	//パッドのボタンが押されているか
	static bool GetPadButton(UINT button);

	//パッドのボタンが離れた瞬間か
	static bool GetPadButtonUp(UINT button);

	//パッドのボタンが押された瞬間か
	static bool GetPadButtonDown(UINT button);

	//パッドの左スティック
	static XMFLOAT2 GetPadLStick();

	//パッドの右スティック
	static XMFLOAT2 GetPadRStick();

	//左トリガーを押し込んだ瞬間か
	static bool GetLTriggerDown();

	//左トリガーを押されているか
	static bool GetLTrigger();


	//右トリガーを押し込んだ瞬間か
	static bool GetRTriggerDown();


	//右トリガーを押されているか
	static bool GetRTrigger();

	/// <summary>
	/// 左スティック入力を得る
	/// </summary>
	/// <param name="useWASD">WASDキーもスティックに見立てて処理する</param>
	/// <param name="useArrow">矢印キーもスティックに見立てて処理する</param>
	/// <returns>入力量</returns>
	static XMFLOAT2 GetLStick(bool useWASD, bool useArrow);

	/// <summary>
	/// 右スティック入力を得る
	/// </summary>
	/// <param name="useWASD">WASDキーもスティックに見立てて処理する</param>
	/// <param name="useArrow">矢印キーもスティックに見立てて処理する</param>
	/// <returns>入力量</returns>
	static XMFLOAT2 GetRStick(bool useWASD, bool useArrow);



private:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="hinstance"></param>
	/// <param name="hwnd"></param>
	void Initialize(HINSTANCE hinstance, HWND hwnd);

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