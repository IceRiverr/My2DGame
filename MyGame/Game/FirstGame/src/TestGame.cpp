
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
#include <sstream>  
#include "TG_Sprites.h"
#include "GameWindow.h"

CEngine* g_pEngine = nullptr;
CAttackGameRule	rule;
CPlayer* pPlayer = nullptr;

void Test_Other();

void InitScene();

int main()
{
	CGameWindow* pMainWnd = new CGameWindow("Test Game", 800, 600);
	pMainWnd->Init();

	g_pEngine = new CEngine();
	g_pEngine->Init(pMainWnd);

	CCamera* pCamera = new CCamera(-GetMainWindow()->m_nWidth / 2.0f, GetMainWindow()->m_nWidth / 2.0f, -GetMainWindow()->m_nHeight / 2.0f, GetMainWindow()->m_nHeight / 2.0f);
	g_pEngine->SetMainCamera(pCamera);

	InitScene();

	Test_Other();
	
	double gameTime = glfwGetTime();
	double lastFrameTime = gameTime;
	double deltaTime = 0.0;

	std::cout << "Attack 10 box monster." << std::endl;
	
	while (!glfwWindowShouldClose(GetMainWindow()->m_WndHnd))
	{
		if (pPlayer->GetGameRule()->IsWin())
		{
			std::cout << "YOU WIN..." << std::endl;
			break;
		}

		CFontLib::DrawTextAt("Hello, Boy, Good Night! Long long age, a witcher find a evil method to have dead to live...", -350, -250, 16, glm::vec3(0.0f, 1.0f, 0.0f));

		gameTime = glfwGetTime();
		deltaTime = gameTime - lastFrameTime;
		lastFrameTime = gameTime;

		float fps = (float)(1.0 / deltaTime);
		
		std::stringstream ss;
		ss.setf(std::ios::fixed, std::ios::floatfield);
		ss.precision(2);
		ss << "FPS: " << fps;

		CFontLib::DrawTextAt(ss.str(), -380, 280, 16, glm::vec3(0.0f, 0.0f, 0.0f));

		g_pEngine->ProcessEvent();
		g_pEngine->Update((float)deltaTime);

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

void Test_Other()
{
	


	int test = 10;
}

void InitScene()
{
	int nNumEnemies = 10;
	rule.SetNumEnemies(nNumEnemies);

	// Player
	CTexture2D* pSprite = GetResourceFactory()->Create<CTexture2D>(RESOURCE_TYPE::RESOURCE_TEXTURE);
	pSprite->Init(GetBaseDirectory() + "resource\\playerShip1_orange.png");//playerShip1_orange.png  strike.jpg

	pPlayer = new CPlayer();
	AddGameObject(pPlayer);
	pPlayer->Init();
	pPlayer->SetScale(32.0f, 32.0f);
	pPlayer->SetShape(CBuildInResource::GetResource<CShape>(CBuildInResource::SHAPE_SPRITE));
	pPlayer->SetSprite(pSprite);
	pPlayer->SetPosiiton(0.0f, 0.0f, 1.0f);
	pPlayer-> SetGameRule(&rule);
	
	std::default_random_engine random_e;
	std::uniform_real_distribution<double> random(-1.0, 1.0);
	for (int i = 0; i < nNumEnemies; ++i)
	{
		CBoxEnemy* pBox = new CBoxEnemy();
		AddGameObject(pBox);

		float x = (float)random(random_e) * 400.0f;
		float y = (float)random(random_e) * 300.0f;

		pBox->Init();
		pBox->SetPosiiton(x, y, 0.0f);
		pBox->SetScale(16.0f, 16.0f);
		pBox->SetTarget(pPlayer);
	}

	CLineObject* pLine = new CLineObject();
	AddGameObject(pLine);
	pLine->Init();
	pLine->SetPosiiton(100.0f, 100.0f, 0.0f);
	pLine->SetScale(32.0f, 32.0f);
	pLine->SetShape(CBuildInResource::GetResource<CShape>(CBuildInResource::SHAPE_QUAD_LINE));

	CLineObject* pCircle = new CLineObject();
	AddGameObject(pCircle);
	pCircle->Init();
	pCircle->SetPosiiton(-100.0f, 100.0f, 0.0f);
	pCircle->SetScale(32.0f, 32.0f);
	pCircle->SetShape(CBuildInResource::GetResource<CShape>(CBuildInResource::SHAPE_CIRCLE_LINE));

	CSolidColorObject* pFillCircle = new CSolidColorObject();
	AddGameObject(pFillCircle);
	pFillCircle->Init();
	pFillCircle->SetPosiiton(-100.0f, 0.0f, 0.0f);
	pFillCircle->SetScale(32.0f, 32.0f);
	pFillCircle->SetShape(CBuildInResource::GetResource<CShape>(CBuildInResource::SHAPE_CIRCLE));
	pFillCircle->SetColor(glm::vec4(0.2f, 0.5f, 0.1f, 1.0f));

	// CTexture2D
	
	//wchar_t* chinese_str = L"·±";
	//CCharTexture* pCharTexture = GetResourceFactory()->Create<CCharTexture>(RESOURCE_TYPE::RESOURCE_TEXTURE);
	//pCharTexture->Init(chinese_str[0]);
}
