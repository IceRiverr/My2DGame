
#pragma once
#include <list>
#include "SmartPointer.h"

class CMeshData;
class CShape;
class CShader;
class CEffect;
class CTexture2D;

enum RESOURCE_TYPE
{
	RESOURCE_MESH_DATA,
	RESOURCE_SHAPE,
	RESOURCE_SHADER,
	RESOURCE_EFFECT,
	RESOURCE_TEXTURE,
};

class IResource : public CRefCounter<IResource>
{
public:
	IResource();
	~IResource();
private:
};

class CResourceFactory
{
public:
	CResourceFactory();
	~CResourceFactory();
	
	template<typename T>
	T* Create(RESOURCE_TYPE type); // only resource type can be create
	
	template<typename T>
	bool Remove(RESOURCE_TYPE type, T* pResource);

private:
	std::list<IResource*> m_ResourcePool;
};

template<typename T>
T* CResourceFactory::Create(RESOURCE_TYPE type)
{
	IResource* pResource = new T();
	if (pResource == nullptr)
	{
		return nullptr;
	}
	else
	{
		m_ResourcePool.push_back(pResource);
		return static_cast<T*>(pResource);
	}
}

template<typename T>
bool CResourceFactory::Remove(RESOURCE_TYPE type, T* pResource)
{
	IResource* pToDelete = static_cast<T*>(pResource);
	for (auto it = m_ResourcePool.begin(); it != m_ResourcePool.end(); ++it)
	{
		IResource* pFind = *it;
		if (pFind == pResource)
		{
			RELEASE_PTR(pFind);
			m_ResourcePool.erase(it);
			return true;
		}
	}
	return false;
}
