#include "GameObject.h"

#include <glad\glad.h>
#include <GLFW\glfw3.h>

CSceneManager::CSceneManager()
{

}

CSceneManager::~CSceneManager()
{
	for (auto it = m_GameObjectPool.begin(); it != m_GameObjectPool.end();)
	{
		IGameObject* obj = *it;
		DELETE_PTR(obj);
		m_GameObjectPool.erase(it);
		it = m_GameObjectPool.begin();
	}
}

void CSceneManager::Update()
{
	for (auto it = m_GameObjectPool.begin(); it != m_GameObjectPool.end(); ++it)
	{
		(*it)->Update();
	}
}

void CSceneManager::Draw()
{
	for (auto it = m_GameObjectPool.begin(); it != m_GameObjectPool.end(); ++it)
	{
		(*it)->Draw();
	}
}

void CSceneManager::ProcessEvent(GLFWwindow * window)
{
	for (auto it = m_GameObjectPool.begin(); it != m_GameObjectPool.end(); ++it)
	{
		(*it)->ProcessInput(window);
	}
}

void CSceneManager::AddGameObject(IGameObject* obj)
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


CBaseObject::CBaseObject()
	: m_pShape(nullptr)
	, m_vPosition(0.0f, 0.0f, 0.0f)
	, m_fRotate(0.0f)
	, m_fScale(1.0f)
	, m_bMatrixDirty(true)
{

}

CBaseObject::~CBaseObject()
{
	RELEASE_PTR(m_pShape);
	
}

void CBaseObject::Init()
{
}

void CBaseObject::Update()
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

void CBaseObject::Draw()
{
}

void CBaseObject::ProcessInput(GLFWwindow * window)
{
}

void CBaseObject::SetShape(CShape * pShape)
{
	RELEASE_PTR(m_pShape);
	m_pShape = pShape;
	m_pShape->AddRef();
}

void CBaseObject::SetPosiiton(float x, float y, float z)
{
	m_vPosition.x = x;
	m_vPosition.y = y;
	m_vPosition.z = z;
	m_bMatrixDirty = true;
}

void CBaseObject::SetPosiiton(const glm::vec3& pos)
{
	m_vPosition = pos;
	m_bMatrixDirty = true;
}

void CBaseObject::SetRotate(float angle)
{
	m_fRotate = angle;
	m_bMatrixDirty = true;
}

void CBaseObject::SetScale(float scale)
{
	m_fScale = scale;
	m_bMatrixDirty = true;
}

CSpriteObject::CSpriteObject()
	: m_pEffect(nullptr)
	, m_pSprite(nullptr)
{
	
}

CSpriteObject::~CSpriteObject()
{
	RELEASE_PTR(m_pEffect);
	RELEASE_PTR(m_pSprite);
}

void CSpriteObject::Init()
{
	SetEffect(CBuildInResource::GetResource<CSpriteEffect>(CBuildInResource::EFFECT_SPRITE));
}

void CSpriteObject::Update()
{
	CBaseObject::Update();
}

void CSpriteObject::Draw()
{
	if (m_pShape == nullptr || m_pEffect == nullptr || m_pSprite == nullptr)
		return;

	m_pShape->Bind();
	m_pSprite->Bind();
	m_pEffect->Bind();
	m_pEffect->BindParameters(m_mModelMatrix);

	glDrawElements(GL_TRIANGLES, m_pShape->GetMeshData()->m_nNumIndex, GL_UNSIGNED_INT, 0);
}

void CSpriteObject::ProcessInput(GLFWwindow* window)
{
	
}

void CSpriteObject::SetEffect(CSpriteEffect* pEffect)
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

CSolidColorObject::CSolidColorObject()
	: m_pEffect(NULL)
	, m_vColor(0.0f, 0.0f, 1.0f, 1.0f)
{
}

CSolidColorObject::~CSolidColorObject()
{

}

void CSolidColorObject::Init()
{
	SetEffect(CBuildInResource::GetResource<CSolidColorEffect>(CBuildInResource::EFFECT_SOLID_COLOR));
}

void CSolidColorObject::Update()
{
	CBaseObject::Update();
}

void CSolidColorObject::Draw()
{
	if (m_pShape == nullptr || m_pEffect == nullptr)
		return;

	m_pShape->Bind();
	m_pEffect->Bind();
	m_pEffect->BindParameters(m_mModelMatrix, m_vColor);

	glDrawElements(GL_TRIANGLES, m_pShape->GetMeshData()->m_nNumIndex, GL_UNSIGNED_INT, 0);
}

void CSolidColorObject::ProcessInput(GLFWwindow* window)
{

}

void CSolidColorObject::SetColor(const glm::vec4& color)
{
	m_vColor = color;
}

void CSolidColorObject::SetEffect(CSolidColorEffect * pEffect)
{
	RELEASE_PTR(m_pEffect);
	m_pEffect = pEffect;
	m_pEffect->AddRef();
}

CLineObject::CLineObject()
{
}

CLineObject::~CLineObject()
{
}

void CLineObject::Draw()
{
	if (m_pShape == nullptr || m_pEffect == nullptr)
		return;

	m_pShape->Bind();
	m_pEffect->Bind();
	m_pEffect->BindParameters(m_mModelMatrix, m_vColor);

	glDrawElements(GL_LINES, m_pShape->GetMeshData()->m_nNumIndex, GL_UNSIGNED_INT, 0);
}
