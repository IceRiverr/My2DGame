
#pragma once

#include "TypeDefine.h"
#include "Resource.h"
#include <string>
#include <glm\glm.hpp>

class ITexture : public IResource
{
public:
	ITexture() {}
	~ITexture() {}
	virtual void Bind() = 0;
};

class CTexture2D : public ITexture
{
public:
	CTexture2D();
	~CTexture2D();

	void Init(const std::string& filePath);
	void Bind();
private:
	uint			m_TextureID;
	std::string		m_FilePath;
};

class CCharTexture : public ITexture
{
public:
	CCharTexture();
	~CCharTexture();

	void Init(int charCode);
	void Bind();
private:
	uint			m_TextureID;
	uint			m_CharCode;
	glm::ivec2		m_Size;
	glm::ivec2		m_Bearing;
	uint			m_Advance;
};