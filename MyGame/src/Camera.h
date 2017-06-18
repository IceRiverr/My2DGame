
#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class CCamera
{
public:
	CCamera(int width, int height);
	CCamera(int width, int height, float near, float far);
	~CCamera();
	
	void Update(float dt);
	void Resize(int width, int height);

	void SetPosition(const glm::vec3& pos);
	void SetClipPlan(float near, float far);

public:
	glm::mat4 m_mViewMat;
	glm::mat4 m_mProjectionMat;

private:
	glm::vec3 m_vPosition;
	float m_fNear;
	float m_fFar;
	int m_nScreenWidth;
	int m_nScreenHeight;

	bool m_bViewDirty;
	bool m_bProjDirty;
};
