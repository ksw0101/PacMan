#include "Sample.h"
#include "ObjectMgr.h"
void Sample::CreateResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
void Sample::DeleteResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}

bool Sample::Init()
{
	I_Sound.Init();

	m_IntroWorld.Init();
	m_IntroWorld.m_pd3dDevice = m_pd3dDevice.Get();
	m_IntroWorld.m_pContext = m_pImmediateContext.Get();
	m_IntroWorld.Load(L"intro.txt");

	m_IntroWorld.m_pNextWorld = &m_GameWorld;

	m_GameWorld.Init();
	m_GameWorld.World::m_pd3dDevice = m_pd3dDevice.Get();
	m_GameWorld.World::m_pContext = m_pImmediateContext.Get();
	m_GameWorld.m_pNextWorld = &m_IntroWorld;

	World::m_pWorld = &m_IntroWorld;


	return true;
}
bool Sample::Frame()
{
	World::m_pWorld->Frame();
	if (World::m_pWorld->btnintro == true)
	{
		World::m_pWorld->btnintro = false;

	}

	if (World::m_pWorld->btnrule == true)
	{
		I_ObjectMgr.Release();
		World::m_pWorld = &m_IntroWorld;
		World::m_pWorld->btnrule = false;
	}
	if (m_GameWorld.m_PlayerObj.m_life <= 0)
	{
		World::m_pWorld->btngame = false;
		m_GameWorld.m_gamestart = false;
		m_GameWorld.m_PlayerObj.m_life = 3;
		m_GameWorld.m_pBackGroundMusic->Stop();
		World::m_pWorld->m_bLoadZone = false;

		m_GameWorld.m_score = (int)m_GameTimer.m_fTimer * 100;
		I_ObjectMgr.Release();
		m_GameWorld.Release();

	}

	return true;
}
bool Sample::Render()
{
	World::m_pWorld->Render();
	if (World::m_pWorld == (World*)&m_GameWorld)
	{
		if (m_GameWorld.m_gamestart == false)
		{
			m_GameTimer.m_fTimer = 0;
			m_GameWorld.m_gamestart = true;
		}
		std::wstring msg = L"HI-SCORE\n";
		msg += std::to_wstring((int)m_GameTimer.m_fTimer*100);
		RECT a;
		a.top = 30;
		a.left = 600;
		a.bottom = g_rtClient.bottom;
		a.right = g_rtClient.right;
		if (m_colortime == 0)
		{
			m_dxWrite.Draw(msg, a, D2D1::ColorF(0.8, 0.02, 0.02, 1));
		}
		else if(m_colortime!=0)
		{
			m_checktime += g_fSecPerFrame;
			m_dxWrite.Draw(msg, a, D2D1::ColorF(1, 0, 0, 1));
		}
		if (m_checktime >= m_colortime)
		{
			m_colortime = 0;
		}
		if (m_GameWorld.m_plusescore == true)
		{
			std::wstring msg = L"HI-SCORE\n";
			m_GameTimer.m_fTimer += 10;
			msg += std::to_wstring((int)m_GameTimer.m_fTimer * 100);
			m_dxWrite.Draw(msg, a, D2D1::ColorF(1, 0, 0, 1));
			m_GameWorld.m_plusescore = false;
			m_colortime = 1.5;
			m_checktime = g_fSecPerFrame;
		}
	}

	return true;
}
bool Sample::Release()
{
	I_Sound.Release();
	m_IntroWorld.Release();
	m_GameWorld.Release();

	return true;
}
Sample::Sample()
{

}
Sample:: ~Sample()
{

}
RUN();