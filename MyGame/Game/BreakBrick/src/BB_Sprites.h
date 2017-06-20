
#pragma once
#include "GameObject.h"

class CBallRacket : public CSpriteObject
{
public:
	CBallRacket();
	~CBallRacket();

	enum MOVE_DIR
	{
		MOVE_LEFT = 0,
		MOVE_RIGHT,
		MOVE_PAUSE,
	};

	void Init();
	void Update(float dt);
	void Draw();
	void ProcessInput(GLFWwindow* window);

	void SetBoundingBox(float w, float h);
	const glm::vec2& GetBoundingBox() const;
	void SetHeight(float h);
	void SetMoveSpeed(float speed);

private:
	glm::vec2		m_BoundingBox;
	float			m_fMoveSpeed;
	float			m_fHeight;
	MOVE_DIR		m_MoveDir;
};

class CBrick : public CSpriteObject
{
public:
	CBrick();
	~CBrick();

	void Update(float dt);
	void Draw();
	
private:
	glm::vec2	m_Size;

};
