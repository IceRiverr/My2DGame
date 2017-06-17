#pragma once

#include "TypeDefine.h"

class CShape
{
public:
	CShape();
	~CShape();

	void Init();
	void Bind();

private:
	uint m_VAO;
	uint m_VBO;
	uint m_EBO;
};