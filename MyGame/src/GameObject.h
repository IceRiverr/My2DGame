
#pragma once

#include "Effect.h"
#include "Shape.h"
#include "Texture.h"

#include <list>
#include <string>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class IGameObject;

class CGameObjectManager
{
public:
	CGameObjectManager();
	~CGameObjectManager();
	
	void AddGameObject(IGameObject* obj);
private:
	std::list<IGameObject*> m_GameObjectPool;
};


class IGameObject
{
public:
	virtual ~IGameObject() {}
	virtual void Init() = 0;
	virtual	void Update() = 0;
	virtual void Draw() = 0;
	virtual void ProcessInput(GLFWwindow* window) = 0;
};

class CSpriteObject : public IGameObject
{
public:
	CSpriteObject();
	~CSpriteObject();

	void Init();
	void Update();
	void Draw();
	void ProcessInput(GLFWwindow* window);

	void SetShape(CShape* pShape);
	void SetEffect(CSpriteEffect* pEffect);
	void SetSprite(CTexture2D* pSprite);

	void SetPosiiton(const glm::vec3& pos);
	void SetRotate(float angle);
	void SetScale(float scale);

private:
	CShape*			m_pShape;
	CSpriteEffect*	m_pEffect;
	CTexture2D*		m_pSprite;

	glm::vec3		m_vPosition;
	float			m_fRotate;
	float			m_fScale;
	glm::mat4		m_mModelMatrix;

	bool			m_bMatrixDirty;
};