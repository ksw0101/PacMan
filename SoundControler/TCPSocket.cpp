#include "TCPSocket.h"
bool TCPSocket::AcceptClient()
{
	// 엑셉트 부분, 논블러킹
	SOCKET clientSock = accept(m_ListenSock, (sockaddr*)&m_clientAddr, &m_iLen);
	// 엑셉트가 들어오지 않은 경우 
	if (clientSock == SOCKET_ERROR)
	{
		int iError = WSAGetLastError();
		// 엑셉트에 오류가 발생한 경우
		if (iError != WSAEWOULDBLOCK)
		{
			std::cout << "ErrorCode=" << iError << std::endl;
			return false;
		}
	}
	// 엑셉트가 들어온 경우 
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
		std::cout << m_userlist.size() << " 명 접속중.." << std::endl;
		char szSendBuffer[256] = { 0, };
		sprintf(szSendBuffer, "%f", m_pSysVol->GetVolume() * 100);
		int iSendByte = send(user.m_Sock, szSendBuffer, 3, 0);

	}
	return true;
}
bool TCPSocket::ReadMessage()
{
	// 유저가 한명도 없으면 바로 리턴.
	if (m_userlist.size() <= 0)
	{
		return true;
	}
	// 유저리스트를 돈다.
	std::list<TUser>::iterator iter;
	for (iter = m_userlist.begin(); iter != m_userlist.end(); )
	{
		TUser user = *iter;
		char szRecvBuffer[256] = { 0, };
		int iRecvByte = recv(user.m_Sock, szRecvBuffer, 256, 0);
		// 접속 종료
		if (iRecvByte == 0)
		{
			closesocket(user.m_Sock);
			iter = m_userlist.erase(iter);
			std::cout << user.m_csName << " 접속종료됨." << std::endl;
			continue;
		}
		// 에러 검출
		if (iRecvByte == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				iter = m_userlist.erase(iter);
				std::cout << user.m_csName << " 비정상 접속종료됨." << std::endl;
			}
			else
			{
				iter++;
			}
		}
		// 접속 종료도 에러도 아닌 경우.
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
				//std::cout << szRecvBuffer << "받음" << std::endl;
				//int iSendByte = send(user.m_Sock, szRecvBuffer, iRecvByte, 0);
				int iSendByte = send(user.m_Sock, cCurrentSound, 4, 0);
				std::cout << user.m_Sock << ":" << iSendByte << "보냄." << std::endl;
				if (iSendByte == SOCKET_ERROR)
				{
					int iError = WSAGetLastError();
					if (iError != WSAEWOULDBLOCK)
					{
						closesocket(user.m_Sock);
						iterSend = m_userlist.erase(iterSend);
						std::cout << user.m_csName << " 비정상 접속종료됨." << std::endl;
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
			break; // 에러코드가 발생한 경우
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
		<< "서버 가동중......." << std::endl;
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
