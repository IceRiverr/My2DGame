#include "GameObject.h"

#include <glad\glad.h>
#include <GLFW\glfw3.h>

CGameObjectManager::CGameObjectManager()
{

}

CGameObjectManager::~CGameObjectManager()
{
	for (auto it = m_GameObjectPool.begin(); it != m_GameObjectPool.end();)
	{
		IGameObject* obj = *it;
		DELETE_PTR(obj);
		m_GameObjectPool.erase(it);
		it = m_GameObjectPool.begin();
	}
}

void CGameObjectManager::AddGameObject(IGameObject* obj)
{
	if (obj == nullptr)
		return;

	for (auto it = m_GameObjectPool.begin(); it != m_GameObjectPool.end(); ++it)
	{
		if (*it == obj)
			return;
	}
	m_GameObjectPool.push_back(obj);
}

CSpriteObject::CSpriteObject()
	: IGameObject()
	, m_pShape(nullptr)
	, m_pEffect(nullptr)
	, m_pSprite(nullptr)
	, m_vPosition(0.0f, 0.0f, 0.0f)
	, m_fRotate(0.0f)
	, m_fScale(1.0f)
	, m_bMatrixDirty(true)
{
	
}

CSpriteObject::~CSpriteObject()
{
	RELEASE_PTR(m_pShape);
	RELEASE_PTR(m_pEffect);
	RELEASE_PTR(m_pSprite);
}

void CSpriteObject::Init()
{
}

void CSpriteObject::Update()
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

void CSpriteObject::Draw()
{
	if (m_pShape == nullptr || m_pEffect == nullptr || m_pSprite == nullptr)
		return;

	m_pShape->Bind();
	m_pSprite->Bind();
	m_pEffect->Bind();
	m_pEffect->BindParameters(m_mModelMatrix);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // test, need to adjust
}

void CSpriteObject::ProcessInput(GLFWwindow* window)
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

void CSpriteObject::SetShape(CShape * pShape)
{
	RELEASE_PTR(m_pShape);
	m_pShape = pShape;
	m_pShape->AddRef();
}

void CSpriteObject::SetEffect(CSpriteEffect * pEffect)
{
	RELEASE_PTR(m_pEffect);
	m_pEffect = pEffect;
	m_pEffect->AddRef();
}

void CSpriteObject::SetSprite(CTexture2D * pSprite)
{
	RELEASE_PTR(m_pSprite);
	m_pSprite = pSprite;
	m_pSprite->AddRef();
}

void CSpriteObject::SetPosiiton(const glm::vec3 & pos)
{
	m_vPosition = pos;
	m_bMatrixDirty = true;
}

void CSpriteObject::SetRotate(float angle)
{
	m_fRotate = angle;
	m_bMatrixDirty = true;
}

void CSpriteObject::SetScale(float scale)
{
	m_fScale = scale;
	m_bMatrixDirty = true;
}
