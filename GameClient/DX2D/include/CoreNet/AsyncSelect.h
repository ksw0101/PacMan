#pragma once
#include "NetWork.h"
//+1�� ���ִ� ������ WM_USER�� 0x400�̴ϱ� �� �������� �ý����� �̿��Ѵ�. �׷��� ���� ���� +1������ ����� ����Ѵٴ� �ǹ�
//WM_USER - �ý��ۿ��� ����ڰ� window message �� ����� �� �ְ� �����ص� ����!
#define NETWORK_MSG WM_USER+1
class AsyncSelect : public NetWork
{
public:
	bool m_connect = false;
	bool Connect(HWND hwnd, int protocol, int port, const char* ip);
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

