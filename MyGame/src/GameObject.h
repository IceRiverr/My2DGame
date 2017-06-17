
#pragma once

#include "Effect.h"
#include "Shape.h"
#include "Resource.h"

#include <string>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class CGameObject
{
public:
	CGameObject();
	~CGameObject();

	void Init();
	void Update();
	void Draw();
	void ProcessInput(GLFWwindow* window);

	void SetPosiiton(const glm::vec3& pos);
	void SetRotate(float angle);
	void SetScale(float scale);
	void SetSpriteImage(const std::string& filePath);

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