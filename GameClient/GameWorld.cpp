#include "GameWorld.h"
#include "Input.h"
#include "ObjectMgr.h"
ObjectNpc2D* plusheart;
ObjectNpc2D* plusscore;
bool GameWorld:: CreateNPC()
{
	Shader* pVShader = I_Shader.CreateVertexShader(World::m_pd3dDevice, L"Shader.hlsl", "VS");
	Shader* pPShader = I_Shader.CreatePixelShader(World::m_pd3dDevice, L"Shader.hlsl", "PSAlphaBlend");
	for (int iNpc = 0; iNpc < 1; iNpc++)
	{
		//std::shared_ptr<ObjectNpc2D> npc = std::make_shared<ObjectNpc2D>();
		ObjectNpc2D* npc = new ObjectNpc2D;
		npc->m_csName = L"NPC";
		//npc->m_csName += std::to_wstring(iNpc);
		npc->Init();

		npc->SetRectDraw({ 0,0, 40,40 });
		npc->SetPosition(Vector2(350, 400));

		npc->m_pColorTex = I_Texture.Load(L"../../DX2D/data/NPC_up1.png");
		npc->m_pMaskTex = nullptr;
		npc->m_pVShader = pVShader;
		npc->m_pPShader = pPShader;

		if (!npc->Create(World::m_pd3dDevice, m_pContext))
		{
			return false;
		}
		npc->SetCollisionType(CollisionType::Overlap, SelectType::Select_Ignore);
		npc->UpdateData();
		//m_NpcObj.push_back(npc);
		m_npclist.push_back(npc);
	}
}

bool GameWorld::CreateModelType()
{
	/// 배경이미지---------------------------------------------------------------
	Shader* pVShader = I_Shader.CreateVertexShader(World::m_pd3dDevice, L"Shader.hlsl", "VS");
	Shader* pPShader = I_Shader.CreatePixelShader(World::m_pd3dDevice, L"Shader.hlsl", "PSAlphaBlend");
	std::shared_ptr<ImageObject> obj(new ImageObject);
	obj->m_csName = L"ImageObject:BG";
	obj->Init();
	obj->SetRectDraw({ 0,0, g_rtClient.right,g_rtClient.bottom });
	obj->SetPosition(Vector2(0, 0));
	obj->m_pColorTex = m_BGTex;
	obj->m_pMaskTex = nullptr;
	obj->m_pVShader = pVShader;
	obj->m_pPShader = pPShader;
	if (!obj->Create(World::m_pd3dDevice, m_pContext))
	{
		return false;
	}
	obj->SetCollisionType(CollisionType::Ignore, SelectType::Select_Ignore);
	I_UI.m_list.insert(std::make_pair(L"bg", obj));
}
bool GameWorld::Init()
{
	return true;
}
bool GameWorld::Load(std::wstring file)
{
	m_BGTex = I_Texture.Load(L"../../DX2D/data/GameBG.png");

	m_pBackGroundMusic = I_Sound.Load("../../DX2D/data/BGM.mp3");

	m_pBackGroundMusic->Play(true);

	CreateModelType();

	//배경 세팅
	Object2D* pNewBK = I_UI.GetPtr(L"bg")->Clone();
	pNewBK->m_csName = L"TImageObjectClock:bk";
	pNewBK->SetPosition(Vector2(400, 400));
	pNewBK->UpdateData();
	m_UIObj.push_back(std::shared_ptr<Object2D>(pNewBK));

	//스테이지
	Shader* pVShader = I_Shader.CreateVertexShader(World::m_pd3dDevice, L"Shader.hlsl", "VS");
	Shader* pPShader = I_Shader.CreatePixelShader(World::m_pd3dDevice, L"Shader.hlsl", "PSAlphaBlend");

	m_Stage.m_csName = L"ImageObject:Stage";
	m_Stage.Init();
	m_Stage.SetRectDraw({ 0,0, 600, 700 });
	m_Stage.SetPosition(Vector2(310, 400));
	m_Stage.m_pColorTex = I_Texture.Load(L"../../DX2D/data/GameStage01.bmp");
	m_Stage.m_pMaskTex = nullptr;
	m_Stage.m_pVShader = pVShader;
	m_Stage.m_pPShader = pPShader;
	if (!m_Stage.Create(m_pd3dDevice, m_pContext))
	{
		return false;
	}
	m_Stage.SetCollisionType(CollisionType::Ignore, SelectType::Select_Ignore);

	WallMgr* mg = new WallMgr;
	m_pWallMgr = mg;
	//
	// 
	// 
	//Life
	m_Life.m_csName = L"Life";
	m_Life.Init();

	m_Life.SetRectDraw({ 0,0, 150,50 });
	m_Life.SetPosition(Vector2(700, 500));

	m_Life.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Life.png");
	m_Life.m_pMaskTex = nullptr;
	m_Life.m_pVShader = pVShader;
	m_Life.m_pPShader = pPShader;

	if (!m_Life.Create(World::m_pd3dDevice, m_pContext))
	{
		return false;
	}
	m_Life.SetCollisionType(CollisionType::Ignore, SelectType::Select_Ignore);

	//플레이어 세팅----------------------------------------------------
	m_PlayerObj.m_csName = L"PlayerUser";
	m_PlayerObj.Init();
	m_PlayerObj.SetPosition(Vector2(300, 450));
	m_PlayerObj.SetRectDraw({ 300, 460, 40,50 });
	m_PlayerObj.SetCollisionType(CollisionType::Overlap, SelectType::Select_Ignore);
	m_PlayerObj.m_pVShader = pVShader;
	m_PlayerObj.m_pPShader = pPShader;
	m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Pacman_stand.png");

	//플레이어 생성
	if (!m_PlayerObj.Create(World::m_pd3dDevice, m_pContext))
	{
		return false;
	}
	m_PlayerObj.UpdateData();
	//m_PlayerObj.m_life = 3;
	CreateNPC();
	//CreatePlusLife();
	I_Sprite.Load(L"SpriteData.txt");
	//World::m_pWorld = m_pNextWorld;
	return true;
}
bool GameWorld::Frame()
{
	m_Life.Frame();
	m_PlayerObj.Frame();
	ChangePlayerTexture();

	if (m_PlayerObj.m_life <= 0)
	{
		// 사망시 작업
	}
	if (m_PlayerObj.m_life == 1) {m_Life.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Life1.png");}
	if (m_PlayerObj.m_life == 2) {m_Life.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Life2.png");}
	if (m_PlayerObj.m_life >= 3) {m_Life.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Life3.png");
								  m_PlayerObj.m_life = 3;}


	m_pBackGroundMusic->Frame();

	if (Input::Get().GetKey(VK_F6) == KEY_HOLD)
	{
		m_pBackGroundMusic->VolumeUp(g_fSecPerFrame);
	}
	if (Input::Get().GetKey(VK_F5) == KEY_HOLD)
	{
		m_pBackGroundMusic->VolumeDown(g_fSecPerFrame);
	}

	World::Frame();

	return true;
}

bool GameWorld::ChangePlayerTexture() 
{
	//플레이어 이동 모션
	static int Lindex = 1;
	if (Input::Get().GetKey(VK_LEFT) == KEY_HOLD)
	{
		if (Lindex == 1)
		{
			m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Pacman_Left1.png");
			Lindex = 2;
		}
		else if (Lindex == 2)
		{
			m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Pacman_Left2.png");
			Lindex = 3;
		}
		else if (Lindex == 3)
		{
			m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Pacman_Left1.png");
			Lindex = 4;
		}
		else if (Lindex == 4)
		{
			m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Pacman_stand.png");
			Lindex = 1;
		}
	}
	static int Rindex = 1;
	if (Input::Get().GetKey(VK_RIGHT) == KEY_HOLD)
	{
		if (Rindex == 1)
		{
			m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Pacman_Right1.png");
			Rindex = 2;
		}
		else if (Rindex == 2)
		{
			m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Pacman_Right2.png");
			Rindex = 3;
		}
		else if (Rindex == 3)
		{
			m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Pacman_Right1.png");
			Rindex = 4;
		}
		else if (Rindex == 4)
		{
			m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Pacman_stand.png");
			Rindex = 1;
		}
	}
	static int Uindex = 1;
	if (Input::Get().GetKey(VK_UP) == KEY_HOLD)
	{
		if (Uindex == 1)
		{
			m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Pacman_Up1.png");
			Uindex = 2;
		}
		else if (Uindex == 2)
		{
			m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Pacman_Up2.png");
			Uindex = 3;
		}
		else if (Uindex == 3)
		{
			m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Pacman_Up1.png");
			Uindex = 4;
		}
		else if (Uindex == 4)
		{
			m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Pacman_stand.png");
			Uindex = 1;
		}
	}
	static int Dindex = 1;
	if (Input::Get().GetKey(VK_DOWN) == KEY_HOLD)
	{
		if (Dindex == 1)
		{
			m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Pacman_Down1.png");
			Dindex = 2;
		}
		else if (Dindex == 2)
		{
			m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Pacman_Down2.png");
			Dindex = 3;
		}
		else if (Dindex == 3)
		{
			m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Pacman_Down1.png");
			Dindex = 4;
		}
		else if (Dindex == 4)
		{
			m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Pacman_stand.png");
			Dindex = 1;
		}
	}
	return true;
}


bool GameWorld::Render()
{

	World::Render();
	m_Stage.Render();
	vector<ObjectNpc2D*>::iterator iter;
	for (iter = m_npclist.begin(); iter != m_npclist.end(); iter++)
	{
		//ObjectNpc2D* pObj = obj;
		if ((*iter) != nullptr)
		{
			(*iter)->Render();
		}
	}
	m_PlayerObj.Render();
	m_Life.Render();
	if(plusheart!=nullptr)plusheart->Render();
	if (plusscore != nullptr)plusscore->Render();

	return true;
}
bool GameWorld::Release()
{
	if (plusheart != nullptr)plusheart->Release();
	if (plusscore != nullptr)plusscore->Release();
	m_PlayerObj.Release();
	m_Life.Release();
	vector<ObjectNpc2D*>::iterator iter;
	for (iter = m_npclist.begin(); iter != m_npclist.end(); iter++)
	{
		//ObjectNpc2D* pObj = obj;
		if ((*iter) != nullptr)
		{
			delete *iter;
			//(*iter)->Release();
			*iter = nullptr;
		}
	}
	return true;
}