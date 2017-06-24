
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

BaseGameEntity::BaseGameEntity()
{

}

BaseGameEntity::~BaseGameEntity()
{

}

void BaseGameEntity::SetPosition(float x, float y)
{
	m_vPos.x = x;
	m_vPos.y = y;
}

void BaseGameEntity::SetScale(float sx, float sy)
{
	m_vScale.x = sx;
	m_vScale.y = sy;
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

	m_vScale = glm::vec2(8.0f, 8.0f);
	m_fBRadius = 10.0f;
	//m_fRotation = 1.0f;
	m_fMass = 1.0f;
	m_fMaxSpeed = 200.0f;
	m_fMaxForce = 200.0f;
}

Vehicle::~Vehicle()
{
	DELETE_PTR(m_pDraw);

}

void Vehicle::Start(CShape* shape)
{
	m_pDraw = new Drawable();
	m_pDraw->Init();
	m_pDraw->SetShape(shape);
	m_pDraw->SetColor(glm::vec4(1.0f, 0.2f, 0.3f, 1.0f));

	m_pSteering = new SteeringBehaviors(this);
	
}

void Vehicle::Update(float dt)
{
	glm::vec2 SteeringForce = m_pSteering->Calculate(dt);

	glm::vec2 acceleration = SteeringForce / m_fMass;

	m_vVelocity += acceleration * dt;

	m_vVelocity = Truncate(m_vVelocity, m_fMaxSpeed);

	m_vPos += m_vVelocity * dt;

	if (m_vPos.x < 0.0f)
		m_vPos.x += GetMainWindow()->m_nWidth;
	if (m_vPos.x > GetMainWindow()->m_nWidth)
		m_vPos.x -= GetMainWindow()->m_nWidth;
	if (m_vPos.y < 0.0f)
		m_vPos.y += GetMainWindow()->m_nHeight;
	if (m_vPos.y > GetMainWindow()->m_nHeight)
		m_vPos.y -= GetMainWindow()->m_nHeight;

	if (LengthSqr(m_vVelocity) > 0.0000001f)
	{
		m_vHeading = glm::normalize(m_vVelocity);
		m_vSide = Perp(m_vHeading);
	}

	m_fRotation = UnitVec2ToRadian(m_vHeading) - (float)PI * 0.5f;

	m_pDraw->SetPosiiton(glm::vec3(m_vPos, 1.0f));
	m_pDraw->SetScale(m_vScale.x , m_vScale.y);
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

SteeringBehaviors::SteeringBehaviors(Vehicle* v)
	: m_pVehicle(v)
{
	m_BehaviorFlag = 0;

	m_fSeekWeight = 1.0f;
	m_fFleeWeight = 1.0f;
	m_fWanderWeight = 1.0f;

	m_fWanderRadius = 50.0f;
	m_fWanderDistance = 2.0f;
	m_fWanderJitter = 1000.0f;

	float random = (float)RandomUnit();
	m_vWanderTarget = glm::vec2(std::cos(random), std::sin(random)) * m_fWanderRadius;
}

SteeringBehaviors::~SteeringBehaviors()
{
	m_pVehicle = nullptr;
}

glm::vec2 SteeringBehaviors::Calculate(float dt)
{
	m_vSteeringForce = CalculateWeightedSum(dt);

	return m_vSteeringForce;
}

glm::vec2 SteeringBehaviors::CalculateWeightedSum(float dt)
{
	// force = Seek(glm::vec2(550.0f, 583.0f)) * m_fSeekWeight;

	// force = Flee(glm::vec2(410.0f, 310.0f)) * m_fFleeWeight;

	// force = Arrive(glm::vec2(500.0f, 500.0f), DECELERATION::FAST) * 1.0f;

	m_vSteeringForce += Wander(dt) * m_fWanderWeight;

	m_vSteeringForce = Truncate(m_vSteeringForce, m_pVehicle->m_fMaxForce);
	
	return m_vSteeringForce;
}

void SteeringBehaviors::OnBehavior(BEHAVIOR_TYPE type)
{
	SetFlag(m_BehaviorFlag, type);
}

void SteeringBehaviors::OffBehavior(BEHAVIOR_TYPE type)
{
	ClearFlag(m_BehaviorFlag, type);
}

glm::vec2 SteeringBehaviors::Seek(glm::vec2 TargetPos)
{
	glm::vec2 DesiredVelocity = glm::normalize(TargetPos - m_pVehicle->m_vPos) * m_pVehicle->m_fMaxSpeed;
	return (DesiredVelocity - m_pVehicle->m_vVelocity);
}

glm::vec2 SteeringBehaviors::Flee(glm::vec2 TargetPos)
{
	const float PanicDistanceSqr = 100.0f * 100.0f;
	if (LengthSqr(m_pVehicle->m_vPos - TargetPos) > PanicDistanceSqr)
		return glm::vec2(0.0f, 0.0f);

	glm::vec2 DesiredVelocity = glm::normalize(m_pVehicle->m_vPos - TargetPos) * m_pVehicle->m_fMaxSpeed;
	return(DesiredVelocity - m_pVehicle->m_vVelocity);
}

glm::vec2 SteeringBehaviors::Arrive(glm::vec2 TargetPos, DECELERATION dec)
{
	glm::vec2 ToTarget = TargetPos - m_pVehicle->m_vPos;
	float dist = glm::length(ToTarget);
	if (dist > 0.01f )
	{
		const float DecelerationTweaker = 0.3f;
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

	float r1 = (float)RandomUniform();
	float r2 = (float)RandomUniform();

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

float TurnaroundTime(const Vehicle* v, glm::vec2 TargetPos)
{
	glm::vec2 ToTarget = glm::normalize(TargetPos - v->GetPosition());

	float dot = glm::dot(v->GetHeading(), ToTarget);

	const float coefficient = 0.5f;

	return (dot - 1.0f) * (-coefficient);
}

void GameWorld::Update(float dt)
{
	for (int i = 0; i < m_Vehicles.size(); ++i)
		m_Vehicles[i]->Update(dt);
}

void GameWorld::Draw()
{
	for (int i = 0; i < m_Vehicles.size(); ++i)
		m_Vehicles[i]->Draw();
}

void GameWorld::AddEntity(Vehicle* v)
{
	m_Vehicles.push_back(v);
}

float MovingEntity::GetSpeed() const
{
	return glm::length(m_vVelocity);
}