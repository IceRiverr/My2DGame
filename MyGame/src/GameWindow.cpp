
#include "GameWindow.h"
#include "Engine.h"
#include <iostream>

CGameWindow::CGameWindow(std::string title, int w, int h)
	: m_WndHnd(nullptr)
	, m_Title(title)
	, m_nWidth(w)
	, m_nHeight(h)
{

}

CGameWindow::~CGameWindow()
{
	Close();
}

int CGameWindow::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_WndHnd = glfwCreateWindow(m_nWidth, m_nHeight, m_Title.c_str(), nullptr, nullptr);
	if (m_WndHnd == nullptr)
	{
		std::cout << "ERROR: Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(m_WndHnd);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR: Failed to initialize GLAD." << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(m_WndHnd, framebuffer_size_callback);

	return 0;
}

void CGameWindow::Close()
{
	if (m_WndHnd)
	{
		glfwDestroyWindow(m_WndHnd);
		m_WndHnd = nullptr;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	CEngine::GetEngine()->Resize(width, height);
}

