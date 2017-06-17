
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

class CBaseObject : public IGameObject
{
public:
	CBaseObject();
	~CBaseObject();

	void Init();
	void Update();
	void Draw();
	void ProcessInput(GLFWwindow* window);

	void SetShape(CShape* pShape);
	void SetPosiiton(const glm::vec3& pos);
	void SetRotate(float angle);
	void SetScale(float scale);

protected:
	CShape*			m_pShape;

	glm::vec3		m_vPosition;
	float			m_fRotate;
	float			m_fScale;
	glm::mat4		m_mModelMatrix;

	bool			m_bMatrixDirty;
};

class CSpriteObject : public CBaseObject
{
public:
	CSpriteObject();
	~CSpriteObject();

	void Init();
	void Update();
	void Draw();
	void ProcessInput(GLFWwindow* window);
	
	void SetEffect(CSpriteEffect* pEffect);
	void SetSprite(CTexture2D* pSprite);
protected:
	CSpriteEffect*		m_pEffect;
	CTexture2D*			m_pSprite;
};

class CSolidColorObject : public CBaseObject
{
public:
	CSolidColorObject();
	~CSolidColorObject();

	void Init();
	void Update();
	void Draw();
	void ProcessInput(GLFWwindow* window);
	
	void SetColor(const glm::vec4& color);
	void SetEffect(CSolidColorEffect* pEffect);
protected:
	CSolidColorEffect*	m_pEffect;
	glm::vec4			m_vColor;
};

class CLineShape : public CSolidColorObject
{
public:
	CLineShape();
	~CLineShape();

	void Draw();
};