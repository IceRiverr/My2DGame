
#include "Shape.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>

CMeshData::CMeshData()
	: IResource()
{

}

CShape::CShape()
	: IResource()
	, m_pMeshData(nullptr)
{
	
}

CShape::~CShape()
{
	RELEASE_PTR(m_pMeshData);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
	glDeleteVertexArrays(1, &m_VAO);
}

void CShape::Init(CMeshData* pMesh)
{
	RELEASE_PTR(m_pMeshData);
	m_pMeshData = pMesh;
	m_pMeshData->AddRef();

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 
		m_pMeshData->m_Positions.size() * sizeof(float) + m_pMeshData->m_Colors.size() * sizeof(float) + m_pMeshData->m_UVs.size() * sizeof(float), 
		nullptr, 
		GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0,
		m_pMeshData->m_Positions.size() * sizeof(float), 
		&m_pMeshData->m_Positions[0]);
	glBufferSubData(GL_ARRAY_BUFFER, m_pMeshData->m_Positions.size() * sizeof(float),
		m_pMeshData->m_Colors.size() * sizeof(float), 
		&m_pMeshData->m_Colors[0]);
	glBufferSubData(GL_ARRAY_BUFFER, m_pMeshData->m_Positions.size() * sizeof(float) + m_pMeshData->m_Colors.size(),
		m_pMeshData->m_UVs.size() * sizeof(float), 
		&m_pMeshData->m_UVs[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		m_pMeshData->m_Indices.size() * sizeof(uint), 
		&m_pMeshData->m_Indices[0], 
		GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(m_pMeshData->m_Positions.size() * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(m_pMeshData->m_Positions.size() * sizeof(float) + m_pMeshData->m_Colors.size()));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CShape::Bind()
{
	glBindVertexArray(m_VAO);
}

