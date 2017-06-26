
#include "Entity.h"
#include "FontLib.h"
#include "Entity.h"
#include "MathLib.h"
#include "Engine.h"
#include <xtgmath.h>
#include "DebugLib.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <stdlib.h>
#include <iostream>

void GameWorld::Update(float dt)
{
	for (int i = 0; i < m_Vehicles.size(); ++i)
		m_Vehicles[i]->Update(dt);

	for (int i = 0; i < m_Obstacles.size(); ++i)
		m_Obstacles[i]->Update(dt);
}

void GameWorld::Draw()
{
	for (int i = 0; i < m_Vehicles.size(); ++i)
		m_Vehicles[i]->Draw();

	for (int i = 0; i < m_Obstacles.size(); ++i)
		m_Obstacles[i]->Draw();
}

void GameWorld::AddEntity(Vehicle* v)
{
	m_Vehicles.push_back(v);
	v->SetWorld(this);
}

void GameWorld::AddObstacle(BaseGameEntity* o)
{
	m_Obstacles.push_back(o);
}

void GameWorld::TagObstaclesWithinViewRange(BaseGameEntity* pVehicle, float range)
{
	for (auto it = m_Obstacles.begin(); it != m_Obstacles.end(); ++it)
	{
		(*it)->UnTag();

		glm::vec2 To = (*it)->GetPosition() - pVehicle->GetPosition();

		float nRange = range + (*it)->GetBRadius();

		if (pVehicle != (*it) && Math::LengthSqr(To) < nRange * nRange)
		{
			(*it)->Tag();
		}
	}
}


SteeringBehaviors::SteeringBehaviors(Vehicle* v)
	: m_pVehicle(v)
{
	m_BehaviorFlag = 0;

	m_fSeekWeight = 1.0f;
	m_fFleeWeight = 1.0f;
	m_fWanderWeight = 1.0f;
	m_fObstacleAvoidanceWeight = 1.0f;

	m_fWanderRadius = 50.0f;
	m_fWanderDistance = 2.0f;
	m_fWanderJitter = 1000.0f;

	float random = (float)Math::RandomUnit();
	m_vWanderTarget = glm::vec2(std::cos(random), std::sin(random)) * m_fWanderRadius;
}

SteeringBehaviors::~SteeringBehaviors()
{
	m_pVehicle = nullptr;
}

glm::vec2 SteeringBehaviors::Calculate(float dt)
{
	m_vSteeringForce = glm::vec2(0.0f, 0.0f);
	m_vSteeringForce = CalculateWeightedSum(dt);

	return m_vSteeringForce;
}

glm::vec2 SteeringBehaviors::CalculateWeightedSum(float dt)
{
	// force = Seek(glm::vec2(550.0f, 583.0f)) * m_fSeekWeight;

	// force = Flee(glm::vec2(410.0f, 310.0f)) * m_fFleeWeight;

	if(TestBehavior(ARRIVE))
		m_vSteeringForce = Arrive(glm::vec2(500.0f, 500.0f), DECELERATION::FAST) * 1.0f;
	if (TestBehavior(OBSTACLE_AVOIDANCE))
		m_vSteeringForce += ObstacleAvoidance(m_pVehicle->GetWorld()->GetObstacles()) * m_fObstacleAvoidanceWeight;
	if (TestBehavior(WANDER))
		m_vSteeringForce += Wander(dt) * m_fWanderWeight;

	m_vSteeringForce = Math::Truncate(m_vSteeringForce, m_pVehicle->m_fMaxForce);

	return m_vSteeringForce;
}

void SteeringBehaviors::OnBehavior(BEHAVIOR_TYPE type)
{
	m_BehaviorFlag = Math::SetFlag(m_BehaviorFlag, type);
}

void SteeringBehaviors::OffBehavior(BEHAVIOR_TYPE type)
{
	m_BehaviorFlag = Math::ClearFlag(m_BehaviorFlag, type);
}

bool SteeringBehaviors::TestBehavior(BEHAVIOR_TYPE type)
{
	return Math::TestFlag(m_BehaviorFlag, type);
}

glm::vec2 SteeringBehaviors::Seek(glm::vec2 TargetPos)
{
	glm::vec2 DesiredVelocity = glm::normalize(TargetPos - m_pVehicle->m_vPos) * m_pVehicle->m_fMaxSpeed;
	return (DesiredVelocity - m_pVehicle->m_vVelocity);
}

glm::vec2 SteeringBehaviors::Flee(glm::vec2 TargetPos)
{
	const float PanicDistanceSqr = 100.0f * 100.0f;
	if (Math::LengthSqr(m_pVehicle->m_vPos - TargetPos) > PanicDistanceSqr)
		return glm::vec2(0.0f, 0.0f);

	glm::vec2 DesiredVelocity = glm::normalize(m_pVehicle->m_vPos - TargetPos) * m_pVehicle->m_fMaxSpeed;
	return(DesiredVelocity - m_pVehicle->m_vVelocity);
}

glm::vec2 SteeringBehaviors::Arrive(glm::vec2 TargetPos, DECELERATION dec)
{
	glm::vec2 ToTarget = TargetPos - m_pVehicle->m_vPos;
	float dist = glm::length(ToTarget);
	if (dist > 1.0f)
	{
		const float DecelerationTweaker = 1.0f;
		float speed = dist / ((float)dec * DecelerationTweaker);
		speed = glm::min(speed, m_pVehicle->m_fMaxSpeed);
		glm::vec2 DesiredVelocity = ToTarget / dist * speed;
		return (DesiredVelocity - m_pVehicle->m_vVelocity);
	}
	return glm::vec2(0.0f, 0.0f);
}

glm::vec2 SteeringBehaviors::Pursuit(const Vehicle* evader)
{
	glm::vec2 ToEvader = evader->m_vPos - m_pVehicle->m_vPos;

	float RelativeHeading = glm::dot(m_pVehicle->m_vHeading, evader->m_vHeading);

	if (glm::dot(m_pVehicle->m_vHeading, ToEvader) &&
		(RelativeHeading < -0.95f))
	{
		return Seek(evader->m_vPos);
	}

	float LookAheadTime = glm::length(ToEvader) / (m_pVehicle->m_fMaxSpeed + evader->GetSpeed());
	LookAheadTime += TurnaroundTime(m_pVehicle, evader->GetPosition());
	return Seek(evader->m_vPos + evader->m_vVelocity * LookAheadTime);
}

glm::vec2 SteeringBehaviors::Evade(const Vehicle* pursuer)
{
	glm::vec2 ToPursuer = pursuer->GetPosition() - m_pVehicle->GetPosition();

	float LookAheadTime = glm::length(ToPursuer) / (m_pVehicle->m_fMaxSpeed + pursuer->GetSpeed());

	return Flee(pursuer->GetPosition() + pursuer->m_vVelocity * LookAheadTime);
}

glm::vec2 SteeringBehaviors::Wander(float dt)
{
	float JitterThisTimeSlice = m_fWanderJitter * dt;

	float r1 = (float)Math::RandomUniform();
	float r2 = (float)Math::RandomUniform();

	glm::vec2 jitterValue = glm::vec2(r1 * JitterThisTimeSlice, r2 * JitterThisTimeSlice);

	m_vWanderTarget += jitterValue;

	m_vWanderTarget = glm::normalize(m_vWanderTarget);

	m_vWanderTarget *= m_fWanderRadius;

	glm::vec2 target = m_vWanderTarget + glm::vec2(0.0f, m_fWanderDistance);

	glm::mat4 trans;
	trans = glm::translate(trans, glm::vec3(m_pVehicle->m_vPos, 1.0f));
	trans = glm::rotate(trans, m_pVehicle->m_fRotation, glm::vec3(0.0f, 1.0f, 0.0f));
	target = trans * glm::vec4(target, 0.0f, 1.0f);

	glm::vec2 force = target - m_pVehicle->GetPosition();

	return force;
}

glm::vec2 SteeringBehaviors::ObstacleAvoidance(const std::vector<BaseGameEntity*>& obstacles)
{
	const float fMinDetectionBoxLength = 80.0f;
	m_fDBoxLength = fMinDetectionBoxLength + (m_pVehicle->GetSpeed() / m_pVehicle->m_fMaxSpeed) * fMinDetectionBoxLength;

	m_pVehicle->GetWorld()->TagObstaclesWithinViewRange(m_pVehicle, m_fDBoxLength);

	BaseGameEntity* ClosestIntersectingObatacle = nullptr;

	float DistToClosestIP = FLT_MAX;

	glm::vec2 LocalPosOfClosestObstacle;

	for (auto it = obstacles.begin(); it != obstacles.end(); ++it)
	{
		if ((*it)->IsTagged())
		{
			glm::mat4 Trans = m_pVehicle->GetTransformMatrix();
			Trans = glm::inverse(Trans);
			glm::vec2 LocalPos = Trans * glm::vec4((*it)->GetPosition(), 0.0f, 1.0f);

			if (LocalPos.y >= 0.0f)
			{
				float ExpandedRadius = (*it)->GetBRadius() + m_pVehicle->GetBRadius();

				if (std::abs(LocalPos.x) < ExpandedRadius)
				{
					float cx = LocalPos.x;
					float cy = LocalPos.y;

					float SqrtPart = std::sqrt(ExpandedRadius * ExpandedRadius - cx * cx);

					float ip = cy - SqrtPart;

					if (ip <= 0.0f)
						ip = cy + SqrtPart;

					if (ip < DistToClosestIP)
					{
						DistToClosestIP = ip;
						ClosestIntersectingObatacle = *it;
						LocalPosOfClosestObstacle = LocalPos;
					}
				}
			}
		}
	}

	glm::vec2 SteeringForce;

	if (ClosestIntersectingObatacle)
	{
		//float Multiplier = 1.0f + (m_fDBoxLength - LocalPosOfClosestObstacle.y) / m_fDBoxLength;
		float Multiplier = 2.0f;

		SteeringForce.x = (ClosestIntersectingObatacle->GetBRadius() - LocalPosOfClosestObstacle.x) * Multiplier;

		float BreakingWeight = 2.0f;

		SteeringForce.y = (ClosestIntersectingObatacle->GetBRadius() - LocalPosOfClosestObstacle.y) * BreakingWeight;

		glm::mat4 Trans;

		float Rotate = m_pVehicle->m_fRotation;

		Trans = glm::rotate(Trans, Rotate, glm::vec3(0.0f, 0.0f, 1.0f));

		SteeringForce = Trans * (glm::vec4(SteeringForce, 0.0f, 0.0f));
	}
	return SteeringForce;
}

glm::vec2 SteeringBehaviors::Interpose(const Vehicle* AgentA, const Vehicle* AgentB)
{
	glm::vec2 MidPoint = (AgentA->GetPosition() + AgentB->GetPosition()) * 0.5f;
	float TimeToReachMidPoint = glm::length(MidPoint - m_pVehicle->GetPosition()) / m_pVehicle->m_fMaxSpeed;

	glm::vec2 APos = AgentA->GetPosition() + AgentA->m_vVelocity * TimeToReachMidPoint;
	glm::vec2 BPos = AgentB->GetPosition() + AgentB->m_vVelocity * TimeToReachMidPoint;

	MidPoint = (APos + BPos) * 0.5f;

	return Arrive(MidPoint, FAST);
}

glm::vec2 SteeringBehaviors::Hide(const Vehicle* target, const std::vector<BaseGameEntity*>& obstacles)
{
	float DistToClosest = FLT_MAX;
	glm::vec2 BestHidingSpot;

	for (auto it = obstacles.begin(); it != obstacles.end(); ++it)
	{
	 	glm::vec2 HidingSpot = GetHidingPosition((*it)->GetPosition(), (*it)->GetBRadius(), target->GetPosition());

		float Dist = glm::length(HidingSpot - m_pVehicle->GetPosition());

		if (Dist < DistToClosest)
			BestHidingSpot = HidingSpot;
	}

	if (DistToClosest == FLT_MAX)
		return Evade(target);
	else
		return Arrive(BestHidingSpot, FAST);
}

glm::vec2 SteeringBehaviors::OffsetPursuit(const Vehicle* leader, glm::vec2 offset)
{
	glm::vec2 WorldOffstPos = TransformPosition(leader->GetTransformMatrix(), offset);

	glm::vec2 ToOffset = WorldOffstPos - m_pVehicle->GetPosition();

	float LookAheadTime = glm::length(ToOffset) / (m_pVehicle->m_fMaxSpeed + leader->GetSpeed());

	return Arrive(WorldOffstPos + leader->m_vVelocity * LookAheadTime, FAST);
}

glm::vec2 SteeringBehaviors::Sepration(const std::vector<Vehicle*>& neithbors)
{
	glm::vec2 SteeringForce;

	for (int i = 0; i < neithbors.size(); ++i)
	{
		if (neithbors[i] != m_pVehicle && neithbors[i]->IsTagged())
		{
			glm::vec2 ToAgent = m_pVehicle->GetPosition() - neithbors[i]->GetPosition();
			SteeringForce += glm::normalize(ToAgent) / glm::length(ToAgent);
		}
	}

	return SteeringForce;
}

glm::vec2 SteeringBehaviors::Alignment(const std::vector<Vehicle*>& neithbors)
{
	glm::vec2 AverageHeading;

	int NeighborCount = 0;

	for (int i = 0; i < neithbors.size(); ++i)
	{
		if (neithbors[i] != m_pVehicle && neithbors[i]->IsTagged())
		{
			AverageHeading += neithbors[i]->GetHeading();
			++NeighborCount;
		}
	}

	if (NeighborCount > 0)
	{
		AverageHeading /= NeighborCount;
		AverageHeading -= m_pVehicle->GetHeading();
	}

	return AverageHeading;
}

glm::vec2 SteeringBehaviors::Cohesion(const std::vector<Vehicle*>& neithbors)
{
	glm::vec2 CenterOfMass, SteeringForce;

	int NeighborCount = 0;

	for (int i = 0; i < neithbors.size(); ++i)
	{
		if (neithbors[i] != m_pVehicle && neithbors[i]->IsTagged())
		{
			CenterOfMass += neithbors[i]->GetPosition();
			++NeighborCount;
		}
	}

	if (NeighborCount > 0)
	{
		CenterOfMass /= NeighborCount;
		SteeringForce = Seek(CenterOfMass);
	}

	return SteeringForce;
}

glm::vec2 SteeringBehaviors::GetHidingPosition(const glm::vec2& posOb, float radiusOb, const glm::vec2& posTarget)
{
	const float DistFromBoundary = 30.0f;
	float DistAway = DistFromBoundary + radiusOb;

	glm::vec2 ToOb = glm::normalize(posOb - posTarget);

	return (ToOb * DistAway) + posOb;
}

BaseGameEntity::BaseGameEntity()
{

}

BaseGameEntity::~BaseGameEntity()
{

}

void BaseGameEntity::Update(float dt)
{

}

void BaseGameEntity::Draw()
{

}

void BaseGameEntity::SetPosition(float x, float y)
{
	m_vPos.x = x;
	m_vPos.y = y;
}

void BaseGameEntity::SetBRadius(float r)
{
	m_fBRadius = r;
}

Vehicle::Vehicle()
{
	m_pWorld = nullptr;
	m_pSteering = nullptr;
	m_pDraw = nullptr;
	
	m_fMass = 1.0f;
	m_fMaxSpeed = 200.0f;
	m_fMaxForce = 200.0f;
	m_vVelocity = glm::vec2(0.0f, 10.0f);
}

Vehicle::~Vehicle()
{
	DELETE_PTR(m_pDraw);
}

void Vehicle::Start(CShape* shape, glm::vec3 color)
{
	m_pDraw = new Drawable();
	m_pDraw->Init();
	m_pDraw->SetShape(shape);
	m_pDraw->SetColor(glm::vec4(color, 1.0f));

	m_pSteering = new SteeringBehaviors(this);
}

void Vehicle::Update(float dt)
{
	if (m_pWorld == nullptr)
		return;

	glm::vec2 SteeringForce = m_pSteering->Calculate(dt);

	glm::vec2 acceleration = SteeringForce / m_fMass;

	m_vVelocity += acceleration * dt;

	m_vVelocity = Math::Truncate(m_vVelocity, m_fMaxSpeed);

	m_vPos += m_vVelocity * dt;

	if (m_vPos.x < 0.0f)
		m_vPos.x += GetMainWindow()->m_nWidth;
	if (m_vPos.x > GetMainWindow()->m_nWidth)
		m_vPos.x -= GetMainWindow()->m_nWidth;
	if (m_vPos.y < 0.0f)
		m_vPos.y += GetMainWindow()->m_nHeight;
	if (m_vPos.y > GetMainWindow()->m_nHeight)
		m_vPos.y -= GetMainWindow()->m_nHeight;

	if (Math::LengthSqr(m_vVelocity) > 0.0000001f)
	{
		m_vHeading = glm::normalize(m_vVelocity);
		m_vSide = Math::Perp(m_vHeading);
	}

	m_fRotation = Math::UnitVec2ToRadian(m_vHeading) - (float)PI * 0.5f;

	m_pDraw->SetPosiiton(glm::vec3(m_vPos, 1.0f));
	m_pDraw->SetRotate(m_fRotation);
	m_pDraw->Update(dt);
}

void Vehicle::Draw()
{
	m_pDraw->Draw();
}

const glm::mat4& Vehicle::GetTransformMatrix() const
{
	return m_pDraw->GetTransformMatrix();
}

void Vehicle::SetWorld(GameWorld* g)
{
	m_pWorld = g;
}

float MovingEntity::GetSpeed() const
{
	return glm::length(m_vVelocity);
}

ObatacleEntity::ObatacleEntity()
{

}

void ObatacleEntity::Start(CShape* shape)
{
	m_pDraw = new Drawable();
	m_pDraw->Init();
	m_pDraw->SetShape(shape);
}

void ObatacleEntity::Update(float dt)
{
	m_pDraw->SetPosiiton(glm::vec3(m_vPos, 1.0f));
	m_pDraw->SetScale(m_fBRadius, m_fBRadius);
	m_pDraw->Update(dt);
}

void ObatacleEntity::Draw()
{
	m_pDraw->Draw();
}

float TurnaroundTime(const Vehicle* v, glm::vec2 TargetPos)
{
	glm::vec2 ToTarget = glm::normalize(TargetPos - v->GetPosition());

	float dot = glm::dot(v->GetHeading(), ToTarget);

	const float coefficient = 0.5f;

	return (dot - 1.0f) * (-coefficient);
}

glm::vec2 TransformPosition(const glm::mat4 Trans, glm::vec2 pos)
{
	glm::vec3 NewPos = Trans * glm::vec4(pos, 0.0f, 1.0f);
	return glm::vec2(NewPos.x, NewPos.y);
}
