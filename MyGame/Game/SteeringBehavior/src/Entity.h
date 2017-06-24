
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

	enum BEHAVIOR_TYPE
	{
		NONE			= 0x00000,
		SEEK			= 0x00001,
		FLEE			= 0x00002,
		ARRIVE			= 0x00004,
		PURSUIT			= 0x00008,
		EVADE			= 0x00010,
	};

	SteeringBehaviors(Vehicle* v);
	~SteeringBehaviors();

	glm::vec2 Calculate(float dt);

	glm::vec2 CalculateWeightedSum(float dt);

	void OnBehavior(BEHAVIOR_TYPE type);
	void OffBehavior(BEHAVIOR_TYPE type);

	glm::vec2 Seek(glm::vec2 TargetPos);

	glm::vec2 Flee(glm::vec2 TargetPos);

	glm::vec2 Arrive(glm::vec2 TargetPos, DECELERATION dec);

	glm::vec2 Pursuit(const Vehicle* evader);

	glm::vec2 Evade(const Vehicle* pursuer);

	glm::vec2 Wander(float dt);

private:
	Vehicle*			m_pVehicle;
	int					m_BehaviorFlag;
	glm::vec2			m_vSteeringForce;

	float				m_fSeekWeight;
	float				m_fFleeWeight;
	float				m_fWanderWeight;

	// Wander
	float				m_fWanderRadius;
	float				m_fWanderDistance;
	float				m_fWanderJitter;
	glm::vec2			m_vWanderTarget;
	
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

	void Start(CShape* shape);
	void Update(float dt);
	void Draw();

	const glm::mat4& GetTransformMatrix() const;

protected:
	GameWorld*			m_pWorld;
	SteeringBehaviors*	m_pSteering;
	Drawable*			m_pDraw;

	float				m_fRotation;
};

float TurnaroundTime(const Vehicle* v, glm::vec2 TargetPos);
