
#include <iostream>
#include <math.h>
#include <random>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "TypeDefine.h"
#include "Effect.h"
#include "Engine.h"
#include "Shape.h"
#include "GameObject.h"
#include "SmartPointer.h"
#include "Physics2DLib.h"
#include "BB_Sprites.h"
#include "GameWindow.h"
#include <sstream>

CEngine* g_pEngine = nullptr;

CScene* g_pScene = nullptr;

void Test_Other();

void InitScene();

int main()
{
	CGameWindow* pMainWnd = new CGameWindow("Break Brick", 800, 600);
	pMainWnd->Init();

	g_pEngine = new CEngine();
	g_pEngine->Init(pMainWnd);

	CCamera* pCamera = new CCamera(0.0f, (float)GetMainWindow()->m_nWidth, 0.0f, (float)GetMainWindow()->m_nHeight * 1.0f);
	g_pEngine->SetMainCamera(pCamera);

	g_pScene = new CScene();

	Test_Other();

	InitScene();
	
	double gameTime = glfwGetTime();
	double lastFrameTime = gameTime;
	double deltaTime = 0.0;

	while (!glfwWindowShouldClose(GetMainWindow()->m_WndHnd))
	{
		gameTime = glfwGetTime();
		deltaTime = gameTime - lastFrameTime;
		lastFrameTime = gameTime;

		g_pEngine->ProcessEvent();
		g_pEngine->Update((float)deltaTime);

		float fps = (float)(1.0 / deltaTime);

		std::stringstream ss;
		ss.setf(std::ios::fixed, std::ios::floatfield);
		ss.precision(2);
		ss << "FPS: " << fps;
		CFontLib::DrawTextAt(ss.str(), 2.0f, GetMainWindow()->m_nHeight -18.0f, 16, glm::vec3(1.0f, 0.0f, 0.0f));

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		g_pEngine->Draw();

		glfwPollEvents();
		glfwSwapBuffers(GetMainWindow()->m_WndHnd);
	}

	glfwTerminate();

	DELETE_PTR(g_pEngine);

	return 0;
}

void InitScene()
{
	// Player
	CTexture2D* pSprite = GetResourceFactory()->Create<CTexture2D>(RESOURCE_TYPE::RESOURCE_TEXTURE);
	pSprite->Init(GetBaseDirectory() + "resource\\buttonLong_brown_pressed.png");

	CTexture2D* pBallSprite = GetResourceFactory()->Create<CTexture2D>(RESOURCE_TYPE::RESOURCE_TEXTURE);
	pBallSprite->Init(GetBaseDirectory() + "resource\\dotRed.png");

	CTexture2D* pBrickSprite = GetResourceFactory()->Create<CTexture2D>(RESOURCE_TYPE::RESOURCE_TEXTURE);
	pBrickSprite->Init(GetBaseDirectory() + "resource\\elementMetal051.png");

	CBallRacket* pRacket = new CBallRacket();
	AddGameObject(pRacket);
	pRacket->Init();
	pRacket->SetScale(64.0f, 16.0f);
	pRacket->SetBBoxSize(128.0f, 32.0f);
	pRacket->SetSprite(pSprite);
	pRacket->SetPosiiton(400.0f, 0.0f, 0.0f);
	pRacket->SetHeight(50.0f);
	pRacket->SetMoveSpeed(500.0f);
	g_pScene->m_Colliders.push_back(pRacket);

	CLineObject* pLine = new CLineObject();
	AddGameObject(pLine);
	pLine->Init();
	pLine->SetPosiiton(750.0f, 550.0f, 0.0f);
	pLine->SetScale(32.0f);
	pLine->SetShape(CBuildInResource::GetResource<CShape>(CBuildInResource::SHAPE_QUAD_LINE));

	CBall* pBall = new CBall();
	AddGameObject(pBall);
	pBall->Init();
	pBall->SetPosiiton(glm::vec3(400.0f, 50.0f, 0.0f));
	pBall->SetScale(10.0f, 10.0f);
	pBall->SetSceneRef(g_pScene);
	pBall->SetSprite(pBallSprite);

	int numBricks = 20;
	
	glm::vec3 postions[20] = 
	{
		glm::vec3(50.0f, 500.0f, 0.0f),
		glm::vec3(190, 500.0f, 0.0f),
		glm::vec3(430.0f, 500.0f, 0.0f),
		glm::vec3(599.0f, 500.0f, 0.0f),
		glm::vec3(748.0f, 500.0f, 0.0f),
		glm::vec3(20.0f, 400.0f, 0.0f),
		glm::vec3(210.0f, 400.0f, 0.0f),
		glm::vec3(400.0f, 400.0f, 0.0f),
		glm::vec3(600.0f, 400.0f, 0.0f),
		glm::vec3(762.0f, 400.0f, 0.0f),

		glm::vec3(50.0f, 300.0f, 0.0f),
		glm::vec3(170, 300.0f, 0.0f),
		glm::vec3(430.0f, 300.0f, 0.0f),
		glm::vec3(569.0f, 300.0f, 0.0f),
		glm::vec3(718.0f, 300.0f, 0.0f),
		glm::vec3(20.0f, 200.0f, 0.0f),
		glm::vec3(240.0f, 200.0f, 0.0f),
		glm::vec3(410.0f, 200.0f, 0.0f),
		glm::vec3(6150.0f, 200.0f, 0.0f),
		glm::vec3(762.0f, 200.0f, 0.0f)
	};

	for (int i = 0; i < numBricks; ++i)
	{
		CBrick* pBrack = new CBrick();
		AddGameObject(pBrack);
		pBrack->Init();
		pBrack->SetScale(48.0f, 16.0f);
		pBrack->SetBBoxSize(96.0f, 16.0f);
		pBrack->SetPosiiton(postions[i]);
		pBrack->SetSprite(pBrickSprite);
		g_pScene->m_Colliders.push_back(pBrack);
	}
}

void Test_Other()
{
	
	int ret = 0;
}
