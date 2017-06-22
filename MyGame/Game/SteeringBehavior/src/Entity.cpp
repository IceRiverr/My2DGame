
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
	m_fRotation = 1.0f;
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
}

void Vehicle::Update(float dt)
{
	glm::vec2 SteeringForce = m_pSteering->Calculate();

	glm::vec2 acceleration = SteeringForce / m_fMass;

	m_vVelocity += acceleration * dt;

	m_vVelocity = Truncate(m_vVelocity, m_fMaxSpeed);

	m_vPos += m_vVelocity * dt;

	if (LengthSqr(m_vVelocity) > 0.0000001f)
	{
		m_vHeading = glm::normalize(m_vVelocity);
		m_vSide = Perp(m_vHeading);
	}

	m_pDraw->SetPosiiton(glm::vec3(m_vPos, 1.0f));
	m_pDraw->SetScale(m_vScale.x , m_vScale.y);
	m_pDraw->SetRotate(glm::degrees(m_fRotation));
	m_pDraw->Update(dt);
}

void Vehicle::Draw()
{
	m_pDraw->Draw();
}

glm::vec2 SteeringBehaviors::Calculate()
{
	return glm::vec2();
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
