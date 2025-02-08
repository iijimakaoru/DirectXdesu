#ifndef	__RENDER_P_ASSIMPLOADER_H__
#define	__RENDER_P_ASSIMPLOADER_H__

#include<P_MODEL_DATA.h>

PHONON_SUPPRESS_WARNINGS_BEGIN

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<assimp/cimport.h>

PHONON_SUPPRESS_WARNINGS_END

PHONON_LOADER_NAMESPACE_BEGIN

class PAssimpLoader final
{
public:

	static void Load(P_MODEL_DATA* pData,const std::string& filePath);

private:

	PAssimpLoader();
	~PAssimpLoader();
	PAssimpLoader(const PAssimpLoader&) = delete;
	PAssimpLoader& operator=(const PAssimpLoader&) = delete;

	static PAssimpLoader* GetInstance();

	void _Load(P_MODEL_DATA* pData,const std::string& filePath);

	void _ParseNodeRecursive(P_MODEL_DATA* pData,const std::string& directoryPath,const aiScene* pScene,aiNode* pAiNode,P_NODE* pParent = nullptr);
	void _ParseMesh(P_MODEL_DATA* pData,const std::string& directoryPath,const aiScene* pScene,aiMesh* pAiMesh);
	void _ParseMeshVertices(P_MODEL_DATA* pData,aiMesh* pAiMesh);
	void _ParseMeshFaces(P_MODEL_DATA* pData,aiMesh* pAiMesh);
	void _ParseMaterial(P_MODEL_DATA* pData,const std::string& directoryPath,aiMesh* pAiMesh,aiMaterial* pAiMaterial);
	void _ParseSkin(P_MODEL_DATA* pData,aiMesh* pAiMesh);

	void _ParseNodeAnime(aiNodeAnim* pNodeAnim,P_MOTION_NODE& pNode);

	std::vector<std::string> _LoadMatrixialTextures(aiMaterial* pAiMaterial,const std::string& directoryPath,aiTextureType type,std::string typeName);

	void  _ConvertMatrix(PMatrix4& dest,const aiMatrix4x4& src);
	void  _ConvertVector3(PVector3& dest,const aiVector3D& src);
	void  _ConvertQuaternion(PQuaternion& dest,const aiQuaternion& src);

	std::string _ExtractFileName(const std::string& path);
	void _GetNodeNum(const aiNode* pAiNode,uint32_t& num);

	std::vector<std::string> _GetFileNames(const std::string& folderPath);
private:

	const uint32_t ASSIMP_LOAD_FLAG_DEFAULT =
		aiProcess_FlipUVs |
		aiProcess_FlipWindingOrder |
		aiProcess_Triangulate | //三角面化
		aiProcess_CalcTangentSpace | //接線ベクトル生成
		aiProcess_GenUVCoords | //非マッピングを適切なUV座標に変換
		aiProcess_RemoveRedundantMaterials | //冗長なマテリアルを削除
		aiProcess_OptimizeMeshes | //メッシュ数を最適化
		aiProcess_MakeLeftHanded | //ノードを左手座標系に
		aiProcess_JoinIdenticalVertices |//インデックスを生成
		aiProcess_LimitBoneWeights;//各頂点が影響を受けるボーンを4に制限
};

PHONON_LOADER_NAMESPACE_END

#endif	//__RENDER_P_ASSIMPLOADER_H__