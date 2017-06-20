
#pragma once

#include "GameObject.h"

class CPlayer : public CSpriteObject
{
public:
	CPlayer();
	~CPlayer();

	void Update(float dt);
	void Draw();
	void ProcessInput(GLFWwindow* window);

	bool IsAttack()			{ return m_bActive; }
	float GetAttackRange()	{ return m_fAttackRange; }
private:
	float	m_fAttackRange;
	bool	m_bActive;
};

class CBrick : public CSpriteObject
{
public:
	CBrick();
	~CBrick();

	void Update(float dt);
	void Draw();
	
private:
	glm::vec2	m_Size;

};



class CBoxEnemy : public CSolidColorObject
{
public:
	CBoxEnemy();
	~CBoxEnemy();
	
	void Init();
	void Update(float dt);
	void Draw();

	void SetTarget(CPlayer* pTarget);
	void SetMoveDir(const glm::vec3& dir);

private:
	CPlayer*		m_pTarget;
	glm::vec3		m_vMoveDir;
	float			m_fRotateSpeed;
	float			m_fMaxMoveSpeed;
};
