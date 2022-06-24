#pragma once
#include "std.h"
class Window
{
public:
	
	HINSTANCE m_hInsatance;
	//�ڵ� = �̸�
	HWND m_hWnd;
	RECT m_rtClient;
	RECT m_rtWindow;

	//1. ������ Ŭ���� ��� / �㰡 (�ü��)
	BOOL  SetWinClass(HINSTANCE hInstance);
	//2. ������ ����
	BOOL  SetWindow(const WCHAR* szTitle = L"Pac_Man", 
					int iWidth = 800, 
					int iHeight = 800);
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	virtual void ResizeDevice(UINT iWidth, UINT iHeight);
	bool  WinRun();

	Window();
	virtual ~Window() {}
};

