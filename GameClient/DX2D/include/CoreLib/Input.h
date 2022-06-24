#pragma once
#include "Std.h"
enum KeyState
{
	KEY_FREE = 0,//Ű �ȴ���
	KEY_UP,//�������� Ű���� �� ���� ����
	KEY_PUSH,//�������� ����
	KEY_HOLD,//�����ִ� ����
};
class Input
{
public:
	DWORD m_dwKeyState[256]; //Ű���� ���� �迭
	POINT m_ptMouse;
	DWORD m_dwMouseState[3];

	//Ŭ������ private�Ǿ� �ֱ⿡ ����ϱ� ���ؼ��� �� �Լ��� �̿�
	//static���� �س��⿡ �� �Ѱ��� �����ȴ�!
	static Input& Get()
	{
		static Input theSingle;
		return theSingle;
	}
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	DWORD   GetKey(DWORD dwKey);
private:
	Input();//�Է��� �������� ���� �ʿ䰡 ���⿡ �ٸ� �������� ��� ���ϰ� ����!
public:
	virtual ~Input();
};

