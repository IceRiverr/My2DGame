#pragma once

#include <string>
#include <map>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "TypeDefine.h"
#include "Resource.h"

uint LinkShaderProgram(uint vs, uint ps);
uint CreateShader(GLenum type, std::string shaderFile);

class CShader;
class CShaderManager
{
public:
	CShaderManager();
	~CShaderManager();
	void Init();
	
	// note: AddRef
	CShader* GetShader(std::string name);

private:
	std::map<std::string, CShader*> m_Shaders;
};

class CShader : public IResource
{
public:
	CShader();
	~CShader();
	void Init(GLenum type, std::string shaderFile);
	void Delete();

	inline uint GetShader() { return m_Shader; }
	
private:
	GLenum			m_ShaderType;
	std::string		m_ShaderFile;
	uint			m_Shader;
};

class CEffect : public IResource
{
public:
	CEffect();
	~CEffect();
	
	virtual void Init() = 0;
	void Bind();

protected:
	void LinkShader(CShader* vs, CShader* ps);

protected:
	CShader* m_VertexShader;
	CShader* m_FragmentShader;
	uint m_ShaderProgram;

	int m_ModelMatLocation;
	int m_ViewMatLocation;
	int m_ProjectMatLocation;
};

class CSpriteEffect : public CEffect
{
public:
	CSpriteEffect();
	~CSpriteEffect();

	void Init();
	void BindParameters(const glm::mat4& model);

private:
	int m_SrcTexLocation;
};

class CSolidColorEffect : public CEffect
{
public:
	CSolidColorEffect();
	~CSolidColorEffect();
	
	void Init();
	void BindParameters(const glm::mat4& model, glm::vec4 vertexColor);

private:
	int m_VertexColorLocation;
};
