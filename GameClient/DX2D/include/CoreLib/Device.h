#pragma once
#include "Window.h"
class Device : public Window
{
public:
	// windows runtime c++ template library(wrl)

	ComPtr<ID3D11Device> m_pd3dDevice;
	//ID3D11Device* m_pd3dDevice;	// ����̽� ��ü
	
	// ����̽� ���ؽ�Ʈ = ȭ�� ��¿� �ʿ��� ��� ������ ���� ����ü, GDI�� ���� ����
	ComPtr<ID3D11DeviceContext> m_pImmediateContext;
	//ID3D11DeviceContext* m_pImmediateContext;// �ٺ��̽� ���ؽ�Ʈ ��ü
	
	ComPtr<IDXGISwapChain> m_pSwapChain;
	//IDXGISwapChain* m_pSwapChain;	// ����ü�� ��ü

	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	//ID3D11RenderTargetView* m_pRenderTargetView;// ���� ����Ÿ�� ��

	D3D_FEATURE_LEVEL       m_FeatureLevel;	   // Direct3D Ư������ �Ӽ���	
	DXGI_SWAP_CHAIN_DESC	m_SwapChainDesc;   // ����ü�� �Ӽ���
	D3D11_VIEWPORT			m_ViewPort;	       // �� ��Ʈ �Ӽ���

public:
	HRESULT InitDevice();
	virtual bool CreateDevice();
	virtual bool CreateRenderTargetView();
	virtual bool SetViewport();
	virtual bool CleanupDevice();
	void ResizeDevice(UINT iWidth, UINT iHeight);
public:
	Device()
	{
		m_pd3dDevice = nullptr;
		m_pImmediateContext = nullptr;
		m_pSwapChain = nullptr;
		m_pRenderTargetView = nullptr;
	}
	virtual ~Device() {}
};
