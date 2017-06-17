#pragma once
#include "Effect.h"
#include "Camera.h"
#include "FileSystem.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
const std::string& GetBaseDirectory();

class CEngine
{
	friend const std::string& GetBaseDirectory();
public:
	CEngine();
	~CEngine();

	int Init();
	void Update();
	void Resize(int width, int height);
	void ProcessInput(GLFWwindow* window);

	static CEngine* GetEngine() { return m_sEngine; };

public:
	int					m_gScreenWidth;
	int					m_gScreenHeight;
	GLFWwindow*			m_gWindow;
	CShaderManager*		m_gShaderMgr;
	CCamera*			m_gCamera;

private:
	static CEngine*		m_sEngine;

	CFileSystem*		m_gFileSys;
};

