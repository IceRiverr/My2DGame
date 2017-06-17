
#pragma once

#include "TypeDefine.h"
#include "SmartPointer.h"
#include <string>

class CTexture2D : public CRefCounter<CTexture2D>
{
public:
	CTexture2D(const std::string& filePath);
	~CTexture2D();

	void Init();
	void Bind();

public:

private:
	uint			m_SourceTexture;
	std::string		m_FilePath;
};