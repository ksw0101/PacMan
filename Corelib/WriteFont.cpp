#include "WriteFont.h"

void WriteFont::DeleteDeviceResize()
{
	if (m_pd2dColorBrush)m_pd2dColorBrush->Release();
	if (m_pd2dRT)m_pd2dRT->Release();
}
bool WriteFont ::Init()
{
	//DirectWrite �� �ؽ�Ʈ�� �׸��� ���
	//1. DirectWrite ���丮 ���� -> DWriteCreateFactory - dwrite.h, dwrite.lib
	//2. ���丮�� �ؽ�Ʈ ���� ���� -> CreateTextFormat
	//3. Direct2D ����Ÿ���� DrawTextW �޼ҵ带 ����� �׸���.

	//Direct2D �� �����ϴ� ��ü���� ID2D1Resource �������̽��κ��� ��ӹ޾Ƽ� �����ǰ�, 
	//ID2D1Factory ��ü�� ID2D1Resource �� ��ӹ޾� ������� ��ü�� �����ϰ� ����� �� �ֵ��� ����
	// 
	//D2D1CreateFactory - Direct2D ���ҽ��� ����� �� ����� �� �ִ� ���͸� ��ü
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pd2dFactory);
	if (SUCCEEDED(hr))
	{                                   
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,        //���μ��� ���� ���
								 __uuidof(IDWriteFactory),          //COM ���� ����  
								 (IUnknown**)(&m_pWriteFactory));   //��ȯ��
		if (SUCCEEDED(hr))
		{
			hr = m_pWriteFactory->CreateTextFormat( L"�ü�", //��Ʈ �̸�
													NULL, //��Ʈ �÷���(��Ʈ �ȿ� ����ִ� �ٸ� ����)
													DWRITE_FONT_WEIGHT_NORMAL, //����
													DWRITE_FONT_STYLE_NORMAL, //��Ÿ��
													DWRITE_FONT_STRETCH_NORMAL, //��Ʈ �Ӽ� �� �ϳ�
													20, //ũ��
													L"ko-kr", //��� ���� �̸� L"en-us"
													&m_pd2dTextFormat); //��� ����Ǵ� ������
			hr = m_pWriteFactory->CreateTextFormat( L"Imprint MT Shadow",
													NULL,
													DWRITE_FONT_WEIGHT_NORMAL,
													DWRITE_FONT_STYLE_NORMAL,
													DWRITE_FONT_STRETCH_NORMAL,
													50,
													L"en-us",
													&m_pd2dMTShadowTF);
			hr = m_pWriteFactory->CreateTextFormat(L"PF��Ÿ����Ʈ",
													NULL,
													DWRITE_FONT_WEIGHT_NORMAL,
													DWRITE_FONT_STYLE_NORMAL,
													DWRITE_FONT_STRETCH_NORMAL,
													30,
													L"ko-kr",
													&m_pd2dStardust);
			hr = m_pWriteFactory->CreateTextFormat(L"PF��Ÿ����Ʈ Bold",
													NULL,
													DWRITE_FONT_WEIGHT_NORMAL,
													DWRITE_FONT_STYLE_NORMAL,
													DWRITE_FONT_STRETCH_NORMAL,
													50,
													L"ko-kr",
													&m_pd2dStardustBig);

		}
	}

	if (SUCCEEDED(hr))
	{
		return true;
	}
	return false;
}
bool WriteFont::SetRenderTarget(IDXGISurface1* pSurface)
{
	//RenderTarget ->  Direct2D�� ����� ���ؼ� �� �������̽� ���,ID2D1Resource �� ��ӵǾ� ����

	UINT dpi = GetDpiForWindow(g_hwnd);

	//������ �ɼ� ���� - D2D1_RENDER_TARGET_PROPERTIES ����ü-----------------------------------
	D2D1_RENDER_TARGET_PROPERTIES rtp;
	ZeroMemory(&rtp, sizeof(D2D1_RENDER_TARGET_PROPERTIES));

	//�ϵ��� ��밡�� �� �� �ϵ����� �׷��� ���� ���� �ڵ����� ����Ʈ��� ����Ͽ� ������
	rtp.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;

	rtp.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);

	//ȭ���� Ȯ�� �� ��쿡 ��µ� ��Ʈ���� ������� ������ �����ϱ� ���� ���
	rtp.dpiX = dpi;
	rtp.dpiY = dpi;

	//GDI�� ȣȯ�� ���� Direct3D ��� ��Ʈ���� ������� ���� Ÿ���� ����
	rtp.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	
	//�ϵ���� �������� �ʿ��� �ּ����� Direct3D ��� ������ �����ϴ� ��
	rtp.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	//���� Ÿ�� �� ����
	HRESULT hr = m_pd2dFactory->CreateDxgiSurfaceRenderTarget(pSurface,	&rtp,&m_pd2dRT);
	//--------------------------------------------------------------------------------------------
	if (FAILED(hr))
	{
		return false;
	}
	//Direct2D������ ������ä��� �������� Brush �� ����ؾ� ��
	if (FAILED(m_pd2dRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pd2dColorBrush)))
	{
		return false;
	}
	return true;
}
bool WriteFont::Frame()
{
	return true;
}
void WriteFont::Draw(std::wstring msg, RECT rt, D2D1::ColorF color,IDWriteTextFormat* tf)
{
	m_pd2dRT->BeginDraw();// RenderTarget �� ����Ѵ�.

	//D2D_RECT_F ����ü -> �� �������� ���� ���ǵ� �簢��
	D2D_RECT_F fRT;
	fRT.top = rt.top;
	fRT.left = rt.left;
	fRT.right = rt.right;
	fRT.bottom = rt.bottom;

	m_pd2dColorBrush->SetColor(color);
	if (tf == nullptr)
	{
		//��� ����
		m_pd2dStardust->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		//�۾� �׸���                               ������ ��Ʈ
		m_pd2dRT->DrawText(msg.c_str(), msg.size(), m_pd2dStardust, &fRT, m_pd2dColorBrush);
	}
	else
	{
		m_pd2dRT->DrawText(msg.c_str(), msg.size(), tf, &fRT, m_pd2dColorBrush);
	}
	m_pd2dRT->EndDraw();// RenderTarget �� ������. �� �Լ��� ȣ��Ǹ� ȭ�鿡 ��µȴ�.
}
bool WriteFont::Render()
{
	return true;
}
bool WriteFont::Release()
{
	if (m_pd2dMTShadowTF)m_pd2dMTShadowTF->Release();
	if (m_pd2dTextFormat)m_pd2dTextFormat->Release(); 
	if (m_pd2dTextFormat)m_pd2dStardust->Release();
	if (m_pd2dColorBrush)m_pd2dColorBrush->Release();
	if (m_pd2dRT)m_pd2dRT->Release();
	if (m_pWriteFactory)m_pWriteFactory->Release();
	if (m_pd2dFactory)m_pd2dFactory->Release();
	return true;
}