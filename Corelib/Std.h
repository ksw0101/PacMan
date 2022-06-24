#pragma once
//#include <winsock2.h>			//���� ���
#include <windows.h>
#include <d3d11.h>				// Device, DC, SC ����
#include <dxgidebug.h>			// DXGIGetDebugInterface  ����� ����
#include <atlconv.h>			// A2W , ��Ƽ�� ���ø�

#include <vector>
#include <list>
#include <queue>
#include <map>
#include <tchar.h>
#include <string>
#include <functional>			// placeholders
#include <iostream>

#include "Collision.h"
#include <wrl.h> 
using namespace Microsoft::WRL;

#pragma comment(lib, "DirectXTK.lib")
#pragma comment	(lib, "d3d11.lib")
// lib ���Ϸ� ����� �ڽ��� ������ �ٽ� �о������ vs���� ���� �߰��� �ʿ����.
// header���ϸ� ����Ǹ� ��� ������ ���� �� �б� ������
#ifdef _DEBUG
#pragma comment	(lib, "Corelib_d.lib")	
#else
#pragma comment	(lib, "Corelib_r.lib")
#endif

#pragma comment	(lib, "ws2_32.lib")

using namespace std;

extern RECT		g_rtClient;
extern HWND		g_hwnd;
extern float	g_fSecPerFrame;
extern float	g_fGameTimer;
extern POINT	g_ptMouse;

static std::wstring to_mw(const std::string& _src)
{
	USES_CONVERSION;//�����ڵ� <-> ��Ƽ����Ʈ
	return std::wstring(A2W(_src.c_str()));//��Ƽ����Ʈ -> �����ڵ�
};

static std::string to_wm(const std::wstring& _src)
{
	USES_CONVERSION;
	return std::string(W2A(_src.c_str()));//�����ڵ� -> ��Ƽ����Ʈ
};

template<class T>
class Singleton //��ü�� �ν��Ͻ��� ���� 1���� �����Ǵ� ����
{
public://T->�ڽ�
	static T& Get()
	{
		static T theSingle;
		return theSingle;
	}
};

static void DisplayText(const char* fmt, ...)
{
	//��������(�Ķ������ ���� ��������������)
	va_list arg;                    //�� ���������� ���� �ּҸ� ����Ű�� ������(���)
	va_start(arg, fmt);             //�������� �� ù��° ������ �ּҸ� ����Ŵ(������,�����μ�)
	char buf[1024 + 256] = { 0, };  //
	vsprintf_s(buf, fmt, arg);      //������ ���ڿ��� ȭ���� �ƴ� ���ۿ� ���޹���
	OutputDebugStringA((char*)buf); //����� �� ���

	va_end(arg);                    //����� ���� ���� ����
}
static void MemoryReporting()
{
#if defined(DEBUG) | defined(_DEBUG)
	HMODULE dxgidebugdll = GetModuleHandleW(L"dxgidebug.dll");
	decltype(&DXGIGetDebugInterface) GetDebugInterface = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(dxgidebugdll, "DXGIGetDebugInterface"));
	IDXGIDebug* debug;
	GetDebugInterface(IID_PPV_ARGS(&debug));
	OutputDebugStringW(L"Starting Live Direct3D Object Dump:\r\n");
	debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_ALL);//��ü ���� ����
	OutputDebugStringW(L"Completed Live Direct3D Object Dump.\r\n");
	debug->Release();
#endif
}

#define RUN()											   \
int WINAPI wWinMain( 									   \
	HINSTANCE hInstance, 								   \
	HINSTANCE hPrevInstance, 							   \
	LPWSTR lpCmdLine,									   \
	int nCmdShow )										   \
{														   \
	Sample core;										   \
	if (core.SetWinClass(hInstance) == FALSE) 			   \
		return 1;										   \
	if (core.SetWindow() == FALSE) 						   \
		return 1;										   \
	core.GameRun();										   \
	return 1;											   \
}

#define GAME_RUN(Title, width, height) 					   \
int WINAPI wWinMain(									   \
			HINSTANCE hInstance,						   \
			HINSTANCE hPrevInstance,					   \
			LPWSTR lpCmdLine,							   \
			int nCmdShow)								   \
{														   \
	Sample core;										   \
	if (core.SetWinClass(hInstance) == FALSE) 			   \
		return 1;   									   \
	if (core.SetWindow(L#Title, width, height) == FALSE)   \
		return 1;										   \
	core.GameRun();										   \
	return 1;											   \
}														   




#define BEGIN_START(S) friend class Singleton<S>

