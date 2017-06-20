#pragma once
#include "Effect.h"
#include "Camera.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "FontLib.h"
#include "GameWindow.h"

class CGameWindow;

const std::string&	GetBaseDirectory();
CResourceFactory*	GetResourceFactory();
const CCamera*		GetSceneCamera();
const CGameWindow*	GetMainWindow();
void				AddGameObject(IGameObject* pObj);

class CEngine
{
	friend const std::string&	GetBaseDirectory();
	friend CResourceFactory*	GetResourceFactory();
	friend const CCamera*		GetSceneCamera();
	friend const CGameWindow*	GetMainWindow();
	friend void					AddGameObject(IGameObject* pObj);
public:
	CEngine();
	~CEngine();

	int Init(CGameWindow* win);
	void Update(float dt);
	void Draw();
	void Resize(int width, int height);
	void ProcessEvent();

	static CEngine* GetEngine() { return m_sEngine; };
	void SetMainCamera(CCamera* camera);

public:
	CGameWindow*		m_gMainWindow;
	CFontLib*			m_gFontLib;

private:
	static CEngine*		m_sEngine;
	
	CResourceFactory*	m_gResourceFactory;
	CSceneManager*		m_gSceneMgr;
	CFileSystem*		m_gFileSys;
	CCamera*			m_gMainCamera;
};
