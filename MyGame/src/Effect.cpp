
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


CShader::CShader(GLenum type, std::string shaderFile)
	: CRefCounter(this)
	, m_ShaderType(type)
	, m_ShaderFile(shaderFile)
	, m_Shader(0)
{
	m_Shader = CreateShader(m_ShaderType, m_ShaderFile);
}

CShader::~CShader()
{
	Delete();
}

void CShader::Delete()
{
	if (m_Shader == 0)
		glDeleteShader(m_Shader);
}

CEffect::CEffect()
	: CRefCounter(this)
	, m_ShaderProgram(0)
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
	: CEffect()
{

}

CSpriteEffect::~CSpriteEffect()
{
	CEffect::~CEffect();
}

void CSpriteEffect::Init()
{
	CShaderManager* shaderMgr = CEngine::GetEngine()->m_gShaderMgr;
	LinkShader(shaderMgr->GetShader("simpleVS"), shaderMgr->GetShader("simplePS"));

	m_ModelMatLocation = glGetUniformLocation(m_ShaderProgram, "modelMat");
	m_ViewMatLocation = glGetUniformLocation(m_ShaderProgram, "viewMat");
	m_ProjectMatLocation = glGetUniformLocation(m_ShaderProgram, "projectMat");
	m_SrcTexLocation = glGetUniformLocation(m_ShaderProgram, "spriteTexture2D");
}

void CSpriteEffect::BindParameters(const glm::mat4 & model)
{
	glUniformMatrix4fv(m_ModelMatLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(m_ViewMatLocation, 1, GL_FALSE, glm::value_ptr(CEngine::GetEngine()->m_gCamera->m_mViewMat));
	glUniformMatrix4fv(m_ProjectMatLocation, 1, GL_FALSE, glm::value_ptr(CEngine::GetEngine()->m_gCamera->m_mProjectionMat));
	glUniform1i(m_SrcTexLocation, 0);
}

CShaderManager::CShaderManager()
{
}

CShaderManager::~CShaderManager()
{
	for (auto it = m_Shaders.begin(); it != m_Shaders.end();)
	{
		RELEASE_PTR(it->second);
		m_Shaders.erase(it++);
	}
}

void CShaderManager::Init()
{
	CShader* pSimpleVS = new CShader(GL_VERTEX_SHADER, "../shader/simpleVS.glsl");
	CShader* pSimplePS = new CShader(GL_FRAGMENT_SHADER, "../shader/simplePS.glsl");

	m_Shaders.insert(std::pair<std::string, CShader*>("simpleVS", pSimpleVS));
	m_Shaders.insert(std::pair<std::string, CShader*>("simplePS", pSimplePS));
}

CShader * CShaderManager::GetShader(std::string name)
{
	auto it = m_Shaders.find(name);
	if (it != m_Shaders.end())
		return it->second;
	return nullptr;
}
