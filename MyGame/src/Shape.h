#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "TypeDefine.h"
#include "Resource.h"

class CMeshData : public IResource
{
public:
	CMeshData();
	
	int					m_nNumIndex;
	int					m_nNumVertex;

	std::vector<uint>	m_Indices;
	std::vector<float>	m_Positions;
	std::vector<float>	m_Colors;
	std::vector<float>	m_UVs;
};

class CShape : public IResource
{
public:
	CShape();
	~CShape();
	
	void Init(CMeshData* pMesh);
	void Bind();

private:
	CMeshData*	m_pMeshData;

	uint		m_VAO;
	uint		m_VBO;
	uint		m_EBO;
};