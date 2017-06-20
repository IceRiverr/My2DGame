
#include <iostream>
#include "FontLib.h"
#include "TG_Sprites.h"

CPlayer::CPlayer()
{
	m_fAttackRange = 128.0f;
	m_Name = "Amy";
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
	CFontLib::DrawTextAt(m_Name,GetPosition().x - 16.0f, GetPosition().y + 36.0f, 16.0f, glm::vec3(1.0f, 0.0f, 1.0f));
}

void CPlayer::ProcessInput(GLFWwindow* window)
{
	glm::vec3 pos = GetPosition();

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		pos.x += 1.0f;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		pos.x -= 1.0f;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		pos.y += 1.0f;

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		pos.y -= 1.0f;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		m_bActive = true;
	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		m_bActive = false;
	
	SetPosiiton(pos);
}

void CPlayer::SetGameRule(CAttackGameRule* rule)
{
	m_pRule = rule;
}

CBoxEnemy::CBoxEnemy()
{
	m_fRotateSpeed = 500.0f;
	m_vMoveDir = glm::vec3();
	m_fMaxMoveSpeed = 100.0f;
	m_nLife = 20;
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
		glm::vec3 pos = GetPosition();

		glm::vec3 dir = m_pTarget->GetPosition() - pos;
		dir.z = 0.0f;
		float length = glm::length(dir);

		dir = glm::normalize(dir);

		float fAttackArange = m_pTarget->GetAttackRange();

		if (length < fAttackArange && m_pTarget->IsAttack())
		{
			m_vMoveDir = -dir * m_fMaxMoveSpeed;
			m_nLife--;
			if (m_nLife < 0)
			{
				m_pTarget->GetGameRule()->DecressEnemies();
				SetDestroyFlag();
			}
		}
		else
		{
			m_vMoveDir += dir * m_fRotateSpeed *  dt;
			float moveSpeed = glm::length(m_vMoveDir);
			m_vMoveDir /= moveSpeed;
			m_vMoveDir *= m_fMaxMoveSpeed;
		}

		pos += m_vMoveDir * dt;
		SetPosiiton(pos);

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

bool CAttackGameRule::IsWin()
{
	if (m_nNumEnemies == 0)
 		return true;
	return false;
}

void CAttackGameRule::SetNumEnemies(int num)
{
	m_nNumEnemies = num;
}

void CAttackGameRule::DecressEnemies()
{
	m_nNumEnemies--;
	std::cout << "You Kill a box monster, yyy." << std::endl;
}
