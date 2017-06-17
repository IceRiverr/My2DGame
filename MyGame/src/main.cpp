
#include <iostream>
#include <math.h>

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

void Test_Other();

CEngine* g_pEngine = nullptr;

int main()
{
	g_pEngine = new CEngine();
	g_pEngine->Init();

	CMeshData* pMeshData = GetResourceFactory()->Create<CMeshData>(RESOURCE_TYPE::RESOURCE_MESH_DATA);

	pMeshData->m_nNumVertex = 4;
	pMeshData->m_nNumIndex = 6;
	pMeshData->m_Positions =
	{
		-1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
	};

	pMeshData->m_UVs =
	{
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};

	pMeshData->m_Indices =
	{
		0,1,2,
		0,2,3,
	};

	CShape* pShape = GetResourceFactory()->Create<CShape>(RESOURCE_TYPE::RESOURCE_SHAPE);
	pShape->Init(pMeshData, VERTEX_POSITION_UV);

	CShape* pSolidShape = GetResourceFactory()->Create<CShape>(RESOURCE_TYPE::RESOURCE_SHAPE);
	pSolidShape->Init(pMeshData, VERTEX_POSITION);

	CMeshData* pLineMesh = GetResourceFactory()->Create<CMeshData>(RESOURCE_TYPE::RESOURCE_MESH_DATA);
	pLineMesh->m_nNumVertex = 4;
	pLineMesh->m_nNumIndex = 8;
	pLineMesh->m_Positions =
	{
		-1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
	};
	pLineMesh->m_Indices =
	{
		0, 1, 1, 2,
		2, 3, 3, 0,
	};

	CShape* pLineShape = GetResourceFactory()->Create<CShape>(RESOURCE_TYPE::RESOURCE_SHAPE);
	pLineShape->Init(pLineMesh, VERTEX_POSITION);

	CSpriteEffect* pEffect = GetResourceFactory()->Create<CSpriteEffect>(RESOURCE_TYPE::RESOURCE_EFFECT);
	pEffect->Init();

	CSolidColorEffect* pSolidEffect = GetResourceFactory()->Create<CSolidColorEffect>(RESOURCE_TYPE::RESOURCE_EFFECT);
	pSolidEffect->Init();

	CTexture2D* pSprite = GetResourceFactory()->Create<CTexture2D>(RESOURCE_TYPE::RESOURCE_TEXTURE);
	pSprite->Init(GetBaseDirectory() + "resource\\strike.jpg");
	
	CSpriteObject* pPlayer = new CSpriteObject();
	AddGameObject(pPlayer);
	pPlayer->Init();
	pPlayer->SetScale(32.0f);
	pPlayer->SetShape(pShape);
	pPlayer->SetEffect(pEffect);
	pPlayer->SetSprite(pSprite);

	CSpriteObject* pPlayer1 = new CSpriteObject();
	AddGameObject(pPlayer1);
	pPlayer1->Init();
	pPlayer1->SetPosiiton(glm::vec3(100.0f, 0.0f, 0.0f));
	pPlayer1->SetScale(32.0f);
	pPlayer1->SetShape(pShape);
	pPlayer1->SetEffect(pEffect);
	pPlayer1->SetSprite(pSprite);

	CSolidColorObject* pBox = new CSolidColorObject();
	AddGameObject(pBox);
	pBox->Init();
	pBox->SetPosiiton(glm::vec3(0.0f, 100.0f, 0.0f));
	pBox->SetScale(32.0f);
	pBox->SetShape(pSolidShape);
	pBox->SetEffect(pSolidEffect);

	CLineShape* pLine = new CLineShape();
	AddGameObject(pLine);
	pLine->Init();
	pLine->SetPosiiton(glm::vec3(100.0f, 100.0f, 0.0f));
	pLine->SetScale(32.0f);
	pLine->SetShape(pLineShape);
	pLine->SetEffect(pSolidEffect);

	Test_Other();

	while (!glfwWindowShouldClose(CEngine::GetEngine()->m_gWindow))
	{
		g_pEngine->ProcessInput(CEngine::GetEngine()->m_gWindow);
		pPlayer->ProcessInput(CEngine::GetEngine()->m_gWindow);
		pPlayer1->ProcessInput(CEngine::GetEngine()->m_gWindow);

		g_pEngine->Update();
		pPlayer->Update();

		float time = (float)glfwGetTime();

		pPlayer1->SetRotate(glm::degrees(time) * 2.0f);
		pPlayer1->Update();
		pBox->Update();
		pLine->Update();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// Mesh也没有很好的处理
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode, put in effect // 析构没有处理，引用关系没有处理

		pPlayer->Draw();
		pPlayer1->Draw();
		pBox->Draw();
		pLine->Draw();

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


