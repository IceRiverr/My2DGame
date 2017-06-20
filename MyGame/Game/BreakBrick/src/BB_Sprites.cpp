#include "BB_Sprites.h"
#include "Engine.h"

CBallRacket::CBallRacket()
{
	m_fMoveSpeed = 300.0f;
	m_fHeight = 32.0f;
	m_MoveDir = MOVE_DIR::MOVE_PAUSE;
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
		pos.x += -m_fMoveSpeed * dt;
	}
	else if (m_MoveDir == MOVE_DIR::MOVE_RIGHT)
	{
		pos.x += m_fMoveSpeed * dt;
	}

	if (pos.x < -GetMainWindow()->m_nWidth / 2.0f + m_BoundingBox.x)
		pos.x = -GetMainWindow()->m_nWidth / 2.0f + m_BoundingBox.x;
	if (pos.x > GetMainWindow()->m_nWidth / 2.0f - m_BoundingBox.x)
		pos.x = GetMainWindow()->m_nWidth / 2.0f - m_BoundingBox.x;
	

	
	pos.y = m_fHeight;

	SetPosiiton(pos);

	CSpriteObject::Update(dt);
}

void CBallRacket::Draw()
{
	CSpriteObject::Draw();
}

void CBallRacket::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		m_MoveDir = MOVE_DIR::MOVE_RIGHT;
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		m_MoveDir = MOVE_DIR::MOVE_LEFT;
	else
		m_MoveDir = MOVE_DIR::MOVE_PAUSE;
}

void CBallRacket::SetBoundingBox(float w, float h)
{
	m_BoundingBox.x = w;
	m_BoundingBox.y = h;
}

const glm::vec2& CBallRacket::GetBoundingBox() const
{
	return m_BoundingBox;
}

void CBallRacket::SetHeight(float h)
{
	m_fHeight = h;
}

void CBallRacket::SetMoveSpeed(float speed)
{
	m_fMoveSpeed = speed;
}
