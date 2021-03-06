#pragma once
//#include <winsock2.h>			//소켓 통신
#include <windows.h>
#include <d3d11.h>				// Device, DC, SC 생성
#include <dxgidebug.h>			// DXGIGetDebugInterface  디버그 제어
#include <atlconv.h>			// A2W , 엑티브 템플릿

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
// lib 파일로 저장된 자신의 파일을 다시 읽어옴으로 vs에서 참조 추가가 필요없다.
// header파일만 연결되면 헤더 파일은 먼저 다 읽기 때문에
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
	USES_CONVERSION;//유니코드 <-> 멀티바이트
	return std::wstring(A2W(_src.c_str()));//멀티바이트 -> 유니코드
};

static std::string to_wm(const std::wstring& _src)
{
	USES_CONVERSION;
	return std::string(W2A(_src.c_str()));//유니코드 -> 멀티바이트
};

template<class T>
class Singleton //객체의 인스턴스가 오직 1개만 생성되는 패턴
{
public://T->자식
	static T& Get()
	{
		static T theSingle;
		return theSingle;
	}
};

static void DisplayText(const char* fmt, ...)
{
	//가변인자(파라미터의 수가 정해지지않은것)
	va_list arg;                    //각 가변인자의 시작 주소를 가리키는 포인터(목록)
	va_start(arg, fmt);             //가변인자 중 첫번째 인자의 주소를 가르킴(포인터,고정인수)
	char buf[1024 + 256] = { 0, };  //
	vsprintf_s(buf, fmt, arg);      //조립된 문자열을 화면이 아닌 버퍼에 전달받음
	OutputDebugStringA((char*)buf); //디버깅 중 출력

	va_end(arg);                    //사용한 가변 인자 끝냄
}
static void MemoryReporting()
{
#if defined(DEBUG) | defined(_DEBUG)
	HMODULE dxgidebugdll = GetModuleHandleW(L"dxgidebug.dll");
	decltype(&DXGIGetDebugInterface) GetDebugInterface = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(dxgidebugdll, "DXGIGetDebugInterface"));
	IDXGIDebug* debug;
	GetDebugInterface(IID_PPV_ARGS(&debug));
	OutputDebugStringW(L"Starting Live Direct3D Object Dump:\r\n");
	debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_ALL);//개체 정보 보고
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

