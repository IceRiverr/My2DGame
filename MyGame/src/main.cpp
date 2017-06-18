
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
#include "GamePlayObject.h"

CEngine* g_pEngine = nullptr;

void Test_Other();

void InitScene();

int main()
{
	g_pEngine = new CEngine();
	g_pEngine->Init();

	InitScene();

	Test_Other();

	while (!glfwWindowShouldClose(CEngine::GetEngine()->m_gWindow))
	{
		g_pEngine->ProcessEvent();
		g_pEngine->Update();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		g_pEngine->Draw();

		glfwPollEvents();
		glfwSwapBuffers(CEngine::GetEngine()->m_gWindow);
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
	// Player

	CTexture2D* pSprite = GetResourceFactory()->Create<CTexture2D>(RESOURCE_TYPE::RESOURCE_TEXTURE);
	pSprite->Init(GetBaseDirectory() + "resource\\strike.jpg");

	CPlayer* pPlayer = new CPlayer();
	AddGameObject(pPlayer);
	pPlayer->Init();
	pPlayer->SetScale(32.0f);
	pPlayer->SetShape(CBuildInResource::GetResource<CShape>(CBuildInResource::SHAPE_SPRITE));
	pPlayer->SetSprite(pSprite);
	pPlayer->SetPosiiton(0.0f, 0.0f, 1.0f);

	int nNumEnemies = 10;
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
		pBox->SetScale(16.0f);
		pBox->SetTarget(pPlayer);
	}

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
	pFillCircle->SetPosiiton(-100.0f, 0.0f, 0.0f);
	pFillCircle->SetScale(32.0f);
	pFillCircle->SetShape(CBuildInResource::GetResource<CShape>(CBuildInResource::SHAPE_CIRCLE));
	pFillCircle->SetColor(glm::vec4(0.2f, 0.5f, 0.1f, 1.0f));
}
