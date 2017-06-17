
#include "Shape.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>

CShape::CShape()
{
	
}

CShape::~CShape()
{
	
}

void CShape::Init()
{
	float vertices[] =
	{
		-1.0f, -1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		-1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,		1.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f
	};

	uint indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CShape::Bind()
{
	glBindVertexArray(m_VAO);
}
