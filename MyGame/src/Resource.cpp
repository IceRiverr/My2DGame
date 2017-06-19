
#include "Resource.h"
#include "Shape.h"
#include "Effect.h"
#include "Texture.h"
#include "Engine.h"

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

std::map<CBuildInResource::TYPE, IResource*> CBuildInResource::m_LookUpTable;

void CBuildInResource::Regsiter()
{
	// SHADER_SIMPLE_VS
	CShader* pShader = GetResourceFactory()->Create<CShader>(RESOURCE_TYPE::RESOURCE_SHADER);
	pShader->Init(GL_VERTEX_SHADER, GetBaseDirectory() + "shader\\DefaultVS.glsl");
	m_LookUpTable.insert(std::pair<TYPE, IResource*>(SHADER_DEFAULT_VS, pShader));
	
	// SHADER_SIMPLE_PS
	pShader = GetResourceFactory()->Create<CShader>(RESOURCE_TYPE::RESOURCE_SHADER);
	pShader->Init(GL_FRAGMENT_SHADER, GetBaseDirectory() + "shader\\DefaultPS.glsl");
	m_LookUpTable.insert(std::pair<TYPE, IResource*>(SHADER_DEFAULT_PS, pShader));

	// SHADER_SOLID_COLOR_VS
	pShader = GetResourceFactory()->Create<CShader>(RESOURCE_TYPE::RESOURCE_SHADER);
	pShader->Init(GL_VERTEX_SHADER, GetBaseDirectory() + "shader\\SolidColorVS.glsl");
	m_LookUpTable.insert(std::pair<TYPE, IResource*>(SHADER_SOLID_COLOR_VS, pShader));

	// SHADER_SOLID_COLOR_PS
	pShader = GetResourceFactory()->Create<CShader>(RESOURCE_TYPE::RESOURCE_SHADER);
	pShader->Init(GL_FRAGMENT_SHADER, GetBaseDirectory() + "shader\\SolidColorPS.glsl");
	m_LookUpTable.insert(std::pair<TYPE, IResource*>(SHADER_SOLID_COLOR_PS, pShader));

	// EFFECT_SPRITE
	CSpriteEffect* pSpriteEffect = GetResourceFactory()->Create<CSpriteEffect>(RESOURCE_TYPE::RESOURCE_EFFECT);
	pSpriteEffect->Init();
	m_LookUpTable.insert(std::pair<TYPE, IResource*>(EFFECT_SPRITE, pSpriteEffect));

	// EFFECT_SOLID_COLOR
	CSolidColorEffect* pSolidEffect = GetResourceFactory()->Create<CSolidColorEffect>(RESOURCE_TYPE::RESOURCE_EFFECT);
	pSolidEffect->Init();
	m_LookUpTable.insert(std::pair<TYPE, IResource*>(EFFECT_SOLID_COLOR, pSolidEffect));

	// MESH_DATA_QUAD
	CMeshData* pMesh = GetResourceFactory()->Create<CMeshData>(RESOURCE_TYPE::RESOURCE_MESH_DATA);
	pMesh->m_nNumVertex = 4;
	pMesh->m_nNumIndex = 6;
	pMesh->m_Positions =
	{
		-1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
	};
	pMesh->m_UVs =
	{
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};
	pMesh->m_Indices =
	{
		0,1,2,
		0,2,3,
	};
	m_LookUpTable.insert(std::pair<TYPE, IResource*>(MESH_DATA_QUAD, pMesh));

	// MESH_DATA_QUAD_LINE
	pMesh = GetResourceFactory()->Create<CMeshData>(RESOURCE_TYPE::RESOURCE_MESH_DATA);
	pMesh->m_nNumVertex = 4;
	pMesh->m_nNumIndex = 8;
	pMesh->m_Positions =
	{
		-1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
	};
	pMesh->m_Indices =
	{
		0, 1, 1, 2,
		2, 3, 3, 0,
	};
	m_LookUpTable.insert(std::pair<TYPE, IResource*>(MESH_DATA_QUAD_LINE, pMesh));

	// MESH_DATA_CIRCLE
	pMesh = GetResourceFactory()->Create<CMeshData>(RESOURCE_TYPE::RESOURCE_MESH_DATA);
	int circleSegment = 16;
	pMesh->m_nNumVertex = circleSegment + 1;
	pMesh->m_nNumIndex = circleSegment * 3;
	pMesh->m_Positions.resize(pMesh->m_nNumVertex * 3);
	pMesh->m_Indices.resize(pMesh->m_nNumIndex);
	for (int i = 0; i < circleSegment; ++i)
	{
		double radians = i * PI_2 / circleSegment;
		float x = (float)std::cos(radians);
		float y = (float)std::sin(radians);
		float z = 0.0f;

		pMesh->m_Positions[i * 3 + 0] = x;
		pMesh->m_Positions[i * 3 + 1] = y;
		pMesh->m_Positions[i * 3 + 2] = z;
	}
	pMesh->m_Positions[circleSegment * 3 + 0] = 0.0f;
	pMesh->m_Positions[circleSegment * 3 + 1] = 0.0f;
	pMesh->m_Positions[circleSegment * 3 + 2] = 0.0f;
	for (int i = 0; i < circleSegment; ++i)
	{
		pMesh->m_Indices[i * 3 + 0] = i;
		pMesh->m_Indices[i * 3 + 1] = i + 1;
		pMesh->m_Indices[i * 3 + 2] = circleSegment;
	}
	pMesh->m_Indices[(circleSegment - 1) * 3 + 0] = circleSegment - 1;
	pMesh->m_Indices[(circleSegment - 1) * 3 + 1] = 0;
	pMesh->m_Indices[(circleSegment - 1) * 3 + 2] = circleSegment;
	m_LookUpTable.insert(std::pair<TYPE, IResource*>(MESH_DATA_CIRCLE, pMesh));

	// MESH_DATA_CIRCLE_LINE
	pMesh = GetResourceFactory()->Create<CMeshData>(RESOURCE_TYPE::RESOURCE_MESH_DATA);
	circleSegment = 16;
	pMesh->m_nNumVertex = circleSegment;
	pMesh->m_nNumIndex = circleSegment * 2;
	pMesh->m_Positions.resize(pMesh->m_nNumVertex * 3);
	pMesh->m_Indices.resize(pMesh->m_nNumIndex);
	for (int i = 0; i < circleSegment; ++i)
	{
		double radians = i * PI_2 / circleSegment;
		float x = (float)std::cos(radians);
		float y = (float)std::sin(radians);
		float z = 0.0f;

		pMesh->m_Positions[i * 3 + 0] = x;
		pMesh->m_Positions[i * 3 + 1] = y;
		pMesh->m_Positions[i * 3 + 2] = z;
	}
	for (int i = 0; i < circleSegment; ++i)
	{
		pMesh->m_Indices[i * 2 + 0] = i;
		pMesh->m_Indices[i * 2 + 1] = i + 1;
	}
	pMesh->m_Indices[(circleSegment - 1) * 2 + 0] = circleSegment - 1;
	pMesh->m_Indices[(circleSegment - 1) * 2 + 1] = 0;
	m_LookUpTable.insert(std::pair<TYPE, IResource*>(MESH_DATA_CIRCLE_LINE, pMesh));

	// SHAPE_SPRITE
	CShape* pShape = GetResourceFactory()->Create<CShape>(RESOURCE_TYPE::RESOURCE_SHAPE);
	pShape->Init(CBuildInResource::GetResource<CMeshData>(CBuildInResource::MESH_DATA_QUAD), VERTEX_INDEX_POSITION_UV);
	m_LookUpTable.insert(std::pair<TYPE, IResource*>(SHAPE_SPRITE, pShape));

	// SHAPE_QUAD
	pShape = GetResourceFactory()->Create<CShape>(RESOURCE_TYPE::RESOURCE_SHAPE);
	pShape->Init(CBuildInResource::GetResource<CMeshData>(CBuildInResource::MESH_DATA_QUAD), VERTEX_INDEX_POSITION);
	m_LookUpTable.insert(std::pair<TYPE, IResource*>(SHAPE_QUAD, pShape));

	// SHAPE_QUAD_LINE
	pShape = GetResourceFactory()->Create<CShape>(RESOURCE_TYPE::RESOURCE_SHAPE);
	pShape->Init(CBuildInResource::GetResource<CMeshData>(CBuildInResource::MESH_DATA_QUAD_LINE), VERTEX_INDEX_POSITION);
	m_LookUpTable.insert(std::pair<TYPE, IResource*>(SHAPE_QUAD_LINE, pShape));

	// SHAPE_CIRCLE
	pShape = GetResourceFactory()->Create<CShape>(RESOURCE_TYPE::RESOURCE_SHAPE);
	pShape->Init(CBuildInResource::GetResource<CMeshData>(TYPE::MESH_DATA_CIRCLE), VERTEX_INDEX_POSITION);
	m_LookUpTable.insert(std::pair<TYPE, IResource*>(SHAPE_CIRCLE, pShape));

	// SHAPE_CIRCLE_LINE
	pShape = GetResourceFactory()->Create<CShape>(RESOURCE_TYPE::RESOURCE_SHAPE);
	pShape->Init(CBuildInResource::GetResource<CMeshData>(TYPE::MESH_DATA_CIRCLE_LINE), VERTEX_INDEX_POSITION);
	m_LookUpTable.insert(std::pair<TYPE, IResource*>(SHAPE_CIRCLE_LINE, pShape));
}
