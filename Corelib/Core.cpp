#include "Core.h"
#include "ObjectMgr.h"
#include "SoundMgr.h"
bool Core::CoreInit()
{
	m_GameTimer.Init();
	Input::Get().Init();
	if (SUCCEEDED(InitDevice()))
	{
		I_Shader.Set(m_pd3dDevice.Get());
		I_Texture.Set(m_pd3dDevice.Get());
		DxState::SetState(m_pd3dDevice.Get());
		if (m_dxWrite.Init())
		{
			IDXGISurface1* pSurface = nullptr;
			HRESULT hr = m_pSwapChain->GetBuffer(0,
												__uuidof(IDXGISurface1),
												(void**)&pSurface);
			if (SUCCEEDED(hr))
			{
				m_dxWrite.SetRenderTarget(pSurface);
			}
			if (pSurface) pSurface->Release();
		}
	}
	Init();

	return true;
}
bool Core::GameRun()
{
	CoreInit();
	while (WinRun())
	{
		CoreFrame();
		CoreRender();
	}
	CoreRelease();

	MemoryReporting();

	return true;
}
bool Core::CoreFrame()
{
	m_GameTimer.Frame();
	Input::Get().Frame();
	I_ObjectMgr.Frame();
	I_Sound.Frame();
	Frame();
	m_dxWrite.Frame();
	return true;
}
bool Core::CoreRender()
{
	
	float color[4] = { 0.5, 0.5, 0.5, 1 };//����
	//����� �����ش�
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);

	m_pImmediateContext->PSSetSamplers(0, 1, &DxState::m_pSamplerState);

	// ����ۿ� ������ �Ѵ�.
	Render();

	m_GameTimer.Render();
	Input::Get().Render();
	m_dxWrite.Render();

	m_pSwapChain->Present(0, 0);
	// ȭ�� ���� ��ħ ������ �����մϴ�.
	//m_swapChain->Present(1, 0); //���� ����ȭ�� ���� ��� ��������� ���ΰ�ħ
	return true;
}
bool Core::CoreRelease()
{
	Release();
	DxState::Release();

	m_dxWrite.Release();
	m_GameTimer.Release();

	Input::Get().Release();

	CleanupDevice();
	return true;
}
void Core::ResizeDevice(UINT iWidth, UINT iHeight)
{
	if (m_pd3dDevice == nullptr) return;
	DeleteResizeDevice(iWidth, iHeight);

	m_dxWrite.DeleteDeviceResize();

	Device::ResizeDevice(iWidth, iHeight);

	IDXGISurface1* pSurface = nullptr;
	HRESULT hr = m_pSwapChain->GetBuffer(0,
										__uuidof(IDXGISurface1),
										(void**)&pSurface);
	if (SUCCEEDED(hr))
	{
		m_dxWrite.SetRenderTarget(pSurface);
	}
	if (pSurface) pSurface->Release();

	CreateResizeDevice(iWidth, iHeight);
}
