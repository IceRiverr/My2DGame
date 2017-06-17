
#pragma once

#include "TypeDefine.h"
#include "Resource.h"
#include <string>

class CTexture2D : public IResource
{
public:
	CTexture2D();
	~CTexture2D();

	void Init(const std::string& filePath);
	void Bind();

public:

private:
	uint			m_SourceTexture;
	std::string		m_FilePath;
};