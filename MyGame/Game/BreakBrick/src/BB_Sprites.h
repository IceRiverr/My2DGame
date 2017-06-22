
#pragma once
#include "GameObject.h"
#include "Physics2DLib.h"

bool ClacLineCrossBBox(const glm::vec2& start, const glm::vec2& end, const glm::vec2& dir, const BBox& bb, glm::vec2& newP, glm::vec2& newDir);

enum COLLIDER_TYPE
{
	COLLIDER_RACKET,
	COLLIDER_BRICK
};

class ICollider
{
public:
	virtual const BBox& GetBBox() = 0;
	virtual COLLIDER_TYPE GetColliderType() const= 0;
};

class CScene
{
public:
	std::vector<ICollider*> m_Colliders;
};

class CBallRacket : public CSpriteObject, public ICollider
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

	void SetBBoxSize(float w, float h);
	const BBox& GetBBox();
	void SetHeight(float h);
	void SetMoveSpeed(float speed);
	COLLIDER_TYPE GetColliderType() const { return COLLIDER_TYPE::COLLIDER_RACKET; }
	float GetAddSpeed()const; // + ÏòÓÒ£¬- Ïò×ó

private:
	BBox			m_BBox;
	float			m_fMoveSpeed;
	float			m_fHeight;
	MOVE_DIR		m_MoveDir;
	float			m_fAddSpeed;
	float			m_fAdd;
};

class CBall : public CSpriteObject
{
public: 
	CBall();
	~CBall();

	void Init();
	void Update(float dt);
	void Draw();

	void SetSceneRef(CScene* c);
private:
	CScene*			m_pScene;	// dirty code
	float			m_fRadius;
	glm::vec2		m_Dir;
	float			m_fMoveSpeed;
};

class CBrick : public CSpriteObject, public ICollider
{
public:
	CBrick();
	~CBrick();

	void Init();
	void Update(float dt);
	void Draw();

	const BBox& GetBBox();
	void SetBBoxSize(float w, float h);

	COLLIDER_TYPE GetColliderType() const { return COLLIDER_TYPE::COLLIDER_BRICK; }

private:
	BBox			m_BBox;
};
