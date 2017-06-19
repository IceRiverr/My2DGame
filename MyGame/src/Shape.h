#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "TypeDefine.h"
#include "Resource.h"

enum VERTEX_TYPE
{
	VERTEX_INDEX_POSITION,
	VERTEX_INDEX_POSITION_UV,
	VERTEX_INDEX_POSITION_COLOR,
};

class CMeshData : public IResource
{
public:
	CMeshData();
	bool IsDataValid(VERTEX_TYPE type);
	
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
	
	void Init(CMeshData* pMesh, VERTEX_TYPE type);
	void Bind();

	void SetVBUsage(uint usage);
	const CMeshData* GetMeshData() const { return m_pMeshData; }
	CMeshData* GetMeshData() { return m_pMeshData; }

	uint GetVBO() { return m_VBO; }

private:
	CMeshData*			m_pMeshData;
	VERTEX_TYPE			m_VertexType;
	uint				m_VertexBufferUsage;

	uint				m_VAO;
	uint				m_VBO;
	uint				m_EBO;
};
