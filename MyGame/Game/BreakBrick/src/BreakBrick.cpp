
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

void Test_Other();

void InitScene();

int main()
{
	CGameWindow* pMainWnd = new CGameWindow("Break Brick", 800, 600);
	pMainWnd->Init();

	g_pEngine = new CEngine();
	g_pEngine->Init(pMainWnd);

	CCamera* pCamera = new CCamera(-GetMainWindow()->m_nWidth / 2.0f, GetMainWindow()->m_nWidth / 2.0f, 0.0f, GetMainWindow()->m_nHeight * 1.0f);
	g_pEngine->SetMainCamera(pCamera);

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
		CFontLib::DrawTextAt(ss.str(), -GetMainWindow()->m_nWidth / 2.0f + 2.0f, GetMainWindow()->m_nHeight -18.0f, 16, glm::vec3(1.0f, 0.0f, 0.0f));

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
	pSprite->Init(GetBaseDirectory() + "resource\\buttonLong_brown_pressed.png"); // pad.png

	CBallRacket* pRacket = new CBallRacket();
	AddGameObject(pRacket);
	pRacket->Init();
	pRacket->SetScale(64.0f, 16.0f);
	pRacket->SetBoundingBox(64.0f, 16.0f);
	pRacket->SetSprite(pSprite);
	pRacket->SetHeight(32.0f);
	pRacket->SetMoveSpeed(500.0f);

	CLineObject* pLine = new CLineObject();
	AddGameObject(pLine);
	pLine->Init();
	pLine->SetPosiiton(100.0f, 100.0f, 0.0f);
	pLine->SetScale(32.0f);
	pLine->SetShape(CBuildInResource::GetResource<CShape>(CBuildInResource::SHAPE_QUAD_LINE));

	CLineObject* pCircle = new CLineObject();
	AddGameObject(pCircle);
	pCircle->Init();
	pCircle->SetPosiiton(-100.0f, 100.0f, 0.0f);
	pCircle->SetScale(32.0f);
	pCircle->SetShape(CBuildInResource::GetResource<CShape>(CBuildInResource::SHAPE_CIRCLE_LINE));

	CSolidColorObject* pFillCircle = new CSolidColorObject();
	AddGameObject(pFillCircle);
	pFillCircle->Init();
	pFillCircle->SetPosiiton(-100.0f, 200.0f, 0.0f);
	pFillCircle->SetScale(32.0f);
	pFillCircle->SetShape(CBuildInResource::GetResource<CShape>(CBuildInResource::SHAPE_CIRCLE));
	pFillCircle->SetColor(glm::vec4(0.2f, 0.5f, 0.1f, 1.0f));

	// CTexture2D 
}
