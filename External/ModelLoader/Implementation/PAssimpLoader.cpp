#include "PAssimpLoader.h"

PHONON_SUPPRESS_WARNINGS_BEGIN

#include<cassert>
#include<list>
#include<filesystem>

PHONON_SUPPRESS_WARNINGS_END

PHONON_LOADER_NAMESPACE_BEGIN

void PAssimpLoader::Load(P_MODEL_DATA* pData,const std::string& filePath)
{
	GetInstance()->_Load(pData,filePath);
}

void PAssimpLoader::SmoothingLoad(P_MODEL_DATA* pData,const std::string& filePath)
{
	GetInstance()->_SmoothingLoad(pData,filePath);
}

PAssimpLoader::PAssimpLoader()
{
}

PAssimpLoader::~PAssimpLoader()
{
}

PAssimpLoader* PAssimpLoader::GetInstance()
{
	static PAssimpLoader instance;
	return &instance;
}

void PAssimpLoader::_Load(P_MODEL_DATA* pData,const std::string& filePath)
{
	assert(pData != nullptr);

	const aiScene* pScene = nullptr;

	std::vector<std::string> files = _GetFileNames(filePath);

	for ( size_t i = 0; i < files.size(); i++ )
	{
		pScene = aiImportFile(files[ i ].c_str(),ASSIMP_LOAD_FLAG_DEFAULT);

		if ( pScene != nullptr )
		{
			break;
		}
	}

	assert(pScene != nullptr);

	pData->canAnimation = false;

	pData->name = pScene->mName.C_Str();

	uint32_t nodeNum = 0;
	_GetNodeNum(pScene->mRootNode,nodeNum);

	pData->nodes.reserve(nodeNum);

	_ConvertMatrix(pData->globalInverseTransform,pScene->mRootNode->mTransformation.Inverse());

	_ParseNodeRecursive(pData,filePath,pScene,pScene->mRootNode);

	for ( uint32_t i = 0; i < pScene->mNumAnimations; i++ )
	{
		P_ANIMATION_DATA& pAnimationData = pData->animations.emplace_back();
		aiAnimation* pAnimation = pScene->mAnimations[ i ];

		pAnimationData.duration = (float)pAnimation->mDuration;
		pAnimationData.ticksPerSecond = ( float ) pAnimation->mTicksPerSecond;
		pAnimationData.name = pAnimation->mName.C_Str();

		for ( uint32_t j = 0; j < pAnimation->mNumChannels; j++ )
		{
			P_MOTION_NODE& pMotionNode = pAnimationData.channels.emplace_back();
			aiNodeAnim* pNodeAnim = pAnimation->mChannels[ j ];

			_ParseNodeAnime(pNodeAnim,pMotionNode);
		}
	}

	aiReleaseImport(pScene);

	pScene = nullptr;
}

void PAssimpLoader::_SmoothingLoad(P_MODEL_DATA* pData,const std::string& filePath)
{
	assert(pData != nullptr);

	const aiScene* pScene = nullptr;

	std::vector<std::string> files = _GetFileNames(filePath);

	for ( size_t i = 0; i < files.size(); i++ )
	{
		pScene = aiImportFile(files[ i ].c_str(),ASSIMP_LOAD_FLAG_SMOOTHING);

		if ( pScene != nullptr )
		{
			break;
		}
	}

	assert(pScene != nullptr);

	pData->canAnimation = false;

	pData->name = pScene->mName.C_Str();

	uint32_t nodeNum = 0;
	_GetNodeNum(pScene->mRootNode,nodeNum);

	pData->nodes.reserve(nodeNum);

	_ConvertMatrix(pData->globalInverseTransform,pScene->mRootNode->mTransformation.Inverse());

	_ParseNodeRecursive(pData,filePath,pScene,pScene->mRootNode);

	for ( uint32_t i = 0; i < pScene->mNumAnimations; i++ )
	{
		P_ANIMATION_DATA& pAnimationData = pData->animations.emplace_back();
		aiAnimation* pAnimation = pScene->mAnimations[ i ];

		pAnimationData.duration = ( float ) pAnimation->mDuration;
		pAnimationData.ticksPerSecond = ( float ) pAnimation->mTicksPerSecond;
		pAnimationData.name = pAnimation->mName.C_Str();

		for ( uint32_t j = 0; j < pAnimation->mNumChannels; j++ )
		{
			P_MOTION_NODE& pMotionNode = pAnimationData.channels.emplace_back();
			aiNodeAnim* pNodeAnim = pAnimation->mChannels[ j ];

			_ParseNodeAnime(pNodeAnim,pMotionNode);
		}
	}

	aiReleaseImport(pScene);

	pScene = nullptr;
}

void PAssimpLoader::_ParseNodeRecursive(P_MODEL_DATA* pData,const std::string& directoryPath,const aiScene* pScene,aiNode* pAiNode,P_NODE* pParent)
{

	P_NODE* pNode = &pData->nodes.emplace_back();
	pNode->name = pAiNode->mName.C_Str();

	pNode->transform =
	{
		pAiNode->mTransformation.a1, pAiNode->mTransformation.a2, pAiNode->mTransformation.a3, pAiNode->mTransformation.a4,
		pAiNode->mTransformation.b1, pAiNode->mTransformation.b2, pAiNode->mTransformation.b3, pAiNode->mTransformation.b4,
		pAiNode->mTransformation.c1, pAiNode->mTransformation.c2, pAiNode->mTransformation.c3, pAiNode->mTransformation.c4,
		pAiNode->mTransformation.d1, pAiNode->mTransformation.d2, pAiNode->mTransformation.d3, pAiNode->mTransformation.d4,
	};

	aiVector3D p;
	aiQuaternion r;
	aiVector3D s;
	pAiNode->mTransformation.Decompose(s,r,p);
	pNode->position = { p.x,p.y,p.z };
	pNode->rotation = { r.x,r.y,r.z,r.w };
	pNode->scale = { s.x,s.y,s.z };


	pNode->globalTransform = pNode->transform;

	if ( pParent )
	{
		pNode->parentNodeName = pParent->name;
		pNode->globalTransform *= pParent->globalTransform;

		pParent->childrenNodeNames.push_back(pNode->name);
	}

	for ( uint32_t i = 0; i < pAiNode->mNumMeshes; i++ )
	{
		aiMesh* pNodeAttribute = pScene->mMeshes[ pAiNode->mMeshes[ i ] ];

		if ( pNodeAttribute )
		{
			P_MODEL_MESH& pMesh = pData->meshes.emplace_back();
			pMesh.name = pNodeAttribute->mName.C_Str();
			pMesh.nodeName = pNode->name;
			pMesh.pNode = pNode;

			_ParseMesh(pData,directoryPath,pScene,pNodeAttribute);
		}
	}

	for ( uint32_t i = 0; i < pAiNode->mNumChildren; i++ )
	{
		_ParseNodeRecursive(pData,directoryPath,pScene,pAiNode->mChildren[ i ],pNode);
	}
}

void PAssimpLoader::_ParseMesh(P_MODEL_DATA* pData,const std::string& directoryPath,const aiScene* pScene,aiMesh* pAiMesh)
{
	_ParseMeshVertices(pData,pAiMesh);
	_ParseMeshFaces(pData,pAiMesh);
	_ParseMaterial(pData,directoryPath,pAiMesh,pScene->mMaterials[ pAiMesh->mMaterialIndex ]);

	if ( pAiMesh->HasBones() )
	{
		_ParseSkin(pData,pAiMesh);
	}
}

void PAssimpLoader::_ParseMeshVertices(P_MODEL_DATA* pData,aiMesh* pAiMesh)
{
	P_MODEL_MESH& modelMesh = pData->meshes.back();

	aiVector3D zero3D(0.0f,0.0f,0.0f);

	P_POS_NORM_UV_TANGE_COL_SKIN vert = {};
	modelMesh.vertices.resize(pAiMesh->mNumVertices);

	for ( uint32_t i = 0; i < pAiMesh->mNumVertices; i++ )
	{
		P_POS_NORM_UV_TANGE_COL_SKIN& vertex = modelMesh.vertices[ i ];

		aiVector3D* position = &( pAiMesh->mVertices[ i ] );
		vertex.position =PVector4(position->x,position->y,position->z,1.0f);

		aiVector3D* tangent = ( pAiMesh->HasTangentsAndBitangents() ) ? &( pAiMesh->mTangents[ i ] ) : &zero3D;
		vertex.tangent =PVector3(tangent->x,tangent->y,tangent->z);
	}
}

void PAssimpLoader::_ParseMeshFaces(P_MODEL_DATA* pData,aiMesh* pAiMesh)
{
	P_MODEL_MESH& modelMesh = pData->meshes.back();

	std::vector<P_POS_NORM_UV_TANGE_COL_SKIN>& vertices = modelMesh.vertices;
	std::vector<uint32_t>& indices = modelMesh.indices;

	uint32_t polygonCount = pAiMesh->mNumVertices;

	aiVector3D zero3D(0.0f,0.0f,0.0f);

	for ( uint32_t i = 0; i < polygonCount; i++ )
	{
		P_POS_NORM_UV_TANGE_COL_SKIN& vertex = vertices[ i ];
		aiVector3D* normal = &( pAiMesh->mNormals[ i ] );

		vertex.normal =PVector3(normal->x,normal->y,normal->z);
		aiVector3D* uv = ( pAiMesh->HasTextureCoords(0) ) ? &( pAiMesh->mTextureCoords[ 0 ][ i ] ) : &zero3D;
		vertex.uv =PVector2(uv->x,uv->y);
	}

	indices.resize(static_cast< size_t >( pAiMesh->mNumFaces ) * 3);

	for ( uint32_t i = 0; i < pAiMesh->mNumFaces; ++i )
	{
		const aiFace& face = pAiMesh->mFaces[ i ];


		for ( uint32_t j = 0; j < face.mNumIndices; j++ )
		{
			indices[ static_cast< size_t >( i ) * 3 + j ] = face.mIndices[ j ];
		}
	}
}

void PAssimpLoader::_ParseMaterial(P_MODEL_DATA* pData,const std::string& directoryPath,aiMesh* pAiMesh,aiMaterial* pAiMaterial)
{
	P_MODEL_MESH& modelMesh = pData->meshes.back();

	aiColor4D zeroColor(1.0f,1.0f,1.0f,1.0f);

	for ( uint32_t i = 0; i < pAiMesh->mNumVertices; i++ )
	{
		P_POS_NORM_UV_TANGE_COL_SKIN& vertex = modelMesh.vertices[ i ];
		aiColor4D* color = ( pAiMesh->HasVertexColors(0) ) ? &( pAiMesh->mColors[ 0 ][ i ] ) : &zeroColor;

		vertex.color =PVector4(color->r,color->g,color->b,color->a);
	}

	aiColor3D ambient(0.0f,0.0f,0.0f);
	pAiMaterial->Get(AI_MATKEY_COLOR_AMBIENT,ambient);
	modelMesh.material.ambient =PVector3(ambient.r,ambient.g,ambient.b);

	//拡散反射光係数
	aiColor3D diffuse(0.0f,0.0f,0.0f);
	pAiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE,diffuse);
	modelMesh.material.diffuse =PVector3(diffuse.r,diffuse.g,diffuse.b);

	//スペキュラー係数
	aiColor3D specular(0.0f,0.0f,0.0f);
	pAiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE,specular);
	modelMesh.material.specular =PVector3(specular.r,specular.g,specular.b);

	aiString str;
	pAiMaterial->Get(AI_MATKEY_NAME,str);
	modelMesh.material.name = str.C_Str();

	// ディフューズマップ
	std::vector<std::string> diffuseMaps = _LoadMatrixialTextures(pAiMaterial,directoryPath,aiTextureType_DIFFUSE,"Diffuse");
	// 法線マップ
	std::vector<std::string> normalMaps = _LoadMatrixialTextures(pAiMaterial,directoryPath,aiTextureType_HEIGHT,"Normal");

	modelMesh.textures = diffuseMaps;
	modelMesh.texturesNormal = normalMaps;
}

void PAssimpLoader::_GetNodeNum(const aiNode* pAiNode,uint32_t& num)
{
	if ( pAiNode->mChildren )
	{
		for ( uint32_t i = 0; i < pAiNode->mNumChildren; i++ )
		{
			_GetNodeNum(pAiNode->mChildren[ i ],num);
		}
	}

	num++;
}

std::vector<std::string> PAssimpLoader::_GetFileNames(const std::string& folderPath)
{
	std::filesystem::directory_iterator lDirectoryIterator(folderPath),lIteratorEnd;
	std::error_code lErrorCode;
	std::vector<std::string> lFileNames;

	for ( ; lDirectoryIterator != lIteratorEnd && !lErrorCode; lDirectoryIterator.increment(lErrorCode) )
	{
		const std::filesystem::directory_entry lEntry = *lDirectoryIterator;

		lFileNames.push_back(lEntry.path().string());
	}

	return lFileNames;
}

void PAssimpLoader::_ParseSkin(P_MODEL_DATA* pData,aiMesh* pAiMesh)
{
	struct WeightSet
	{
		uint32_t index;
		float weight;
	};

	std::vector<std::list<WeightSet>>weightLists(pData->meshes.back().vertices.size());

	std::vector<P_POS_NORM_UV_TANGE_COL_SKIN>& vertices = pData->meshes.back().vertices;

	if ( pAiMesh->mNumBones == 0 )
	{
		return;
	}
	else
	{
		pData->canAnimation = true;
	}

	for ( uint32_t i = 0; i < pAiMesh->mNumBones; i++ )
	{
		aiBone* meshBone = pAiMesh->mBones[ i ];
		const char* boneName = meshBone->mName.C_Str();

		P_BONE bone;
		bone.name = boneName;
		bone.index = i;

		_ConvertMatrix(bone.offsetMatrix,meshBone->mOffsetMatrix.Transpose());

		pData->meshes.back().bones.push_back(bone);

		for ( size_t k = 0; k < meshBone->mNumWeights; k++ )
		{
			uint32_t vertIndex = meshBone->mWeights[ k ].mVertexId;
			float weight = meshBone->mWeights[ k ].mWeight;
			weightLists[ vertIndex ].push_back(WeightSet{ ( uint32_t ) i,weight });
		}
	}

	for ( size_t j = 0; j < vertices.size(); j++ )
	{
		std::list<WeightSet>& weightList = weightLists[ j ];

		size_t weightArrayIndex = 0;

		for ( const WeightSet& weightSet : weightList )
		{
			vertices[ j ].boneIndex[ weightArrayIndex ] = weightSet.index;
			vertices[ j ].boneWeight[ weightArrayIndex ] = weightSet.weight;

			if ( ++weightArrayIndex >= 4 )
			{
				break;
			}

		}
	}
}

void PAssimpLoader::_ParseNodeAnime(aiNodeAnim* pNodeAnim,P_MOTION_NODE& pNode)
{
	pNode.name = pNodeAnim->mNodeName.C_Str();

	pNode.scalingKeys.resize(pNodeAnim->mNumScalingKeys);
	for ( size_t i = 0; i < pNodeAnim->mNumScalingKeys; i++ )
	{
		pNode.scalingKeys[ i ].time = ( float ) pNodeAnim->mScalingKeys[ i ].mTime;
		_ConvertVector3(pNode.scalingKeys[ i ].value,pNodeAnim->mScalingKeys[ i ].mValue);
	}

	pNode.rotationKeys.resize(pNodeAnim->mNumRotationKeys);
	for ( size_t i = 0; i < pNodeAnim->mNumRotationKeys; i++ )
	{
		pNode.rotationKeys[ i ].time = ( float ) pNodeAnim->mRotationKeys[ i ].mTime;
		_ConvertQuaternion(pNode.rotationKeys[ i ].value,pNodeAnim->mRotationKeys[ i ].mValue);
	}

	pNode.positionKeys.resize(pNodeAnim->mNumPositionKeys);
	for ( size_t i = 0; i < pNodeAnim->mNumPositionKeys; i++ )
	{
		pNode.positionKeys[ i ].time = ( float ) pNodeAnim->mPositionKeys[ i ].mTime;
		_ConvertVector3(pNode.positionKeys[ i ].value,pNodeAnim->mPositionKeys[ i ].mValue);
	}
}

std::vector<std::string> PAssimpLoader::_LoadMatrixialTextures(aiMaterial* pAiMaterial,const std::string& directoryPath,aiTextureType type,std::string typeName)
{
	std::vector<std::string>textures;

	for ( size_t i = 0; i < pAiMaterial->GetTextureCount(type); i++ )
	{
		aiString str;
		pAiMaterial->GetTexture(type,static_cast< uint32_t >( i ),&str);

		std::string texture;
		std::string filename = _ExtractFileName(str.C_Str());

		filename = directoryPath + "\\" + filename;
		texture = filename;
		textures.push_back(texture);
	}

	return textures;

}

void PAssimpLoader::_ConvertMatrix(PMatrix4& dest,const aiMatrix4x4& src)
{
	dest.Set(0,0,src.a1);
	dest.Set(1,0,src.a2);
	dest.Set(2,0,src.a3);
	dest.Set(3,0,src.a4);

	dest.Set(0,1,src.b1);
	dest.Set(1,1,src.b2);
	dest.Set(2,1,src.b3);
	dest.Set(3,1,src.b4);

	dest.Set(0,2,src.c1);
	dest.Set(1,2,src.c2);
	dest.Set(2,2,src.c3);
	dest.Set(3,2,src.c4);

	dest.Set(0,3,src.d1);
	dest.Set(1,3,src.d2);
	dest.Set(2,3,src.d3);
	dest.Set(3,3,src.d4);

}

void PAssimpLoader::_ConvertVector3(PVector3& dest,const aiVector3D& src)
{
	dest.SetX(src.x);
	dest.SetY(src.y);
	dest.SetZ(src.z);
}

void PAssimpLoader::_ConvertQuaternion(PQuaternion& dest,const aiQuaternion& src)
{
	dest.SetX(src.x);
	dest.SetY(src.y);
	dest.SetZ(src.z);
	dest.SetW(src.w);
}

std::string PAssimpLoader::_ExtractFileName(const std::string& directoryPath)
{
	std::filesystem::path path = directoryPath;

	return path.filename().string();
}

PHONON_LOADER_NAMESPACE_END