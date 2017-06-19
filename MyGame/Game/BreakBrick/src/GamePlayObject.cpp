
#include "GamePlayObject.h"

CPlayer::CPlayer()
{
	m_fAttackRange = 128.0f;
}

CPlayer::~CPlayer()
{

}

void CPlayer::Update(float dt)
{
	CSpriteObject::Update(dt);
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

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		m_bActive = true;
	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		m_bActive = false;
	
	m_bMatrixDirty = true;
}

CBoxEnemy::CBoxEnemy()
{
	m_fRotateSpeed = 500.0f;
	m_vMoveDir = glm::vec3();
	m_fMaxMoveSpeed = 100.0f;
}

CBoxEnemy::~CBoxEnemy()
{

}

void CBoxEnemy::Init()
{
	CSolidColorObject::Init();
	SetShape(CBuildInResource::GetResource<CShape>(CBuildInResource::SHAPE_SPRITE));
}

void CBoxEnemy::Update(float dt)
{
	if (m_pTarget)
	{
		glm::vec3 dir = m_pTarget->GetPosition() - GetPosition();
		dir.z = 0.0f;
		float length = glm::length(dir);

		dir = glm::normalize(dir);

		float fAttackArange = m_pTarget->GetAttackRange();

		if (length < fAttackArange && m_pTarget->IsAttack())
		{
			m_vMoveDir = -dir * m_fMaxMoveSpeed;
		}
		else
		{
			m_vMoveDir += dir * m_fRotateSpeed *  dt;
			float moveSpeed = glm::length(m_vMoveDir);
			m_vMoveDir /= moveSpeed;
			m_vMoveDir *= m_fMaxMoveSpeed;
		}

		m_vPosition += m_vMoveDir * dt;
		SetPosiiton(m_vPosition);

		float red = 0.0f;
		if (length < fAttackArange)
			red = 1.0f - length / fAttackArange;
		else
			red = 0.0f;
		SetColor(glm::vec4(red, m_vColor.g, m_vColor.b, m_vColor.a));
	}

	CSolidColorObject::Update(dt);
}

void CBoxEnemy::Draw()
{
	CSolidColorObject::Draw();
}

void CBoxEnemy::SetTarget(CPlayer* pTarget)
{
	m_pTarget = pTarget;
}

void CBoxEnemy::SetMoveDir(const glm::vec3& dir)
{
	m_vMoveDir = dir;
}
