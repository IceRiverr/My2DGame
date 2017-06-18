#include "Engine.h"
#include <iostream>

CEngine* CEngine::m_sEngine = nullptr;

CEngine::CEngine()
{
	m_sEngine = this;

	m_gWindow = nullptr;
	m_gResourceFactory = nullptr;
	m_gSceneMgr = nullptr;
	m_gFileSys = nullptr;
	m_gCamera = nullptr;

	m_gScreenWidth = 800;
	m_gScreenHeight = 600;
}

CEngine::~CEngine()
{
	DELETE_PTR(m_gFileSys);
	DELETE_PTR(m_gCamera);
	
	DELETE_PTR(m_gSceneMgr);
	DELETE_PTR(m_gResourceFactory);

	glfwDestroyWindow(m_gWindow);
	m_gWindow = nullptr;
}

int CEngine::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_gWindow = glfwCreateWindow(m_gScreenWidth, m_gScreenHeight, "Attack", nullptr, nullptr);
	if (m_gWindow == nullptr)
	{
		std::cout << "ERROR: Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(m_gWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR: Failed to initialize GLAD." << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(m_gWindow, framebuffer_size_callback);

	m_gResourceFactory = new CResourceFactory();

	m_gSceneMgr = new CSceneManager();

	m_gFileSys = new CFileSystem();
	m_gFileSys->SetBaseDirectoryName("MyGame");

	CBuildInResource::Regsiter();

	m_gCamera = new CCamera(800, 600);

	return 0;
}

void CEngine::Update(float dt)
{
	m_gCamera->Update(dt);
	m_gSceneMgr->Update(dt);
}

void CEngine::Draw()
{
	m_gSceneMgr->Draw();
}

void CEngine::Resize(int width, int height)
{
	glViewport(0, 0, m_gScreenWidth, m_gScreenHeight);
}

void CEngine::ProcessEvent()
{
	if (glfwGetKey(m_gWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_gWindow, true);

	m_gSceneMgr->ProcessEvent(m_gWindow);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	CEngine::GetEngine()->Resize(width, height);
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
