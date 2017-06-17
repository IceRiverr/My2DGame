#include "Engine.h"
#include <iostream>

CEngine* CEngine::m_sEngine = nullptr;

CEngine::CEngine()
{
	m_gWindow = nullptr;
	m_gShaderMgr = nullptr;
	m_gCamera = nullptr;

	m_gScreenWidth = 800;
	m_gScreenHeight = 600;
}

CEngine::~CEngine()
{
	if (m_gShaderMgr)
	{
		delete m_gShaderMgr;
		m_gShaderMgr = nullptr;
	}
	if (m_gCamera)
	{
		delete m_gCamera;
		m_gCamera = nullptr;
	}
}

int CEngine::Init()
{
	m_sEngine = this;

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

	m_gShaderMgr = new CShaderManager();
	m_gShaderMgr->Init();

	m_gCamera = new CCamera(800, 600);

	return 0;
}

void CEngine::Update()
{
	m_gCamera->Update();
}

void CEngine::Resize(int width, int height)
{
	glViewport(0, 0, m_gScreenWidth, m_gScreenHeight);
}

void CEngine::ProcessInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	CEngine::GetEngine()->Resize(width, height);
}
