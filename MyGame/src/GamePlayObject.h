
#pragma once

#include "GameObject.h"

class CPlayer : public CSpriteObject
{
public:
	CPlayer();
	~CPlayer();

	void Update();
	void Draw();
	void ProcessInput(GLFWwindow* window);
private:

};

class CBoxEnemy : public CSolidColorObject
{
public:
	CBoxEnemy();
	~CBoxEnemy();
	
	void Init();
	void Update();
	void Draw();

	void SetTarget(CBaseObject* pTarget);

private:
	CBaseObject* m_pTarget;

};
