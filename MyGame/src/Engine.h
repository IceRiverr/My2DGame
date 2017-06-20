#pragma once
#include "Effect.h"
#include "Camera.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "FontLib.h"

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
	void Update(float dt);
	void Draw();
	void Resize(int width, int height);
	void ProcessEvent();

	static CEngine* GetEngine() { return m_sEngine; };

public:
	int					m_gScreenWidth;
	int					m_gScreenHeight;
	GLFWwindow*			m_gWindow;
	CCamera*			m_gCamera;
	CFontLib*			m_gFontLib;

private:
	static CEngine*		m_sEngine;
	
	CResourceFactory*	m_gResourceFactory;
	CSceneManager*		m_gSceneMgr;
	CFileSystem*		m_gFileSys;
};
