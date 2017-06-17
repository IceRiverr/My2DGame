#include "Camera.h"

CCamera::CCamera(int width, int height)
	: m_vPosition(0.0f, 0.0f, -10.0f)
	, m_fNear(0.1f)
	, m_fFar(100.0f)
	, m_nScreenWidth(width)
	, m_nScreenHeight(height)
	, m_bViewDirty(true)
	, m_bProjDirty(true)
{

}

CCamera::CCamera(int width, int height, float near, float far)
	: m_vPosition(0.0f, 0.0f, -10.0f)
	, m_fNear(near)
	, m_fFar(far)
	, m_nScreenWidth(width)
	, m_nScreenHeight(height)
{
}

CCamera::~CCamera()
{

}

void CCamera::Resize(int width, int height)
{
	m_nScreenWidth = width;
	m_nScreenHeight = height;
	m_bProjDirty = true;
}

void CCamera::SetPosition(const glm::vec3& pos)
{
	m_vPosition = pos;
	m_bViewDirty = true;
}

void CCamera::SetClipPlan(float near, float far)
{
	m_fNear = near;
	m_fFar = far;
	m_bProjDirty = true;
}

void CCamera::Update()
{
	if (m_bViewDirty == true)
	{
		m_mViewMat = glm::mat4();
		m_mViewMat = glm::translate(m_mViewMat, m_vPosition);
		m_bViewDirty = false;
	}
	if (m_bProjDirty)
	{
		m_mProjectionMat = glm::ortho(-m_nScreenWidth / 2.0f, m_nScreenWidth / 2.0f, -m_nScreenHeight / 2.0f, m_nScreenHeight / 2.0f, m_fNear, m_fFar);
		// projectionMat = glm::perspective(glm::radians(45.0f), (float)800.0f / (float)600.0f, 0.1f, 100.0f);
		m_bProjDirty = false;
	}
}
