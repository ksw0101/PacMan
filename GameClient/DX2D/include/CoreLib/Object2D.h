#pragma once
#include "DxObject.h"
class Object2D : public DxObject
{
public:
	RECT m_rtSource;//�ҽ� �̹����� u,v ��ǥ/�ؽ���
	RECT m_rtDraw;//ȭ���� ��ο� ����
	Vector4	m_vColor;
public:
	float m_fAlpha = 0.0f;
	bool m_bFadeIn = false;
	bool m_bFadeOut = false;
	virtual void FadeIn();
	virtual void FadeOut();

	//void HitOverlap(BaseObject* pObj, DWORD dwState) override
	//{
	//	this->m_bSelect = true;
	//}

	virtual void SetRectSouce(RECT rt);
	virtual void SetRectDraw(RECT rt);
	virtual void AddPosition(Vector2 vPos);//������ġ�� �������� �����̵ȴ�
	virtual void SetPosition(Vector2 vPos);//�����Ǽ���
	virtual void UpdateRectDraw(RECT rt);//Ŀ�����۾�����
	// ȭ����ǥ ��ġ�� �������� NDC ��ȯ
	virtual void	Convert(Vector2 center, float fWidth, float fHeight,std::vector<SimpleVertex>& retList);
	// ȭ����ǥ�踦 NDC ��ȯ
	virtual void	Convert(std::vector<SimpleVertex>& list, std::vector<SimpleVertex>& retList);
	
	virtual void	ConvertIndex(Vector2 center, float fWidth, float fHeight,
								 std::vector<SimpleVertex>& retList);
	// ȭ����ǥ�踦 NDC ��ȯ
	virtual void	ConvertIndex(std::vector<SimpleVertex>& list,
								 std::vector<SimpleVertex>& retList);

	virtual bool SetVertexData() override;
	virtual bool SetIndexData() override;
	virtual bool Frame() override;

	bool Load(ID3D11Device* pd3dDevice, std::wstring filename)
	{
		return true;
	};
	virtual void UpdateData() {}
	Object2D();
	virtual ~Object2D();
};

