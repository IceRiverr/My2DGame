
#include "Texture.h"
#include "Engine.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H  

CTexture2D::CTexture2D()
{
	
}

CTexture2D::~CTexture2D()
{
	glDeleteTextures(1, &m_TextureID);
}

void CTexture2D::Init(const std::string& filePath)
{
	m_FilePath = filePath;
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, numChannels;
	uchar* data = stbi_load(m_FilePath.c_str(), &width, &height, &numChannels, 0);
	if (data)
	{
		if (numChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (numChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
		{
			std::cout << "ERROR: image no supprot" << std::endl;
			return;
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR: load image failed." << std::endl;
		return;
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CTexture2D::Bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

CCharTexture::CCharTexture()
{

}

CCharTexture::~CCharTexture()
{
	glDeleteTextures(1, &m_TextureID);
}

void CCharTexture::Init(int charCode)
{
	m_CharCode = charCode;
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face face;
	if (FT_New_Face(ft, std::string(GetBaseDirectory() + "resource\\simsun.ttc").c_str(), 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 48);
	
	if (FT_Load_Char(face, m_CharCode, FT_LOAD_RENDER))
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
		face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, 
		GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	m_Size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
	m_Bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
	m_Advance = face->glyph->advance.x;

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void CCharTexture::Bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}
