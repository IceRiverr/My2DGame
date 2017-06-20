#include "Engine.h"
#include <iostream>

CEngine* CEngine::m_sEngine = nullptr;

CEngine::CEngine()
{
	m_sEngine = this;
	
	m_gResourceFactory = nullptr;
	m_gSceneMgr = nullptr;
	m_gFileSys = nullptr;
	m_gMainCamera = nullptr;
	m_gFontLib = nullptr;

	m_gMainWindow = nullptr;
}

CEngine::~CEngine()
{
	DELETE_PTR(m_gFileSys);
	DELETE_PTR(m_gMainCamera);
	
	DELETE_PTR(m_gSceneMgr);
	DELETE_PTR(m_gResourceFactory);

	DELETE_PTR(m_gFontLib);
	DELETE_PTR(m_gMainWindow);
}

int CEngine::Init(CGameWindow* win)
{
	DELETE_PTR(m_gMainWindow);
	m_gMainWindow = win;

	m_gResourceFactory = new CResourceFactory();

	m_gSceneMgr = new CSceneManager();

	m_gFileSys = new CFileSystem();
	m_gFileSys->SetBaseDirectoryName("MyGame");

	CBuildInResource::Regsiter();

	m_gMainCamera = new CCamera(0.0f, (float)m_gMainWindow->m_nWidth, 0.0f, (float)m_gMainWindow->m_nHeight);

	m_gFontLib = new CFontLib();
	m_gFontLib->Init();

	return 0;
}

void CEngine::Update(float dt)
{
	m_gMainCamera->Update(dt);
	m_gSceneMgr->Update(dt);
}

void CEngine::Draw()
{
	

	m_gSceneMgr->Draw();
	m_gFontLib->Draw();
}

void CEngine::Resize(int width, int height)
{
	glViewport(0, 0, m_gMainWindow->m_nWidth, m_gMainWindow->m_nHeight);
}

void CEngine::ProcessEvent()
{
	if (glfwGetKey(m_gMainWindow->m_WndHnd, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_gMainWindow->m_WndHnd, true);

	m_gSceneMgr->ProcessEvent(m_gMainWindow->m_WndHnd);
}

void CEngine::SetMainCamera(CCamera* camera)
{
	DELETE_PTR(m_gMainCamera);
	m_gMainCamera = camera;
}

const std::string& GetBaseDirectory()
{
	return CEngine::GetEngine()->m_gFileSys->GetBaseDirectory();
}

CResourceFactory * GetResourceFactory()
{
	return CEngine::GetEngine()->m_gResourceFactory;
}

void AddGameObject(IGameObject * pObj)
{
	CEngine::GetEngine()->m_gSceneMgr->AddGameObject(pObj);
}

const CCamera* GetSceneCamera()
{
	return CEngine::GetEngine()->m_gMainCamera;
}

const CGameWindow* GetMainWindow()
{
	return CEngine::GetEngine()->m_gMainWindow;
}
