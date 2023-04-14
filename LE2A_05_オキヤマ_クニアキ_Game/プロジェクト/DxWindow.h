#pragma once
#include<Windows.h>
#include <cstdint>

//const int window_width = 1280;	//����
//const int window_height = 720;	//�c��


class DxWindow
{
public:

	static const int window_width = 1280;	//����
	static const int window_height = 720;	//�c��

	//static const wchar_t windowClassName[];

	//DXWindow();

	static DxWindow* GetInstance();

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);



public:

	void CreateGameWindow(const char* title = "MyEngine", UINT windowStyle = WS_OVERLAPPEDWINDOW, int32_t clientWidth = window_width, int32_t clientHeight = window_height);

	void TerminateGameWindow();

	bool ProcessMessage();

	HWND GetHwnd()const { return hwnd; }
	HINSTANCE GetHInstance() const { return winClass.hInstance; }

private:

	HWND hwnd = nullptr;
	WNDCLASSEX winClass{};
	UINT winStyle_;
	RECT winRect;


	//�E�B���h�E�T�C�Y{�@X���W�@Y���W�@�����@�c���@}


	//RECT wrc = { 0,0,window_width, window_height };




};

