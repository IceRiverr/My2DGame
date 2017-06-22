
#pragma once

#include "GameObject.h"

class BaseGameEntity;
class MovingEntity;
class GameWorld;
class SteeringBehaviors;

class Drawable : public CLineObject
{

};

class GameWorld
{


};

class SteeringBehaviors
{
public:
	glm::vec2 Calculate();

};

class BaseGameEntity
{
public:
	BaseGameEntity();
	virtual ~BaseGameEntity();

	void SetPosition(float x, float y);
	void SetScale(float sx, float sy);
	void SetBRadius(float r);

protected:
	glm::vec2			m_vPos;
	glm::vec2			m_vScale;
	float				m_fBRadius;
};

class MovingEntity : public BaseGameEntity
{
public:
	
protected:
	glm::vec2			m_vVelocity;
	glm::vec2			m_vHeading;
	glm::vec2			m_vSide;
	float				m_fMass;
	float				m_fMaxSpeed;
	float				m_fMaxForce;
	float				m_fMaxTurnRate;
};

class Vehicle : public MovingEntity
{
public:
	Vehicle();
	~Vehicle();

	void Init(CShape* shape);
	void Update(float dt);
	void Draw();

protected:
	GameWorld*			m_pWorld;
	SteeringBehaviors*	m_pSteering;
	Drawable*			m_pDraw;

	float				m_fRotation;
};

glm::vec2 Truncate(glm::vec2 in, float length);
float LengthSqr(const glm::vec2 in);
glm::vec2 Perp(glm::vec2 in);