
#pragma once

#include "GameObject.h"

class CAttackGameRule;

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

	void SetGameRule(CAttackGameRule* rule);
	CAttackGameRule* GetGameRule() { return m_pRule; }

private:
	float				m_fAttackRange;
	bool				m_bActive;
	CAttackGameRule*	m_pRule;
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
	int				m_nLife;

};

class CAttackGameRule
{
public:
	bool IsWin();

	void SetNumEnemies(int num);
	void DecressEnemies();
private:
	int				m_nNumEnemies;
};
