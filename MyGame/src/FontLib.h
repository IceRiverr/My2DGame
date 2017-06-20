
#pragma once
#include "Texture.h"
#include <map>
#include <vector>
#include <ft2build.h>
#include "Effect.h"
#include FT_FREETYPE_H  

class CCharTexture;

struct TextInfo
{
	std::string		text;
	glm::vec3		color;
	float			x;
	float			y;
	float			z;
	float			h;
};

class CFontLib
{
	friend class CCharTexture;
public:
	CFontLib();
	~CFontLib();

	void Init();
	void Update(float dt);
	void Draw();

	static void DrawTextAt(const std::string& text, float x, float y, float h = 16.0f, glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f), float z = 0.0f);

	void Build_ASCII_Table();
	CCharTexture* Get_ASCII_Texture(uchar c);
	void SetFontHeight(int h);

private:
	FT_Library			m_FTlib;
	FT_Face				m_FTface;
	int					m_nHeight;

	CFontEffect*		m_pEffect;
	CShape*				m_pShape;
	glm::mat4			m_BaseMatrix;
	glm::vec3			m_FontColor;

	std::map<int, CCharTexture*>	m_ASCII_Table;
	static std::vector<TextInfo>	m_TextInfos;
};

class CCharTexture : public ITexture
{
	friend class CFontLib;
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
