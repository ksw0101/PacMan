#pragma once
#include "Core.h"
#include "AsyncSelect.h"
#include "IntroWorld.h"
#include "GameWorld.h"
#include "UIObject.h"
#include "UIModelMgr.h"
#include "WriteFont.h"
#include "Timer.h"
class Sample : public Core
{
public:
	IntroWorld		m_IntroWorld;
	GameWorld		m_GameWorld;
	float			m_colortime = 0;
	float			m_checktime = 0;

	AsyncSelect m_net;
	HWND		m_edit;
	HWND		m_button;
	HWND		m_listbox;
public:
	virtual void CreateResizeDevice(UINT iWidth, UINT iHeight) override;
	virtual void DeleteResizeDevice(UINT iWidth, UINT iHeight) override;
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
public:
	Sample();
	~Sample();
};
