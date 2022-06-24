#include "IntroWorld.h"
#include "Input.h"
#include <string>
#include "ObjectMgr.h"
#include "UIModelMgr.h"
extern std::shared_ptr<ButtonObject> btnObj1(new ButtonObject);

UIModel* pNewBtnSetClone;
Object2D* pNewbtn1;

bool IntroWorld::CreateModelType()
{
	/// 배경이미지---------------------------------------------------------------
	Shader* pVShader = I_Shader.CreateVertexShader(m_pd3dDevice, L"Shader.hlsl", "VS");
	Shader* pPShader = I_Shader.CreatePixelShader(m_pd3dDevice, L"Shader.hlsl", "PSAlphaBlend");
	Shader* pPShader2 = I_Shader.CreatePixelShader(m_pd3dDevice, L"Shader.hlsl", "PS");

	std::shared_ptr<ImageObject> obj(new ImageObject);
	obj->m_csName = L"ImageObject:BG";
	obj->Init();
	obj->SetRectDraw({ 0,0, g_rtClient.right,g_rtClient.bottom });
	obj->SetPosition(Vector2(0, 0));
	obj->m_pColorTex = m_BGTex;
	obj->m_pMaskTex = nullptr;
	obj->m_pVShader = pVShader;
	obj->m_pPShader = pPShader;
	if (!obj->Create(m_pd3dDevice, m_pContext))
	{
		return false;
	}
	obj->SetCollisionType(CollisionType::Ignore, SelectType::Select_Ignore);
	I_UI.m_list.insert(std::make_pair(L"bg", obj));

	/// 로고  --------------------------------------------------------------
	std::shared_ptr<ImageObject> obj1(new ImageObject);
	obj1->m_csName = L"ImageObject:LOGO";
	obj1->Init();
	obj1->SetRectDraw({ 0,0, 700,250 });
	obj1->SetPosition(Vector2(0, 0));
	obj1->m_pColorTex = m_LOGOTex;
	obj1->m_pMaskTex = nullptr;
	obj1->m_pVShader = pVShader;
	obj1->m_pPShader = pPShader2;
	if (!obj1->Create(m_pd3dDevice, m_pContext))
	{
		return false;
	}
	obj1->SetCollisionType(CollisionType::Ignore, SelectType::Select_Ignore);
	I_UI.m_list.insert(std::make_pair(L"logo", obj1));

	/// 로고2  -------------------------------------------------------------
	std::shared_ptr<ImageObject> obj2(new ImageObject);
	obj2->m_csName = L"ImageObject:LOGO2";
	obj2->Init();
	obj2->SetRectDraw({ 0,0, 670,200 });
	obj2->SetPosition(Vector2(0, 0));
	obj2->m_pColorTex = m_LOGOTex2;
	obj2->m_pMaskTex = nullptr;
	obj2->m_pVShader = pVShader;
	obj2->m_pPShader = pPShader;
	if (!obj2->Create(m_pd3dDevice, m_pContext))
	{
		return false;
	}
	obj2->SetCollisionType(CollisionType::Ignore, SelectType::Select_Ignore);
	I_UI.m_list.insert(std::make_pair(L"logo2", obj2));
	/// 버튼1-게임시작---------------------------------------------------------------
	
	btnObj1->m_csName = L"TButtonObject:btnGameStart";
	btnObj1->m_index = 1;
	btnObj1->Init();
	btnObj1->m_rtOffset = { 0, 0, 0, 0 };
	btnObj1->SetRectDraw({ 0,0, 300,100 });
	btnObj1->SetPosition(Vector2(0,0));

	Texture* pTex1 = I_Texture.Load(L"../../DX2D/data/1P_Away.png");
	Sound* pSound1 = I_Sound.Load("../../DX2D/data/eff.wav");
	btnObj1->m_pStatePlayList.emplace_back(pTex1, pSound1);

	pTex1 = I_Texture.Load(L"../../DX2D/data/1P_Hover.png");
	pSound1 = I_Sound.Load("../../DX2D/data/eff.wav");
	btnObj1->m_pStatePlayList.emplace_back(pTex1, pSound1);

	pTex1 = I_Texture.Load(L"../../DX2D/data/1P_Away.png");
	pSound1 = I_Sound.Load("../../DX2D/data/eff.wav");
	btnObj1->m_pStatePlayList.emplace_back(pTex1, pSound1);

	pTex1 = I_Texture.Load(L"../../DX2D/data/1P_Away.png");
	pSound1 = I_Sound.Load("../../DX2D/data/eff.wav");
	btnObj1->m_pStatePlayList.emplace_back(pTex1, pSound1);

	if (!btnObj1->Create(m_pd3dDevice, m_pContext ,
		L"DefaultUI.txt",
		L"../../DX2D/data/1P_Away.png"))
	{
		return false;
	}
	btnObj1->SetCollisionType(CollisionType::Ignore, SelectType::Select_Overlap);
	I_UI.m_list.insert(std::make_pair(L"btnGameStart", btnObj1));


	std::shared_ptr<UIModelComposed> pComposedObj1(new UIModelComposed);
	pComposedObj1->m_csName = L"TUIModelComposedBtn";


	UIModel* pBtn01 = I_UI.GetPtr(L"btnGameStart")->Clone();
	pBtn01->m_csName = L"btnGameStartClone_ComposedList";
	pBtn01->m_pParent = nullptr;
	pBtn01->SetRectDraw({ 700,150, 340,100 });
	pBtn01->SetPosition(Vector2(700, 150));
	pComposedObj1->Add(pBtn01);



	I_UI.m_list.insert(std::make_pair(L"btnset", pComposedObj1));
	//----------------------------------------------------------------------------------------
	
	return true;
}
bool IntroWorld::Init()
{
	World::Init();
	//m_pBackGroundMusic->Play(true);
	return true;
}
bool IntroWorld::Load(std::wstring file)
{
	m_BGTex = I_Texture.Load(L"../../DX2D/data/GameBG.png");
	m_LOGOTex = I_Texture.Load(L"../../DX2D/data/pacman.png");
	m_LOGOTex2 = I_Texture.Load(L"../../DX2D/data/namco.png");

	CreateModelType();

	Object2D* pNewBK =I_UI.GetPtr(L"bg")->Clone();
	pNewBK->m_csName = L"TImageObjectClock:bk";
	pNewBK->SetPosition(Vector2(400	, 400));
	pNewBK->UpdateData();
	//pNewBK->m_bFadeIn = true;
	m_UIObj.push_back(std::shared_ptr<Object2D>(pNewBK));

	Object2D* pNewLOGO = I_UI.GetPtr(L"logo")->Clone();
	pNewLOGO->m_csName = L"TImageObjectClock:logo";
	pNewLOGO->SetPosition(Vector2(420, 250));
	pNewLOGO->UpdateData();
	m_UIObj.push_back(std::shared_ptr<Object2D>(pNewLOGO));

	Object2D* pNewLOGO2 = I_UI.GetPtr(L"logo2")->Clone();
	pNewLOGO2->m_csName = L"TImageObjectClock:logo2";
	pNewLOGO2->SetPosition(Vector2(400, 650));
	pNewLOGO2->UpdateData();
	m_UIObj.push_back(std::shared_ptr<Object2D>(pNewLOGO2));
	//========================================================================================
	
	pNewbtn1 = I_UI.GetPtr(L"btnGameStart")->Clone();
	pNewbtn1->m_csName = L"gamestart";
	pNewbtn1->SetPosition(Vector2(400, 450));
	pNewbtn1->UpdateData();
	m_UIObj.push_back(std::shared_ptr<Object2D>(pNewbtn1));



	return true;
}
bool IntroWorld::Frame()
{
	if (m_bLoadZone&&m_pNextWorld != nullptr)//m_bLoadZone
	{
		I_ObjectMgr.Release();
		m_pNextWorld->Load(L"world.txt");

	}
	//m_pBackGroundMusic->Frame();
	if (World::m_pWorld->btngame == true)
	{
		World::m_pWorld = m_pNextWorld;
		World::m_pWorld->btngame = false;
	}


	static int iIndex = 0;

	//m_ExplosionObj.Frame();
	World::Frame();
	return true;
}
bool IntroWorld::Render()
{
	World::Render();
	
	return true;
}
bool IntroWorld::Release()
{
	World::Release();
	
	return true;
}