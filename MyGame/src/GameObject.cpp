#include "GameObject.h"

#include <glad\glad.h>
#include <GLFW\glfw3.h>

CGameObject::CGameObject()
	: m_pShape(nullptr)
	, m_pEffect(nullptr)
	, m_pSprite(nullptr)
	, m_vPosition(0.0f, 0.0f, 0.0f)
	, m_fRotate(0.0f)
	, m_fScale(1.0f)
	, m_bMatrixDirty(true)
{
	
}

CGameObject::~CGameObject()
{

}

void CGameObject::Init()
{
	m_pShape = new CShape();
	m_pShape->Init();

	m_pEffect = new CSpriteEffect();
	m_pEffect->Init();
}

void CGameObject::Update()
{
	if (m_bMatrixDirty)
	{
		m_mModelMatrix = glm::mat4();
		m_mModelMatrix = glm::translate(m_mModelMatrix, m_vPosition);
		m_mModelMatrix = glm::rotate(m_mModelMatrix, glm::radians(m_fRotate), glm::vec3(0.0f, 0.0f, 1.0f));
		m_mModelMatrix = glm::scale(m_mModelMatrix, glm::vec3(m_fScale, m_fScale, m_fScale));
		m_bMatrixDirty = false;
	}
}

void CGameObject::Draw()
{
	m_pShape->Bind();
	m_pSprite->Bind();
	m_pEffect->Bind();
	m_pEffect->BindParameters(m_mModelMatrix);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // test, need to adjust
}

void CGameObject::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		m_vPosition.x += 1.0f;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		m_vPosition.x -= 1.0f;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		m_vPosition.y += 1.0f;

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		m_vPosition.y -= 1.0f;

	m_bMatrixDirty = true;
}

void CGameObject::SetPosiiton(const glm::vec3 & pos)
{
	m_vPosition = pos;
	m_bMatrixDirty = true;
}

void CGameObject::SetRotate(float angle)
{
	m_fRotate = angle;
	m_bMatrixDirty = true;
}

void CGameObject::SetScale(float scale)
{
	m_fScale = scale;
	m_bMatrixDirty = true;
}

void CGameObject::SetSpriteImage(const std::string& filePath)
{
	if (m_pSprite != nullptr)
	{
		delete m_pSprite;
		m_pSprite = nullptr;
	}
	m_pSprite = new CTexture2D(filePath);
	m_pSprite->Init();
}
