#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>

CTexture2D::CTexture2D()
	: IResource()
{
	
}

CTexture2D::~CTexture2D()
{
	glDeleteTextures(1, &m_SourceTexture);
}

void CTexture2D::Init(const std::string& filePath)
{
	m_FilePath = filePath;
	glGenTextures(1, &m_SourceTexture);
	glBindTexture(GL_TEXTURE_2D, m_SourceTexture);
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
	glBindTexture(GL_TEXTURE_2D, m_SourceTexture);
}
