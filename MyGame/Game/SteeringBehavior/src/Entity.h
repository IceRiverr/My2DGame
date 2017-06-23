
#pragma once

#include "GameObject.h"

class BaseGameEntity;
class MovingEntity;
class GameWorld;
class SteeringBehaviors;
class Vehicle;

class Drawable : public CLineObject
{

};

class GameWorld
{
public:
	void Update(float dt);
	void Draw();

	void AddEntity(Vehicle* v);

private:
	std::vector<Vehicle*> m_Vehicles;
};

class SteeringBehaviors
{
public:
	enum DECELERATION { SLOW = 3, NORMAL = 2, FAST = 1, };

	SteeringBehaviors(Vehicle* v);
	~SteeringBehaviors();

	glm::vec2 Calculate();

	glm::vec2 Seek(glm::vec2 TargetPos);

	glm::vec2 Flee(glm::vec2 TargetPos);

	glm::vec2 Arrive(glm::vec2 TargetPos, DECELERATION dec);

	glm::vec2 Pursuit(const Vehicle* evader);

private:
	Vehicle*			m_pVehicle;

	float				m_fSeekWeight;
	float				m_fFleeWeight;
};

class BaseGameEntity
{
	friend class SteeringBehaviors; // ReadOnly
public:
	BaseGameEntity();
	virtual ~BaseGameEntity();

	void SetPosition(float x, float y);
	void SetScale(float sx, float sy);
	void SetBRadius(float r);

	glm::vec2 GetPosition() const {return m_vPos;}

protected:
	glm::vec2			m_vPos;
	glm::vec2			m_vScale;
	float				m_fBRadius;
};

class MovingEntity : public BaseGameEntity
{
	friend class SteeringBehaviors; // ReadOnly
public:
	
	float GetSpeed() const;
	glm::vec2 GetHeading() const { return m_vHeading; }

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
	friend class SteeringBehaviors; // ReadOnly
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

float TurnaroundTime(const Vehicle* v, glm::vec2 TargetPos);




glm::vec2 Truncate(glm::vec2 in, float length);
float LengthSqr(const glm::vec2 in);
glm::vec2 Perp(glm::vec2 in);
float Vec2ToRadian(glm::vec2 in);


void DebugValue(std::string msg, glm::vec2 v);
void DebugValue(std::string msg, float v);