#include "KUtility.h"

KConstBuffDataMaterial KModelMaterial::GetConstBuffMaterial()
{
	KConstBuffDataMaterial result;
	result.ambient = ambient;
	result.diffuse = diffuse;
	result.specular = specular;
	result.alpha = alpha;
	return result;
}

ComputeRelation::ComputeRelation()
{
}

ComputeRelation::~ComputeRelation()
{
}

KTextureData::~KTextureData()
{
}
