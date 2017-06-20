#include "Camera.h"

CCamera::CCamera(float left, float right, float bottom, float top)
	: m_vPosition(0.0f, 0.0f, -10.0f)
	, m_fLeft(left)
	, m_fRight(right)
	, m_fBottom(bottom)
	, m_fTop(top)
	, m_fNear(0.1f)
	, m_fFar(100.0f)
	, m_bViewDirty(true)
	, m_bProjDirty(true)
{

}

CCamera::CCamera(float left, float right, float bottom, float top, float near, float far)
	: m_vPosition(0.0f, 0.0f, -10.0f)
	, m_fLeft(left)
	, m_fRight(right)
	, m_fBottom(bottom)
	, m_fTop(top)
	, m_fNear(near)
	, m_fFar(far)
	, m_bViewDirty(true)
	, m_bProjDirty(true)
{

}

CCamera::~CCamera()
{

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

void CCamera::Update(float dt)
{
	if (m_bViewDirty == true)
	{
		m_mViewMat = glm::mat4();
		m_mViewMat = glm::translate(m_mViewMat, m_vPosition);
		m_bViewDirty = false;
	}
	if (m_bProjDirty)
	{
		m_mProjectionMat = glm::ortho(m_fLeft, m_fRight, m_fBottom, m_fTop, m_fNear, m_fFar);
		m_bProjDirty = false;
	}
}

void CCamera::SetViewRect(float left, float right, float bottom, float top)
{
	m_fLeft = left;
	m_fRight = right;
	m_fBottom = bottom;
	m_fTop = top;
	m_bProjDirty = true;
}
