
#include "Resource.h"
#include "Shape.h"
#include "Effect.h"
#include "Texture.h"

IResource::IResource()
	: CRefCounter(this)
{
}

IResource::~IResource()
{
}

CResourceFactory::CResourceFactory()
{

}

CResourceFactory::~CResourceFactory()
{
	for (auto it = m_ResourcePool.begin();  it  != m_ResourcePool.end();)
	{
		IResource* pResource = *it;
		RELEASE_PTR(pResource);
		m_ResourcePool.erase(it);
		it = m_ResourcePool.begin();
	}
}
