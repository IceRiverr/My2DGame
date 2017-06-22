#include "BB_Sprites.h"
#include "Engine.h"
#include <iostream>
#include <GLFW\glfw3.h>
#include <sstream>

CBallRacket::CBallRacket()
{
	m_fMoveSpeed =500.0f;
	m_fHeight = 32.0f;
	m_MoveDir = MOVE_DIR::MOVE_PAUSE;
	m_fAddSpeed = 0.0f;
	m_fAdd = 2.0f;
}

CBallRacket::~CBallRacket()
{

}

void CBallRacket::Init()
{
	CSpriteObject::Init();
	SetShape(CBuildInResource::GetResource<CShape>(CBuildInResource::SHAPE_SPRITE));
}

void CBallRacket::Update(float dt)
{
	glm::vec3 pos = GetPosition();
	
	if (m_MoveDir == MOVE_DIR::MOVE_LEFT)
	{
		pos.x -= m_fMoveSpeed * dt;
		m_fAddSpeed -= m_fAdd * dt;
		m_fAddSpeed = glm::clamp(m_fAddSpeed, -1.0f, -0.2f);
	}
	else if (m_MoveDir == MOVE_DIR::MOVE_RIGHT)
	{
		pos.x += m_fMoveSpeed * dt;
		m_fAddSpeed += m_fAdd * dt;
		m_fAddSpeed = glm::clamp(m_fAddSpeed, 0.2f, 1.0f);
	}
	else if (m_MoveDir == MOVE_DIR::MOVE_PAUSE)
	{
		m_fAddSpeed = 0.0f;
	}

	if (pos.x < m_BBox.size.x / 2.0f)
		pos.x = m_BBox.size.x / 2.0f;
	if (pos.x > GetMainWindow()->m_nWidth - m_BBox.size.x / 2.0f)
		pos.x = GetMainWindow()->m_nWidth - m_BBox.size.x / 2.0f;
	
	pos.y = m_fHeight;

	m_BBox.pos.x = pos.x;
	m_BBox.pos.y = pos.y;

	SetPosiiton(pos);

	CSpriteObject::Update(dt);
}

void CBallRacket::Draw()
{
	std::stringstream ss;
	ss << "AddSpeed: " << m_fAddSpeed;
	CFontLib::DrawTextAt(ss.str(), 2.0f, GetMainWindow()->m_nHeight - 40.0f, 16);

	CSpriteObject::Draw();
}

void CBallRacket::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		m_MoveDir = MOVE_DIR::MOVE_RIGHT;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		m_MoveDir = MOVE_DIR::MOVE_LEFT;
	}
	else
	{
		m_MoveDir = MOVE_DIR::MOVE_PAUSE;
	}
}

void CBallRacket::SetBBoxSize(float w, float h)
{
	m_BBox.size.x = w;
	m_BBox.size.y = h;
}

const BBox& CBallRacket::GetBBox()
{
	return m_BBox;
}

void CBallRacket::SetHeight(float h)
{
	m_fHeight = h;
}

void CBallRacket::SetMoveSpeed(float speed)
{
	m_fMoveSpeed = speed;
}

float CBallRacket::GetAddSpeed() const
{
	return m_fAddSpeed;
}

CBall::CBall()
{
	m_fRadius = 10.0f;
	m_Dir = glm::vec2(1.0f, 2.0f);
	m_Dir = glm::normalize(m_Dir);
	m_fMoveSpeed = 1000.0f;
	m_pScene = nullptr;
}

CBall::~CBall()
{
	
}

void CBall::Init()
{
	CSpriteObject::Init();
	SetShape(CBuildInResource::GetResource<CShape>(CBuildInResource::SHAPE_SPRITE));
}

void CBall::Update(float dt)
{
	glm::vec3 lastPos = GetPosition();

	glm::vec3 newPos = lastPos + glm::vec3(m_Dir, 0.0f) * m_fMoveSpeed * dt;

	if (newPos.y > GetMainWindow()->m_nHeight - m_fRadius)
	{
		m_Dir.y = -m_Dir.y;
		newPos.y = newPos.y - 2.0f * (newPos.y - (GetMainWindow()->m_nHeight - m_fRadius));
	}
	else if (newPos.y < 0 + m_fRadius)
	{
		m_Dir.y = -m_Dir.y;
		newPos.y = newPos.y + 2.0f * (0 + m_fRadius - newPos.y);
	}

	if (newPos.x > GetMainWindow()->m_nWidth - m_fRadius)
	{
		m_Dir.x = -m_Dir.x;
		newPos.x = newPos.x - 2.0f * (newPos.x - (GetMainWindow()->m_nWidth - m_fRadius));
	}
	else if (newPos.x < 0 + m_fRadius)
	{
		m_Dir.x = -m_Dir.x;
		newPos.x = newPos.x + 2.0f * (0 + m_fRadius - newPos.x);
	}

	glm::vec2 afterPos, afterDir;

	for (int i = 0; i < m_pScene->m_Colliders.size(); ++i)
	{
		ICollider* c = m_pScene->m_Colliders[i];
		if (c != nullptr)
		{
			BBox bb = c->GetBBox();
			bb.size.x += 2 * m_fRadius;
			bb.size.y += 2 * m_fRadius;
			
			if (ClacLineCrossBBox(lastPos, newPos, m_Dir, bb, afterPos, afterDir))
			{
				COLLIDER_TYPE type = c->GetColliderType();
				if (type == COLLIDER_RACKET)
				{
					CBallRacket* pRacket = static_cast<CBallRacket*>(c);
					float xSpeed = pRacket->GetAddSpeed();
					afterDir.x += xSpeed;
					afterDir = glm::normalize(afterDir);
					break;
				}
				else if (type == COLLIDER_BRICK)
				{
					CBrick* pBrick = static_cast<CBrick*>(c);
					pBrick->SetDestroyFlag();
					m_pScene->m_Colliders[i] = nullptr;
					break;
				}
			}
		}
	}
	
	m_Dir = afterDir;

	SetPosiiton(glm::vec3(afterPos, 0.0f));

	CSpriteObject::Update(dt);
}

void CBall::Draw()
{
	CSpriteObject::Draw();
}

void CBall::SetSceneRef(CScene* c)
{
	m_pScene = c;
}

bool ClacLineCrossBBox(const glm::vec2& start, const glm::vec2& end, const glm::vec2& dir, const BBox& bb, glm::vec2& newP, glm::vec2& newDir)
{
	// Left 
	glm::vec2 leftCrossPoint;

	glm::vec2 p1 = glm::vec2(bb.pos.x - bb.size.x * 0.5f, bb.pos.y - bb.size.y * 0.5f);
	glm::vec2 p2 = glm::vec2(bb.pos.x - bb.size.x * 0.5f, bb.pos.y + bb.size.y * 0.5f);
	glm::vec2 p3 = glm::vec2(bb.pos.x + bb.size.x * 0.5f, bb.pos.y + bb.size.y * 0.5f);
	glm::vec2 p4 = glm::vec2(bb.pos.x + bb.size.x * 0.5f, bb.pos.y - bb.size.y * 0.5f);
	
	if (glm::dot(dir, glm::vec2(-1.0f, 0.0f)) <= 0.0f && LineRectCrossTest(start, end, p1, p2) && LineSegmentCrossTest(start, end, p1, p2))
	{
		newP.x = end.x - 2.0f * (end.x - p1.x);
		newP.y = end.y;
		newDir = dir;
		newDir.x = -newDir.x;
		return true;
	}
	// Right
	if (glm::dot(dir, glm::vec2(1.0f, 0.0f)) <= 0.0f && LineRectCrossTest(start, end, p3, p4) && LineSegmentCrossTest(start, end, p3, p4))
	{
		newP.x = end.x + 2.0f * (p3.x - end.x);
		newP.y = end.y;
		newDir = dir;
		newDir.x = -newDir.x;
		return true;
	}
	// Bottom
	if (glm::dot(dir, glm::vec2(0.0f, -1.0f)) <= 0.0f && LineRectCrossTest(start, end, p4, p1) && LineSegmentCrossTest(start, end, p4, p1))
	{
		newP.x = end.x;
		newP.y = end.y - 2.0f * (end.y - p4.y);
		newDir = dir;
		newDir.y = -newDir.y;
		return true;
	}
	// Top
	if (glm::dot(dir, glm::vec2(0.0f, 1.0f)) <= 0.0f && LineRectCrossTest(start, end, p2, p3) && LineSegmentCrossTest(start, end, p2, p3))
	{
		newP.x = end.x;
		newP.y = end.y + 2.0f * (p2.y - end.y);
		newDir = dir;
		newDir.y = -newDir.y;
		return true;
	}

	newP = end;
	newDir = dir;
	return false;
}

CBrick::CBrick()
{

}

CBrick::~CBrick()
{

}

void CBrick::Init()
{
	CSpriteObject::Init();
	SetShape(CBuildInResource::GetResource<CShape>(CBuildInResource::SHAPE_SPRITE));
}

void CBrick::Update(float dt)
{
	glm::vec3 pos = GetPosition();
	m_BBox.pos.x = pos.x;
	m_BBox.pos.y = pos.y;

	CSpriteObject::Update(dt);
}

void CBrick::Draw()
{
	CSpriteObject::Draw();
}

const BBox& CBrick::GetBBox()
{
	return m_BBox;
}

void CBrick::SetBBoxSize(float w, float h)
{
	m_BBox.size.x = w;
	m_BBox.size.y = h;
}
