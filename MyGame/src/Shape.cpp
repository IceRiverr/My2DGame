
#include "Shape.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <vector>
#include "MathLib.h"

CMeshData::CMeshData()
	: IResource()
{

}

bool CMeshData::IsDataValid(VERTEX_TYPE type)
{
	// can impove
	bool bDataValid = false;
	if (type == VERTEX_TYPE::VERTEX_INDEX_POSITION)
	{
		bDataValid = (m_Positions.size() > 0);
	}
	else if (type == VERTEX_TYPE::VERTEX_INDEX_POSITION_UV)
	{
		bDataValid = (m_Positions.size() > 0) && (m_UVs.size() > 0);
	}
	return bDataValid;
}

CShape::CShape()
	: IResource()
	, m_pMeshData(nullptr)
	, m_VertexBufferUsage(GL_STATIC_DRAW)
{
	
}

CShape::~CShape()
{
	RELEASE_PTR(m_pMeshData);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
	glDeleteVertexArrays(1, &m_VAO);
}

void CShape::Init(CMeshData* pMesh, VERTEX_TYPE type)
{
	m_VertexType = type;

	m_BBox = CalcBBox(pMesh);

	RELEASE_PTR(m_pMeshData);
	m_pMeshData = pMesh;
	m_pMeshData->AddRef();

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	if (m_VertexType == VERTEX_TYPE::VERTEX_INDEX_POSITION && pMesh->IsDataValid(m_VertexType))
	{
		glBufferData(GL_ARRAY_BUFFER, m_pMeshData->m_Positions.size() * sizeof(float), nullptr, m_VertexBufferUsage);

		glBufferSubData(GL_ARRAY_BUFFER, 0,
			m_pMeshData->m_Positions.size() * sizeof(float),
			&m_pMeshData->m_Positions[0]);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			m_pMeshData->m_Indices.size() * sizeof(uint),
			&m_pMeshData->m_Indices[0],
			GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
	else if (m_VertexType == VERTEX_TYPE::VERTEX_INDEX_POSITION_UV && pMesh->IsDataValid(m_VertexType))
	{
		glBufferData(GL_ARRAY_BUFFER, m_pMeshData->m_Positions.size() * sizeof(float) + m_pMeshData->m_UVs.size() * sizeof(float), nullptr, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0,
			m_pMeshData->m_Positions.size() * sizeof(float),
			&m_pMeshData->m_Positions[0]);
		glBufferSubData(GL_ARRAY_BUFFER, m_pMeshData->m_Positions.size() * sizeof(float),
			m_pMeshData->m_UVs.size() * sizeof(float),
			&m_pMeshData->m_UVs[0]);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			m_pMeshData->m_Indices.size() * sizeof(uint),
			&m_pMeshData->m_Indices[0],
			GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(m_pMeshData->m_Positions.size() * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CShape::Bind()
{
	glBindVertexArray(m_VAO);
}

void CShape::SetVBUsage(uint usage)
{
	m_VertexBufferUsage = usage;
}

BBox CShape::CalcBBox(CMeshData* pMesh)
{
	float minX = pMesh->m_Positions[0];
	float maxX = pMesh->m_Positions[0];
	float minY = pMesh->m_Positions[1];
	float maxY = pMesh->m_Positions[1];

	for (int i = 1; i < pMesh->m_nNumVertex ; ++i)
	{
		minX = Math::Min(minX, pMesh->m_Positions[i * 3 + 0]);
		maxX = Math::Max(maxX, pMesh->m_Positions[i * 3 + 0]);
		minY = Math::Min(minY, pMesh->m_Positions[i * 3 + 1]);
		maxY = Math::Max(maxY, pMesh->m_Positions[i * 3 + 1]);
	}

	BBox bb;
	bb.pos = glm::vec2((minX + maxX) * 0.5f, (minY + maxY) * 0.5f);
	bb.size = glm::vec2(maxX - minX, maxY - minY);
	
	return bb;
}
