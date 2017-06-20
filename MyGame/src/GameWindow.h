
#pragma once

#include <string>
#include <glad\glad.h>
#include <GLFW\glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

class CGameWindow
{
public:
	CGameWindow(std::string title, int w, int h);
	~CGameWindow();
	
	int Init();
	void Close();

	GLFWwindow*		m_WndHnd;
	int				m_nWidth;
	int				m_nHeight;

private:
	std::string		m_Title;
};
