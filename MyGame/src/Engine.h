#pragma once
#include "Effect.h"
#include "Camera.h"
#include "FileSystem.h"
#include "GameObject.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const std::string& GetBaseDirectory();
CResourceFactory* GetResourceFactory();
void AddGameObject(IGameObject* pObj);

class CEngine
{
	friend const std::string& GetBaseDirectory();
	friend CResourceFactory* GetResourceFactory();
	friend void AddGameObject(IGameObject* pObj);
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
	CResourceFactory*	m_gResourceFactory;
	CGameObjectManager*	m_gGameObjeectMgr;
};

