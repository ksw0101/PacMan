#pragma once
#include "std.h"
#include "SystemVolume.h"
#include "Timer.h"
struct TUser
{
	SOCKET		m_Sock;
	SOCKADDR_IN m_Addr;
	std::string m_csName;
	short       m_iPort;
	void set(SOCKET sock, SOCKADDR_IN addr)
	{
		m_Sock = sock;
		m_Addr = addr;
		m_csName = inet_ntoa(addr.sin_addr);
		m_iPort = ntohs(addr.sin_port);
	}
};
class TCPSocket
{
	SystemVolume*	m_pSysVol;
	SOCKET			m_ListenSock;
	int				m_iLen;
	SOCKADDR_IN		m_clientAddr;
	std::list<TUser> m_userlist;
	HWND			m_hConsole;
	Timer			m_Timer;
private:
	bool AcceptClient();
	bool ReadMessage();
	bool CheckTime();
public:
	bool frame();
	TCPSocket();
	~TCPSocket();
};

