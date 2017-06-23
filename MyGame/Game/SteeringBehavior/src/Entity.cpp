
#include "Entity.h"
#include <iostream>
#include "FontLib.h"
#include "Entity.h"

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

	m_vScale = glm::vec2(16.0f, 16.0f);
	m_fBRadius = 10.0f;
	//m_fRotation = 1.0f;
	m_fMass = 1.0f;
	m_fMaxSpeed = 100.0f;
}

Vehicle::~Vehicle()
{
	DELETE_PTR(m_pDraw);

}

void Vehicle::Init(CShape* shape)
{
	m_pDraw = new Drawable();
	m_pDraw->Init();
	m_pDraw->SetShape(shape);
	m_pDraw->SetColor(glm::vec4(1.0f, 0.2f, 0.3f, 1.0f));

	m_pSteering = new SteeringBehaviors(this);
}

void Vehicle::Update(float dt)
{
	glm::vec2 SteeringForce = m_pSteering->Calculate();

	//DebugValue("Force", SteeringForce);

	glm::vec2 acceleration = SteeringForce / m_fMass;

	m_vVelocity += acceleration * dt;

	m_vVelocity = Truncate(m_vVelocity, m_fMaxSpeed);

	m_vPos += m_vVelocity * dt;

	if (LengthSqr(m_vVelocity) > 0.0000001f)
	{
		m_vHeading = glm::normalize(m_vVelocity);
		m_vSide = Perp(m_vHeading);
	}

	m_fRotation = Vec2ToRadian(m_vHeading) - (float)PI * 0.5f;

	// DebugValue("m_fRotation", m_fRotation);

	m_pDraw->SetPosiiton(glm::vec3(m_vPos, 1.0f));
	m_pDraw->SetScale(m_vScale.x , m_vScale.y);
	m_pDraw->SetRotate(m_fRotation);
	m_pDraw->Update(dt);
}

void Vehicle::Draw()
{
	m_pDraw->Draw();
}

SteeringBehaviors::SteeringBehaviors(Vehicle* v)
	: m_pVehicle(v)
{
	m_fSeekWeight = 1.0f;
	m_fFleeWeight = 1.0f;
}

SteeringBehaviors::~SteeringBehaviors()
{
	m_pVehicle = nullptr;
}

glm::vec2 SteeringBehaviors::Calculate()
{
	glm::vec2 force;

	// force = Seek(glm::vec2(550.0f, 583.0f)) * m_fSeekWeight;

	//force = Flee(glm::vec2(410.0f, 310.0f)) * m_fFleeWeight;

	force = Arrive(glm::vec2(500.0f, 500.0f), DECELERATION::FAST) * 1.0f;

	return force;
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

	float LookAheadTime = ToEvader.length() / (m_pVehicle->m_fMaxSpeed + evader->GetSpeed());
	LookAheadTime += TurnaroundTime(m_pVehicle, evader->GetPosition());
	return Seek(evader->m_vPos + evader->m_vVelocity * LookAheadTime);
}

float TurnaroundTime(const Vehicle* v, glm::vec2 TargetPos)
{
	glm::vec2 ToTarget = glm::normalize(TargetPos - v->GetPosition());

	float dot = glm::dot(v->GetHeading(), ToTarget);

	const float coefficient = 0.5f;

	return (dot - 1.0f) * (-coefficient);
}

glm::vec2 Truncate(glm::vec2 in, float length)
{
	float inLen = glm::length(in);
	if (inLen <= length)
		return in;
	else
	{
		return in / inLen * length;
	}
}

float LengthSqr(const glm::vec2 in)
{
	return in.x * in.x + in.y * in.y;
}

glm::vec2 Perp(glm::vec2 in)
{
	glm::vec3 sideDir = glm::cross(glm::vec3(in, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec2 perp = glm::vec2(sideDir.x, sideDir.y);
	perp = glm::normalize(perp);
	return perp;
}

float Vec2ToRadian(glm::vec2 in)
{
	in = glm::normalize(in);
	float radian = 0.0f;
	if (in.y >= 0.0f)
	{
		radian = std::acos(in.x);
	}
	else
	{
		radian = (float)PI_2 - std::acos(in.x);
	}
	return radian;
}

void DebugValue(std::string msg, glm::vec2 v)
{
	std::cout << msg << "\t" << v.x << "\t\t" << v.y << std::endl;
}

void DebugValue(std::string msg, float v)
{
	std::cout << msg << "\t" << v << std::endl;
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
