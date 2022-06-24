#include "Window.h"
RECT g_rtClient;
HWND g_hwnd;
Window* g_pWindow = nullptr;
//������ ���ν���, �������� ���� �޼��� ó�� �Լ�
//(1���� - �ڵ� / 2���� - �޼��� / 3���� - �ڵ�,���� �޾Ƶ��϶� / 4���� - �����Ͱ� ����) 
//WM = ������ �޼���
LRESULT  CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    g_pWindow -> MsgProc(hwnd, msg, wparam, lparam);
    switch (msg)
    {
    case WM_SIZE://������ âũ�� ���� ������
    {
        UINT iWidth = LOWORD(lparam);
        UINT iHeight = HIWORD(lparam);
        g_pWindow->ResizeDevice(iWidth, iHeight);
    }break;
    case WM_DESTROY: //WM_DESTORY = â�� ���� �� ���� ���α׷����� â�� �Ҵ��� �ڿ��� �����ϰų� ��ó�� �۾��� �� �� �ְ� �����ϴ� ������ �޽���
    {
        //�޽��� ť�� WM_QUIT �޽����� ����
        PostQuitMessage(0);
    }break;
    default:
        //DefWindowProc = �������� �������� ������ ó��(ex. â�� ũ�� ���, Ȯ�� ��)
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
    return 0;
}
LRESULT  Window::MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    return 0;
}
void Window::ResizeDevice(UINT iWidth, UINT iHeight)
{

}

//1. ������ Ŭ���� ��� / �㰡 (�ü��)
BOOL Window::SetWinClass(HINSTANCE hInstance)
{
    m_hInsatance = hInstance;
    WNDCLASS wc;

    //memset�� ���� ������Ŭ���� �޸� ����
    ZeroMemory(&wc, sizeof(WNDCLASS));

    //CS = Ŭ���� / CS_HREDRAW = �������� �̵��̳� ũ�� ������ Ŭ���̾�Ʈ�� ������ ũ�Ⱑ ����Ǹ� �ٽ� �׸�
    //              CS_VREDRAW = �������� �̵��̳� ũ�� ������ Ŭ���̾�Ʈ�� ������ ũ�Ⱑ ����Ǹ� �ٽ� �׸�
    //��, ���� ���� ũ�� ���� ����
    //wc.style = CS_HREDRAW | CS_VREDRAW;
    //wc.style = WS_THICKFRAME;

    //lp = ������ / �Լ� �ּ� / ������ ���ν���
    wc.lpfnWndProc = WndProc;

    //�ü���� �����ϴ� ��ȣ = �ֹε�Ϲ�ȣ
    wc.hInstance = hInstance;

    //sz = ��Ʈ�� / L = LONG / �ü�� ��� �̸� / ������ �̸� ��� �������� ���ƾ���
    wc.lpszClassName = L"GameWindow";

    //Stock = �̸� ��������� ������ �ִ°�
    //������ Ŭ���� ��׶���
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    if (RegisterClass(&wc) == false)
    {
        return FALSE;
    }
    return TRUE;
}
//2. ������ ����
BOOL Window::SetWindow(const WCHAR* szTitle, int iWidth, int iHeight)
{
    //������,����,����
    RECT rt = { 0,0, iWidth , iHeight };

    //������ âũ�� ���� ����
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);

    //(lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)
    m_hWnd = CreateWindow(
        L"GameWindow",
        szTitle,
        WS_OVERLAPPEDWINDOW,
        0, 0,
        rt.right - rt.left,
        rt.bottom - rt.top,
        NULL,
        NULL,
        m_hInsatance,
        NULL);
    if (m_hWnd == NULL)
    {
        return FALSE;
    }
    g_hwnd = m_hWnd;
    //������ Ŭ���̾�Ʈ ���������� ��ǥ�� ��ȯ
    GetClientRect(m_hWnd, &m_rtClient);

    //������ ȭ�� ��ǥ ������ ��ġ ������ ��ȯ
    GetWindowRect(m_hWnd, &m_rtWindow);

    g_rtClient = m_rtClient;

    ShowWindow(m_hWnd, SW_SHOW);
    return TRUE;
}
bool Window::WinRun()
{
    MSG msg;

    //PeekMessage = �޼��� ť�� �޼����� �ִ��� ������ �˻��ϰ�, msg ����ü�� �޼����� �������ִ� �Լ�
    //�޼���ť�� �޼����� ������ ���
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return false;
        }
        TranslateMessage(&msg);// �޼��� ����
        DispatchMessage(&msg);// ���������ν��� ȣ��
    }

    return true;
}
Window:: Window()
{
    g_pWindow = this;
}