
#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class CCamera
{
public:
	CCamera(float left, float right, float bottom, float top);
	CCamera(float left, float right, float bottom, float top, float near, float far);
	~CCamera();
	
	void Update(float dt);

	void SetViewRect(float left, float right, float bottom, float top);
	void SetPosition(const glm::vec3& pos);
	void SetClipPlan(float near, float far);

public:
	glm::mat4	m_mViewMat;
	glm::mat4	m_mProjectionMat;

private:
	glm::vec3	m_vPosition;

	float		m_fLeft;
	float		m_fRight;
	float		m_fBottom;
	float		m_fTop;

	float		m_fNear;
	float		m_fFar;

	bool		m_bViewDirty;
	bool		m_bProjDirty;
};
