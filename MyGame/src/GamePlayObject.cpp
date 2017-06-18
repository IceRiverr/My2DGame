
#include "GamePlayObject.h"

CPlayer::CPlayer()
{

}

CPlayer::~CPlayer()
{

}

void CPlayer::Update()
{


	CSpriteObject::Update();
}

void CPlayer::Draw()
{
	CSpriteObject::Draw();
}

void CPlayer::ProcessInput(GLFWwindow* window)
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

CBoxEnemy::CBoxEnemy()
{

}

CBoxEnemy::~CBoxEnemy()
{

}

void CBoxEnemy::Init()
{
	CSolidColorObject::Init();
	SetShape(CBuildInResource::GetResource<CShape>(CBuildInResource::SHAPE_SPRITE));
}

void CBoxEnemy::Update()
{
	if (m_pTarget)
	{
		float spped = 0.5f;
		glm::vec3 dir = m_pTarget->GetPosition() - GetPosition();
		dir.z = 0.0f;
		float length = glm::length(dir);

		dir = glm::normalize(dir);
		m_vPosition += dir * spped;
		SetPosiiton(m_vPosition);
		
		float red = 0.0f;
		if (length < 100.0f)
			red = 1.0f - length / 100.0f;
		else
			red = 0.0f;
		SetColor(glm::vec4(red, m_vColor.g, m_vColor.b, m_vColor.a));
	}

	CSolidColorObject::Update();
}

void CBoxEnemy::Draw()
{
	CSolidColorObject::Draw();

}

void CBoxEnemy::SetTarget(CBaseObject* pTarget)
{
	m_pTarget = pTarget;
}
