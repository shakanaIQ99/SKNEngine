#include "DxWindow.h"
#include <string>
#include"imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


DxWindow* DxWindow::GetInstance()
{
	static DxWindow instance;
	return &instance;
}

LRESULT DxWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}
	switch (msg)
	{
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void DxWindow::CreateGameWindow(const wchar_t* title, UINT windowStyle, int32_t clientWidth, int32_t clientHeight)
{
	winStyle_ = windowStyle;
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.lpfnWndProc = (WNDPROC)WindowProc;	//ウィンドウプロシージャを設定
	winClass.lpszClassName = title;		//ウィンドウクラス名
	winClass.hInstance = GetModuleHandle(nullptr);	//ウィンドウハンドル
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソル指定
	//ウィンドウクラスをOSに登録する
	RegisterClassEx(&winClass);

	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, clientWidth, clientHeight };
	AdjustWindowRect(&wrc, winStyle_, false); // 自動でサイズ補正

	hwnd = CreateWindow
	(
		winClass.lpszClassName,		//クラス名
		title,			//タイトルバーの文字
		winStyle_,	//標準的なウィンドウスタイル
		CW_USEDEFAULT,			//表示X座標(OSに任せる)
		CW_USEDEFAULT,			//表示Y座標(OSに任せる)
		wrc.right - wrc.left,	//ウィンドウ横幅
		wrc.bottom - wrc.top,	//ウィンドウ縦幅
		nullptr,				//親ウィンドウハンドル
		nullptr,				//メニューハンドル
		winClass.hInstance,			//呼び出しアプリケーションハンドル
		nullptr					//オプション
	);

	//ウィンドウを表示状態する
	ShowWindow(hwnd, SW_SHOW);

}

void DxWindow::TerminateGameWindow()
{
	UnregisterClass(winClass.lpszClassName, winClass.hInstance);
}

bool DxWindow::ProcessMessage()
{
	MSG msg{};
	//メッセージある？
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);	//キー入力メッセージの処理
		DispatchMessage(&msg);	//プロシージャにメッセージを送る
	}

	//×ボタンで終了メッセージが来たらゲームループを抜ける
	if (msg.message == WM_QUIT)
	{
		return true;
	}

	return false;
}
