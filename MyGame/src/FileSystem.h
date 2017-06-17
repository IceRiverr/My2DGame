#pragma once

#include <string>

bool find_path_in(const std::string& searchPath, const std::string& toSerch, std::string& resultPath, bool bSerchSubPath);

class CFileSystem
{
public:
	void SetBaseDirectoryName(const std::string& baseName);
	const std::string& GetBaseDirectory() const { return m_BaseDirectory; }

private:
	
	std::string m_BaseName;
	std::string m_BaseDirectory;
};
