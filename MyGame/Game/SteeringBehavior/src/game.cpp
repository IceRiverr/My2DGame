
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
#include "Entity.h"
#include "GameWindow.h"

void InitScene();

CEngine* g_pEngine = nullptr;
Drawable* pLine1 = nullptr;
GameWorld* world = nullptr;

int main()
{
	CGameWindow* pMainWnd = new CGameWindow("Steering Behavior", 800, 600);
	pMainWnd->Init();

	g_pEngine = new CEngine();
	g_pEngine->Init(pMainWnd);

	CCamera* pCamera = new CCamera(0.0f, (float)GetMainWindow()->m_nWidth,0.0f, (float)GetMainWindow()->m_nHeight);
	g_pEngine->SetMainCamera(pCamera);

	world = new GameWorld();

	InitScene();

	// Test



	// Test End


	double gameTime = glfwGetTime();
	double lastFrameTime = gameTime;
	double deltaTime = 0.0;
	while (!glfwWindowShouldClose(GetMainWindow()->m_WndHnd))
	{
		CFontLib::DrawTextAt("Hello, Boy, Good Night!", 4.0f, 10.0f, 16);

		gameTime = glfwGetTime();
		deltaTime = gameTime - lastFrameTime;
		lastFrameTime = gameTime;

		float fps = (float)(1.0 / deltaTime);
		
		std::stringstream ss;
		ss.setf(std::ios::fixed, std::ios::floatfield);
		ss.precision(2);
		ss << "FPS: " << fps;

		CFontLib::DrawTextAt(ss.str(), 4.0f, 580.0f, 16, glm::vec3(1.0f, 0.0f, 0.0f));

		g_pEngine->ProcessEvent();
		g_pEngine->Update((float)deltaTime);
		// Test update
		world->Update((float)deltaTime);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		g_pEngine->Draw();
		// Test Draw
		world->Draw();

		glfwPollEvents();
		glfwSwapBuffers(GetMainWindow()->m_WndHnd);
	}

	glfwTerminate();

	DELETE_PTR(g_pEngine);

	return 0;
}

void InitScene()
{
	int nNumEnemies = 10;

	// Player
	CTexture2D* pSprite = GetResourceFactory()->Create<CTexture2D>(RESOURCE_TYPE::RESOURCE_TEXTURE);
	pSprite->Init(GetBaseDirectory() + "resource\\playerShip1_orange.png");//playerShip1_orange.png  strike.jpg

	CMeshData* pMesh1 = GetResourceFactory()->Create<CMeshData>(RESOURCE_TYPE::RESOURCE_MESH_DATA);
	pMesh1->m_nNumVertex = 3;
	pMesh1->m_nNumIndex = 6;
	pMesh1->m_Positions =
	{
		-0.5f, -0.866f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.5f, -0.866f, 0.0f,
	};
	pMesh1->m_Indices =
	{
		0,1, 1, 2, 2, 0
	};

	CShape* pShape = GetResourceFactory()->Create<CShape>(RESOURCE_TYPE::RESOURCE_SHAPE);
	pShape->Init(pMesh1, VERTEX_INDEX_POSITION);

	CLineObject* pLine = new CLineObject();
	AddGameObject(pLine);
	pLine->Init();
	pLine->SetPosiiton(780.0f, 580.0f, 0.0f);
	pLine->SetScale(20.0f, 20.0f);
	pLine->SetShape(CBuildInResource::GetResource<CShape>(CBuildInResource::SHAPE_QUAD_LINE));

	CLineObject* pCircle = new CLineObject();
	AddGameObject(pCircle);
	pCircle->Init();
	pCircle->SetPosiiton(730.0f, 580.0f, 0.0f);
	pCircle->SetScale(20.0f, 20.0f);
	pCircle->SetShape(CBuildInResource::GetResource<CShape>(CBuildInResource::SHAPE_CIRCLE_LINE));

	Vehicle* pV = new Vehicle();
	pV->Init(pShape);
	pV->SetPosition(400.0f, 300.0f);
	world->AddEntity(pV);

	Vehicle* pV1 = new Vehicle();
	pV1->Init(pShape);
	pV1->SetPosition(500.0f, 10.0f);
	world->AddEntity(pV1);


}
