
#include "FileSystem.h"
#include <direct.h>
#include <io.h>
#include <iostream>

bool find_path_in(const std::string& searchPath, const std::string& toSerch, std::string& resultPath, bool bSerchSubPath)
{
	bool bFind = false;

	_finddata_t findData;
	intptr_t handle;
	handle = _findfirst(std::string(searchPath + "\\*.*").c_str(), &findData);
	if (handle == -1)
	{
		std::cout << "ERROR: no such dir" << searchPath << std::endl;
		bFind = false;
		return bFind;
	}

	while (_findnext(handle, &findData) == 0)
	{
		if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
		{
			continue;
		}

		if (strcmp(findData.name, toSerch.c_str()) == 0)
		{
			bFind = true;
			resultPath = searchPath;
			resultPath.append("\\").append(toSerch).append("\\");
			break;
		}

		if (bSerchSubPath && findData.attrib == _A_SUBDIR)
		{
			std::string subSerchPath = searchPath + "\\" + findData.name;
			bFind = find_path_in(subSerchPath, toSerch, resultPath, bSerchSubPath);
			if (bFind)
				break;
		}
	}
	return bFind;
}

void find_Path()
{
	std::string gameName = "MyGame";
	std::string resourceName = "resource";
	std::string shaderName = "shader";

	std::string resourcePath;
	std::string shaderPath;
	bool bFindResource = false;
	bool bFindShader = false;

	char buf[256];
	_getcwd(buf, sizeof(buf));
	std::string path = buf;

	size_t pos = path.find(gameName);
	path = path.substr(0, pos + gameName.size());

	bool bFind = false;
	bFindResource = find_path_in(path, resourceName, resourcePath, true);
	bFindShader = find_path_in(path, shaderName, shaderPath, true);

	if (bFindResource && bFindShader)
		bFind = true;

	int test = 10;
}

void CFileSystem::SetBaseDirectoryName(const std::string& baseName)
{
	m_BaseName = baseName;

	char buf[256];
	_getcwd(buf, sizeof(buf));
	std::string path = buf;

	size_t pos = path.find(m_BaseName);
	m_BaseDirectory = path.substr(0, pos + m_BaseName.size()).append("\\");
}
