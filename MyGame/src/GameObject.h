
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

class CSceneManager
{
public:
	CSceneManager();
	~CSceneManager();

	void Update(float dt);
	void Draw();
	void ProcessEvent(GLFWwindow* window);
	
	void AddGameObject(IGameObject* obj);
private:
	std::list<IGameObject*> m_GameObjectPool;
};


class IGameObject
{
public:
	virtual ~IGameObject() {}
	virtual void Init() = 0;
	virtual	void Update(float dt) = 0;
	virtual void Draw() = 0;
	virtual void ProcessInput(GLFWwindow* window) = 0;
	virtual void SetDestroyFlag() = 0;
	virtual bool GetDestroyFlag() { return false; }
};

class CBaseObject : public IGameObject
{
public:
	CBaseObject();
	~CBaseObject();

	void Init();
	void Update(float dt);
	void Draw();
	void ProcessInput(GLFWwindow* window);

	void SetShape(CShape* pShape);
	void SetPosiiton(float x, float y, float z);
	void SetPosiiton(const glm::vec3& pos);
	void SetRotate(float radian);
	void SetScale(float sx = 32.0f, float sy = 32.0f);
	void SetDestroyFlag();
	bool GetDestroyFlag();
	
	const glm::vec3& GetPosition() const {return m_vPosition;}

protected:
	CShape*			m_pShape;
	glm::mat4		m_mModelMatrix;

private:
	glm::vec3		m_vPosition;
	float			m_fRotate;
	glm::vec2		m_vScale;

	bool			m_bMatrixDirty;
	bool			m_bDestroyFlag;
};

class CSpriteObject : public CBaseObject
{
public:
	CSpriteObject();
	~CSpriteObject();

	void Init();
	void Update(float dt);
	void Draw();
	void ProcessInput(GLFWwindow* window);
	
	void SetSprite(ITexture* pSprite);

protected:
	void SetEffect(CSpriteEffect* pEffect);

protected:
	CSpriteEffect*		m_pEffect;
	ITexture*			m_pSprite;
};

class CSolidColorObject : public CBaseObject
{
public:
	CSolidColorObject();
	~CSolidColorObject();

	void Init();
	void Update(float dt);
	void Draw();
	void ProcessInput(GLFWwindow* window);
	
	void SetColor(const glm::vec4& color);

protected:
	void SetEffect(CSolidColorEffect* pEffect);

protected:
	CSolidColorEffect*	m_pEffect;
	glm::vec4			m_vColor;
};

class CLineObject : public CSolidColorObject
{
public:
	CLineObject();
	~CLineObject();

	void Draw();
};
