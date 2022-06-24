#pragma once
#include "BaseMgr.h"
#include <d3dcompiler.h>
class Shader
{
public:
	int				m_iIndex;
	std::wstring	m_csName;
	std::wstring	m_szMsg;
	TCHAR			m_szBuffer[256];

	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;

	// 대용량 이진 파일들은 DxObject가 Shader를 맴버로 가진다.
	// DxObj에서 CreateInputLayout을 할 때 필요하다.
	// 셰이더의 릴리즈시 이진 파일들을 릴리즈 해주자
	// ErrorMsg는 에러메시지가 발생시 참조카운트가 1증가하므로 에러 메시지 발생시 릴리즈 해주자
	ID3DBlob* m_pVSCodeResult = nullptr;
	ID3DBlob* m_pErrorMsgs = nullptr;
	ID3DBlob* m_pPSCodeResult = nullptr;
public:
	virtual bool    CreateVertexShader(ID3D11Device* pd3dDevice,
		std::wstring filename, std::string entry = "VS");
	virtual bool    CreatePixelShader(ID3D11Device* pd3dDevice,
		std::wstring filename, std::string entry = "PS");
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	bool	Load(ID3D11Device* pd3dDevice, std::wstring filename);
public:
	Shader();
	virtual ~Shader();
};
//-----------------------------------------------------------------------------
class ShaderMgr : public BaseMgr<Shader, ShaderMgr>
{
	friend class Singleton<ShaderMgr>;
public:
	Shader* CreateVertexShader(ID3D11Device* pd3dDevice,std::wstring filename, std::string entry);
	Shader* CreatePixelShader(ID3D11Device* pd3dDevice,	std::wstring filename, std::string entry);
private:
	ShaderMgr();
public:
	~ShaderMgr();
};
#define I_Shader ShaderMgr::Get()