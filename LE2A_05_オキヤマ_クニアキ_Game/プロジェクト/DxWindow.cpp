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
		//OS�ɑ΂��āA�A�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void DxWindow::CreateGameWindow(const char* title, UINT windowStyle, int32_t clientWidth, int32_t clientHeight)
{
	winStyle_ = windowStyle;
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.lpfnWndProc = (WNDPROC)WindowProc;	//�E�B���h�E�v���V�[�W����ݒ�
	winClass.lpszClassName = L"MyEngine";		//�E�B���h�E�N���X��
	winClass.hInstance = GetModuleHandle(nullptr);	//�E�B���h�E�n���h��
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);//�J�[�\���w��
	//�E�B���h�E�N���X��OS�ɓo�^����
	RegisterClassEx(&winClass);

	// �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
	RECT wrc = { 0, 0, clientWidth, clientHeight };
	AdjustWindowRect(&wrc, winStyle_, false); // �����ŃT�C�Y�␳

	hwnd = CreateWindow
	(
		winClass.lpszClassName,		//�N���X��
		L"MyEngine",			//�^�C�g���o�[�̕���
		winStyle_,	//�W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT,			//�\��X���W(OS�ɔC����)
		CW_USEDEFAULT,			//�\��Y���W(OS�ɔC����)
		wrc.right - wrc.left,	//�E�B���h�E����
		wrc.bottom - wrc.top,	//�E�B���h�E�c��
		nullptr,				//�e�E�B���h�E�n���h��
		nullptr,				//���j���[�n���h��
		winClass.hInstance,			//�Ăяo���A�v���P�[�V�����n���h��
		nullptr					//�I�v�V����
	);

	//�E�B���h�E��\����Ԃ���
	ShowWindow(hwnd, SW_SHOW);

}

void DxWindow::TerminateGameWindow()
{
	UnregisterClass(winClass.lpszClassName, winClass.hInstance);
}

bool DxWindow::ProcessMessage()
{
	MSG msg{};
	//���b�Z�[�W����H
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);	//�L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg);	//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	//�~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
	if (msg.message == WM_QUIT)
	{
		return true;
	}

	return false;
}
