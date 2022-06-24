#include "TCPSocket.h"
bool TCPSocket::AcceptClient()
{
	// ����Ʈ �κ�, ���ŷ
	SOCKET clientSock = accept(m_ListenSock, (sockaddr*)&m_clientAddr, &m_iLen);
	// ����Ʈ�� ������ ���� ��� 
	if (clientSock == SOCKET_ERROR)
	{
		int iError = WSAGetLastError();
		// ����Ʈ�� ������ �߻��� ���
		if (iError != WSAEWOULDBLOCK)
		{
			std::cout << "ErrorCode=" << iError << std::endl;
			return false;
		}
	}
	// ����Ʈ�� ���� ��� 
	else
	{
		TUser user;
		user.set(clientSock, m_clientAddr);
		m_userlist.push_back(user);

		std::cout
			<< "ip =" << inet_ntoa(m_clientAddr.sin_addr)
			<< "port =" << ntohs(m_clientAddr.sin_port)
			<< "  " << std::endl;
		u_long on = 1;
		ioctlsocket(clientSock, FIONBIO, &on);
		std::cout << m_userlist.size() << " �� ������.." << std::endl;
		char szSendBuffer[256] = { 0, };
		sprintf(szSendBuffer, "%f", m_pSysVol->GetVolume() * 100);
		int iSendByte = send(user.m_Sock, szSendBuffer, 3, 0);

	}
	return true;
}
bool TCPSocket::ReadMessage()
{
	// ������ �Ѹ� ������ �ٷ� ����.
	if (m_userlist.size() <= 0)
	{
		return true;
	}
	// ��������Ʈ�� ����.
	std::list<TUser>::iterator iter;
	for (iter = m_userlist.begin(); iter != m_userlist.end(); )
	{
		TUser user = *iter;
		char szRecvBuffer[256] = { 0, };
		int iRecvByte = recv(user.m_Sock, szRecvBuffer, 256, 0);
		// ���� ����
		if (iRecvByte == 0)
		{
			closesocket(user.m_Sock);
			iter = m_userlist.erase(iter);
			std::cout << user.m_csName << " ���������." << std::endl;
			continue;
		}
		// ���� ����
		if (iRecvByte == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				iter = m_userlist.erase(iter);
				std::cout << user.m_csName << " ������ ���������." << std::endl;
			}
			else
			{
				iter++;
			}
		}
		// ���� ���ᵵ ������ �ƴ� ���.
		else
		{
			int getNum = (int)szRecvBuffer[0];

			switch (getNum)
			{
			case 49:
				m_pSysVol->ChangeVolumeUp();
				break;
			case 50:
				m_pSysVol->ChangeVolumeDown();
				break;
			case 51:
				ShowWindow(m_hConsole, SW_SHOW);
				break;
			case 52:
				ShowWindow(m_hConsole, SW_HIDE);
				break;
			default:
				break;
			}

			int iCurrentSound = (int)(m_pSysVol->GetVolume() * 100);
			char cCurrentSound[4];
			sprintf(cCurrentSound, "%d", iCurrentSound);

			std::list<TUser>::iterator iterSend;
			for (iterSend = m_userlist.begin(); iterSend != m_userlist.end(); )
			{
				TUser user = *iterSend;
				//std::cout << szRecvBuffer << "����" << std::endl;
				//int iSendByte = send(user.m_Sock, szRecvBuffer, iRecvByte, 0);
				int iSendByte = send(user.m_Sock, cCurrentSound, 4, 0);
				std::cout << user.m_Sock << ":" << iSendByte << "����." << std::endl;
				if (iSendByte == SOCKET_ERROR)
				{
					int iError = WSAGetLastError();
					if (iError != WSAEWOULDBLOCK)
					{
						closesocket(user.m_Sock);
						iterSend = m_userlist.erase(iterSend);
						std::cout << user.m_csName << " ������ ���������." << std::endl;
					}
				}
				else
				{
					iterSend++;
				}
			}
			if (iter != m_userlist.end())
			{
				iter++;
			}
		}
	}
	return true;
}
bool TCPSocket::CheckTime()
{
	int CurrentHour = m_Timer.GetTime();

	return true;
}
bool TCPSocket::frame()
{
	while (1)
	{
		if (!AcceptClient())
		{
			break; // �����ڵ尡 �߻��� ���
		}
		if (!ReadMessage())
		{
			break;
		}
		Sleep(1);
	}
	return true;
}
TCPSocket::TCPSocket()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	m_ListenSock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	int iRet = bind(m_ListenSock, (sockaddr*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)  return;
	iRet = listen(m_ListenSock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)  return;

	m_iLen = sizeof(m_clientAddr);

	std::cout
		<< "���� ������......." << std::endl;
	u_long on = 1;
	ioctlsocket(m_ListenSock, FIONBIO, &on);
	m_pSysVol = new SystemVolume;
	m_hConsole = GetConsoleWindow();

}
TCPSocket::~TCPSocket()
{
	delete m_pSysVol;
	m_pSysVol = nullptr;
	closesocket(m_ListenSock);
	WSACleanup();
}
