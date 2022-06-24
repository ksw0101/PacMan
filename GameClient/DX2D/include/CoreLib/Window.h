#pragma once
#include "std.h"
class Window
{
public:
	
	//�ν��Ͻ� = �ֹε�Ϲ�ȣ
	HINSTANCE m_hInsatance;
	//�ڵ� = �̸�
	HWND m_hWnd;
	RECT m_rtClient;
	RECT m_rtWindow;

	//1. ������ Ŭ���� ��� / �㰡 (�ü��)
	BOOL  SetWinClass(HINSTANCE hInstance);
	//2. ������ ����
	BOOL  SetWindow(const WCHAR* szTitle = L"StarLight", 
					int iWidth = 1000, 
					int iHeight = 500);
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	virtual void ResizeDevice(UINT iWidth, UINT iHeight);
	bool  WinRun();

	Window();
	virtual ~Window() {}
};

