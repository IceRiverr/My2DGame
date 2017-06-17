
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
	
	CGameObject* pPlayer = new CGameObject();
	pPlayer->Init();
	pPlayer->SetSpriteImage(GetBaseDirectory() + "resource\\strike.jpg");
	pPlayer->SetScale(32.0f);

	CGameObject* pPlayer1 = new CGameObject();
	pPlayer1->Init();
	pPlayer1->SetSpriteImage(GetBaseDirectory() + "resource\\playerShip1_orange.png");
	pPlayer1->SetPosiiton(glm::vec3(100.0f, 0.0f, 0.0f));
	pPlayer1->SetScale(32.0f);

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

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// Mesh也没有很好的处理
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode, put in effect // 析构没有处理，引用关系没有处理

		pPlayer->Draw();
		pPlayer1->Draw();

		glfwPollEvents();
		glfwSwapBuffers(CEngine::GetEngine()->m_gWindow);
	}

	glfwTerminate();

	delete g_pEngine;
	g_pEngine = nullptr;

	return 0;
}

void Test_Other()
{
	
	int test = 10;
}


