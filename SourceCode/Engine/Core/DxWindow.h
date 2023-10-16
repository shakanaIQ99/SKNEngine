#pragma once
//ウィンドウ
#include<Windows.h>
#include <cstdint>

//const int window_width = 1280;	//横幅
//const int window_height = 720;	//縦幅


class DxWindow
{
public:

	static const int window_width = 1280;	//横幅
	static const int window_height = 720;	//縦幅

	//static const wchar_t windowClassName[];

	//DXWindow();

	/// <summary>
	/// インスタンス
	/// </summary>
	/// <returns></returns>
	static DxWindow* GetInstance();

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="msg"></param>
	/// <param name="wparam"></param>
	/// <param name="lparam"></param>
	/// <returns></returns>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);



public:

	/// <summary>
	/// ウィンドウ生成
	/// </summary>
	/// <param name="title"></param>
	/// <param name="windowStyle"></param>
	/// <param name="clientWidth"></param>
	/// <param name="clientHeight"></param>
	void CreateGameWindow(const wchar_t* title = L"Mecha Core", UINT windowStyle = WS_OVERLAPPEDWINDOW, int32_t clientWidth = window_width, int32_t clientHeight = window_height);

	/// <summary>
	/// ウィンドウ破棄
	/// </summary>
	void TerminateGameWindow();

	/// <summary>
	/// ループ抜け
	/// </summary>
	/// <returns></returns>
	bool ProcessMessage();

	/// <summary>
	/// Hwnd取得
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd()const { return hwnd; }

	/// <summary>
	/// HInstance取得
	/// </summary>
	/// <returns></returns>
	HINSTANCE GetHInstance() const { return winClass.hInstance; }

private:

	HWND hwnd = nullptr;
	WNDCLASSEX winClass{};
	UINT winStyle_;
	RECT winRect;


	//ウィンドウサイズ{　X座標　Y座標　横幅　縦幅　}


	//RECT wrc = { 0,0,window_width, window_height };




};

