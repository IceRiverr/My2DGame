
#include "Effect.h"
#include "Engine.h"

#include <fstream>
#include <sstream>
#include <iostream>

uint CreateShader(GLenum type, std::string shaderFile)
{
	uint shader = -1;
	shader = glCreateShader(type);

	std::ifstream file;
	file.open(shaderFile);
	if (!file.is_open())
	{
		std::cout << "ERROR: Shader file : " << shaderFile << " no exit." << std::endl;
		return -1;
	}

	std::string shaderCode;
	std::stringstream shaderStream;

	shaderStream << file.rdbuf();
	shaderCode = shaderStream.str();

	const GLchar* codeChar = shaderCode.c_str();

	glShaderSource(shader, 1, &codeChar, nullptr);
	glCompileShader(shader);
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
		std::cout << "ERROR: Shader compile failed: " << infoLog << std::endl;
		return -1;
	}
	return shader;
}

uint LinkShaderProgram(uint vs, uint ps)
{
	uint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, ps);
	glLinkProgram(shaderProgram);
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
		std::cout << "ERROR: Shader link failed: " << infoLog << std::endl;
		return -1;
	}
	return shaderProgram;
}

CShader::CShader()
	: IResource()
	, m_Shader(0)
{
}

CShader::~CShader()
{
	Delete();
}

void CShader::Init(GLenum type, std::string shaderFile)
{
	m_ShaderType = type;
	m_ShaderFile = shaderFile;
	m_Shader = CreateShader(m_ShaderType, m_ShaderFile);
}

void CShader::Delete()
{
	glDeleteShader(m_Shader);
}

CEffect::CEffect()
	: m_ShaderProgram(0)
	, m_VertexShader(nullptr)
	, m_FragmentShader(nullptr)
{

}

CEffect::~CEffect()
{
	RELEASE_PTR(m_VertexShader);
	RELEASE_PTR(m_FragmentShader);

	glDeleteProgram(m_ShaderProgram);
}

void CEffect::Bind()
{
	glUseProgram(m_ShaderProgram);
}

void CEffect::LinkShader(CShader * vs, CShader * ps)
{
	RELEASE_PTR(m_VertexShader);
	RELEASE_PTR(m_FragmentShader);
	
	m_VertexShader = vs;
	m_FragmentShader = ps;
	m_VertexShader->AddRef();
	m_FragmentShader->AddRef();

	m_ShaderProgram = LinkShaderProgram(m_VertexShader->GetShader(), m_FragmentShader->GetShader());
}

CSpriteEffect::CSpriteEffect()
{

}

CSpriteEffect::~CSpriteEffect()
{

}

void CSpriteEffect::Init()
{
	LinkShader(
		CBuildInResource::GetResource<CShader>(CBuildInResource::SHADER_DEFAULT_VS),
		CBuildInResource::GetResource<CShader>(CBuildInResource::SHADER_DEFAULT_PS)
	);

	m_ModelMatLocation = glGetUniformLocation(m_ShaderProgram, "modelMat");
	m_ViewMatLocation = glGetUniformLocation(m_ShaderProgram, "viewMat");
	m_ProjectMatLocation = glGetUniformLocation(m_ShaderProgram, "projectMat");
	
	m_SpriteTexLocation = glGetUniformLocation(m_ShaderProgram, "Sprite2D");
}

void CSpriteEffect::BindParameters(const glm::mat4 & model)
{
	glUniformMatrix4fv(m_ModelMatLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(m_ViewMatLocation, 1, GL_FALSE, glm::value_ptr(CEngine::GetEngine()->m_gCamera->m_mViewMat));
	glUniformMatrix4fv(m_ProjectMatLocation, 1, GL_FALSE, glm::value_ptr(CEngine::GetEngine()->m_gCamera->m_mProjectionMat));
	
	glUniform1i(m_SpriteTexLocation, 0);
}

CSolidColorEffect::CSolidColorEffect()
{
}

CSolidColorEffect::~CSolidColorEffect()
{
}

void CSolidColorEffect::Init()
{
	LinkShader(
		CBuildInResource::GetResource<CShader>(CBuildInResource::SHADER_SOLID_COLOR_VS),
		CBuildInResource::GetResource<CShader>(CBuildInResource::SHADER_SOLID_COLOR_PS)
	);

	m_ModelMatLocation = glGetUniformLocation(m_ShaderProgram, "modelMat");
	m_ViewMatLocation = glGetUniformLocation(m_ShaderProgram, "viewMat");
	m_ProjectMatLocation = glGetUniformLocation(m_ShaderProgram, "projectMat");
	m_VertexColorLocation = glGetUniformLocation(m_ShaderProgram, "vertexColor");
}

void CSolidColorEffect::BindParameters(const glm::mat4 & model, glm::vec4 vertexColor)
{
	glUniformMatrix4fv(m_ModelMatLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(m_ViewMatLocation, 1, GL_FALSE, glm::value_ptr(CEngine::GetEngine()->m_gCamera->m_mViewMat));
	glUniformMatrix4fv(m_ProjectMatLocation, 1, GL_FALSE, glm::value_ptr(CEngine::GetEngine()->m_gCamera->m_mProjectionMat));
	glUniform4f(m_VertexColorLocation, vertexColor.r, vertexColor.g, vertexColor.b,vertexColor.a);
}

CFontEffect::CFontEffect()
{

}

CFontEffect::~CFontEffect()
{

}

void CFontEffect::Init()
{
	CShader* pVSShader = CBuildInResource::GetResource<CShader>(CBuildInResource::SHADER_DEFAULT_VS);
	CShader* pPSShader = pPSShader = GetResourceFactory()->Create<CShader>(RESOURCE_TYPE::RESOURCE_SHADER);
	pPSShader->Init(GL_FRAGMENT_SHADER, GetBaseDirectory() + "shader\\FontPS.glsl");

	LinkShader(pVSShader, pPSShader);

	m_ModelMatLocation = glGetUniformLocation(m_ShaderProgram, "modelMat");
	m_ViewMatLocation = glGetUniformLocation(m_ShaderProgram, "viewMat");
	m_ProjectMatLocation = glGetUniformLocation(m_ShaderProgram, "projectMat");

	m_CharacterTexLocation = glGetUniformLocation(m_ShaderProgram, "CharacterTex");
	m_FontColorLocation = glGetUniformLocation(m_ShaderProgram, "FontColor");
}

void CFontEffect::BindParameters(const glm::mat4& model, glm::vec3 fontColor)
{
	glUniformMatrix4fv(m_ModelMatLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(m_ViewMatLocation, 1, GL_FALSE, glm::value_ptr(CEngine::GetEngine()->m_gCamera->m_mViewMat));
	glUniformMatrix4fv(m_ProjectMatLocation, 1, GL_FALSE, glm::value_ptr(CEngine::GetEngine()->m_gCamera->m_mProjectionMat));

	glUniform1i(m_CharacterTexLocation, 0);
	glUniform3f(m_FontColorLocation, fontColor.r, fontColor.g, fontColor.b);
}
