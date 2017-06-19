
#include "FontLib.h"
#include <iostream>
#include "Engine.h"
#include "Effect.h"

std::vector<TextInfo> CFontLib::m_TextInfos;

CFontLib::CFontLib()
	: m_FTlib(nullptr)
	, m_FTface(nullptr)
	, m_nHeight(16)
{
	m_FontColor = glm::vec3(0.0f, 1.0f, 0.5f);
}

CFontLib::~CFontLib()
{
	FT_Done_Face(m_FTface);
	FT_Done_FreeType(m_FTlib);
}

void CFontLib::Init()
{
	if (FT_Init_FreeType(&m_FTlib))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	
	if (FT_New_Face(m_FTlib, std::string(GetBaseDirectory() + "resource\\arial.ttf").c_str(), 0, &m_FTface))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	Build_ASCII_Table();

	m_pEffect = GetResourceFactory()->Create<CFontEffect>(RESOURCE_TYPE::RESOURCE_EFFECT);
	m_pEffect->Init();

	CMeshData* pCopyMesh = CBuildInResource::GetResource<CMeshData>(CBuildInResource::MESH_DATA_QUAD);
	
	CMeshData* pMesh = GetResourceFactory()->Create<CMeshData>(RESOURCE_TYPE::RESOURCE_MESH_DATA);
	
	pMesh->m_nNumIndex = pCopyMesh->m_nNumIndex;
	pMesh->m_nNumVertex = pCopyMesh->m_nNumVertex;
	pMesh->m_Indices = pCopyMesh->m_Indices;
	pMesh->m_Positions = pCopyMesh->m_Positions;
	pMesh->m_Colors = pCopyMesh->m_Colors;
	pMesh->m_UVs = pCopyMesh->m_UVs;

	m_pShape = GetResourceFactory()->Create<CShape>(RESOURCE_TYPE::RESOURCE_SHAPE);
	m_pShape->SetVBUsage(GL_DYNAMIC_DRAW);
	m_pShape->Init(pMesh, VERTEX_INDEX_POSITION_UV);

	m_BaseMatrix = glm::mat4();
	m_BaseMatrix = glm::translate(m_BaseMatrix, glm::vec3(-CEngine::GetEngine()->m_gScreenWidth / 2, -CEngine::GetEngine()->m_gScreenHeight / 2, 0.0f));
	m_BaseMatrix = glm::scale(m_BaseMatrix, glm::vec3(1.0, 1.0, 1.0));

	DrawTextAt("Hello, Boy, Good Night! Long long age, a witcher find a evil method to have dead to live...", 10, 10, 16);
}

void CFontLib::Update(float dt)
{
}

void CFontLib::Draw()
{
	if (m_pShape == nullptr || m_pEffect == nullptr)
		return;

	m_pShape->Bind();
	m_pEffect->Bind();

	CMeshData* pMesh = m_pShape->GetMeshData();
	uint VBO = m_pShape->GetVBO();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	for (int i = 0; i < m_TextInfos.size(); ++i)
	{
		const TextInfo& info = m_TextInfos[i];

		m_pEffect->BindParameters(m_BaseMatrix, info.color);

		float x = info.x;
		float y = info.y;
		float scale = info.h * 1.0f / m_nHeight;

		for (auto c = info.text.begin(); c != info.text.end(); ++c)
		{
			CCharTexture* ch = Get_ASCII_Texture(*c);
			ch->Bind();

			float xpos = x + ch->m_Bearing.x * scale;
			float ypos = y - (ch->m_Size.y - ch->m_Bearing.y) * scale;
			float w = ch->m_Size.x * scale;
			float h = ch->m_Size.y * scale;

			pMesh->m_Positions = {
				xpos,		ypos,		0.0f,
				xpos,		ypos + h,	0.0f,
				xpos + w,	ypos + h,	0.0f,
				xpos + w,	ypos,		0.0f,
			};

			glBufferSubData(GL_ARRAY_BUFFER, 0, pMesh->m_Positions.size() * sizeof(float), &pMesh->m_Positions[0]);
			glDrawElements(GL_TRIANGLES, m_pShape->GetMeshData()->m_nNumIndex, GL_UNSIGNED_INT, 0);
			x += (ch->m_Advance >> 6) * scale;
		}
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CFontLib::DrawTextAt(const std::string& text, float x, float y, float h /*= 32.0f*/, glm::vec3 color /*= glm::vec3(0.0f, 0.0f, 0.0f)*/, float z /*= 0.0f*/)
{
	TextInfo info;
	info.text = text;
	info.color = color;
	info.x = x;
	info.y = y;
	info.z = z;
	info.h = h;
	
	m_TextInfos.push_back(info);
}

void CFontLib::Build_ASCII_Table()
{
	for (int c = 0; c < 128; ++c)
	{
		CCharTexture* pChar1Texture = GetResourceFactory()->Create<CCharTexture>(RESOURCE_TYPE::RESOURCE_TEXTURE);
		pChar1Texture->Init(c);
		m_ASCII_Table.insert(std::pair<int, CCharTexture*>(c, pChar1Texture));
	}
}

CCharTexture* CFontLib::Get_ASCII_Texture(uchar c)
{
	if (c >= 128)
		return nullptr;

	return m_ASCII_Table[c];
}

void CFontLib::SetFontHeight(int h)
{
	m_nHeight = h;
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
	CFontLib* fontLib = CEngine::GetEngine()->m_gFontLib;

	m_CharCode = charCode;

	FT_Set_Pixel_Sizes(fontLib->m_FTface, 0, fontLib->m_nHeight);

	if (FT_Load_Char(fontLib->m_FTface, m_CharCode, FT_LOAD_RENDER))
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);// 将纹理 1 位对齐，因为只有r通道有数据

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		fontLib->m_FTface->glyph->bitmap.width, fontLib->m_FTface->glyph->bitmap.rows, 0,
		GL_RED, GL_UNSIGNED_BYTE, fontLib->m_FTface->glyph->bitmap.buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	m_Size = glm::ivec2(fontLib->m_FTface->glyph->bitmap.width, fontLib->m_FTface->glyph->bitmap.rows);
	m_Bearing = glm::ivec2(fontLib->m_FTface->glyph->bitmap_left, fontLib->m_FTface->glyph->bitmap_top);
	m_Advance = fontLib->m_FTface->glyph->advance.x;
}

void CCharTexture::Bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

