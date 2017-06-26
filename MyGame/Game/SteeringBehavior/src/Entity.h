
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
	void AddObstacle(BaseGameEntity* o);

	void TagObstaclesWithinViewRange(BaseGameEntity* pVehicle, float range);

	const std::vector<BaseGameEntity*>& GetObstacles() const { return m_Obstacles; }

private:
	std::vector<Vehicle*>			m_Vehicles;
	std::vector<BaseGameEntity*>	m_Obstacles;
};

class SteeringBehaviors
{
public:
	enum DECELERATION { SLOW = 3, NORMAL = 2, FAST = 1, };

	enum BEHAVIOR_TYPE
	{
		NONE				= 0x00000,
		SEEK				= 0x00001,
		FLEE				= 0x00002,
		ARRIVE				= 0x00004,
		PURSUIT				= 0x00008,
		EVADE				= 0x00010,
		WANDER				= 0x00020,
		OBSTACLE_AVOIDANCE	= 0x00040,
	};

	SteeringBehaviors(Vehicle* v);
	~SteeringBehaviors();

	glm::vec2 Calculate(float dt);

	glm::vec2 CalculateWeightedSum(float dt);

	void OnBehavior(BEHAVIOR_TYPE type);
	void OffBehavior(BEHAVIOR_TYPE type);
	bool TestBehavior(BEHAVIOR_TYPE type);

	glm::vec2 Seek(glm::vec2 TargetPos);

	glm::vec2 Flee(glm::vec2 TargetPos);

	glm::vec2 Arrive(glm::vec2 TargetPos, DECELERATION dec);

	glm::vec2 Pursuit(const Vehicle* evader);

	glm::vec2 Evade(const Vehicle* pursuer);

	glm::vec2 Wander(float dt);

	glm::vec2 ObstacleAvoidance(const std::vector<BaseGameEntity*>& obstacles);

	glm::vec2 Interpose(const Vehicle* AgentA, const Vehicle* AgentB);

	glm::vec2 Hide(const Vehicle* target, const std::vector<BaseGameEntity*>& obstacles);

	glm::vec2 OffsetPursuit(const Vehicle* leader, glm::vec2 offset);

	glm::vec2 Sepration(const std::vector<Vehicle*>& neithbors);
	
	glm::vec2 Alignment(const std::vector<Vehicle*>& neithbors);

	glm::vec2 Cohesion(const std::vector<Vehicle*>& neithbors);


	glm::vec2 GetHidingPosition(const glm::vec2& posOb, float radiusOb, const glm::vec2& posTarget);
	

private:
	Vehicle*			m_pVehicle;
	int					m_BehaviorFlag;
	glm::vec2			m_vSteeringForce;

	float				m_fSeekWeight;
	float				m_fFleeWeight;
	float				m_fWanderWeight;
	float				m_fObstacleAvoidanceWeight;

	// Wander
	float				m_fWanderRadius;
	float				m_fWanderDistance;
	float				m_fWanderJitter;
	glm::vec2			m_vWanderTarget;

	// obstacle
	float				m_fDBoxLength;
	
};

class BaseGameEntity
{
	friend class SteeringBehaviors; // ReadOnly
public:
	BaseGameEntity();
	virtual ~BaseGameEntity();

	virtual void Update(float dt);

	virtual void Draw();

	void SetPosition(float x, float y);
	void SetBRadius(float r);

	glm::vec2 GetPosition() const {return m_vPos;}
	float GetBRadius() { return m_fBRadius; }

	// Tag
	void Tag() { m_bTag = true; }
	void UnTag() { m_bTag = false; }
	bool IsTagged() { return m_bTag; }

protected:
	glm::vec2			m_vPos;
	float				m_fBRadius;
	bool				m_bTag;
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

	void Start(CShape* shape,glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f));
	void Update(float dt);
	void Draw();

	const glm::mat4& GetTransformMatrix() const;
	GameWorld* GetWorld()				{ return m_pWorld; }
	SteeringBehaviors* GetBehaviors()	{ return m_pSteering;}
	void SetWorld(GameWorld* g);

protected:
	GameWorld*			m_pWorld;
	SteeringBehaviors*	m_pSteering;
	Drawable*			m_pDraw;

	float				m_fRotation;
};

class ObatacleEntity : public BaseGameEntity
{
public:
	ObatacleEntity();
	void Start(CShape* shape);
	void Update(float dt);
	void Draw();
private:
	Drawable*			m_pDraw;
};

float TurnaroundTime(const Vehicle* v, glm::vec2 TargetPos);

glm::vec2 TransformPosition(const glm::mat4 Trans, glm::vec2 pos);
