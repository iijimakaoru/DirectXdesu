#pragma once
#include<Animation/ModelOut.h>
#include "KDirectXCommon.h"
#include "TextureManager.h"
#include<Transform.h>
#include<ViewProjection.h>
#include<PipelineManager.h>
#include <KGPlin.h>

class CaptureModel
{
public:
	CaptureModel();
	~CaptureModel();
public:

	static const size_t MAX_BONE_INDICES = 4;
	static const size_t MAX_BONE = 128;

	struct Material
	{
		KMyMath::Vector3 ambient = { 0.06f, 0.06f, 0.06f };
		KMyMath::Vector3 diffuse = { 0.0f,0.0f,0.0f };
		KMyMath::Vector3 specular = { 0.0f,0.0f,0.0f };
		KMyMath::Vector3 emission = { 0.0f,0.0f,0.0f };
		float shininess = 1.0f;
		float alpha = 1.0f;
	};

	struct VertexPosNormalUVSkin
	{
		KMyMath::Vector4 position = { 0.0f,0.f,0.0f,1.0f };
		KMyMath::Vector3 normal;
		KMyMath::Vector2 uv;

		std::array<uint32_t, MAX_BONE_INDICES> boneIndex;
		std::array<float, MAX_BONE_INDICES> boneWeight;
	};

	struct VertexBuffer
	{
		Microsoft::WRL::ComPtr<ID3D12Resource>buff;
		D3D12_VERTEX_BUFFER_VIEW view;
		std::vector<VertexPosNormalUVSkin>vertices;

	};

	struct IndexBuffer
	{
		Microsoft::WRL::ComPtr<ID3D12Resource>buff;
		D3D12_INDEX_BUFFER_VIEW view;
		std::vector<uint16_t> indices;
	};
	
	struct TransformBufferData
	{
		KMyMath::Matrix4 viewproj; // ビュープロジェクション
		KMyMath::Matrix4 world;	   // ワールド行列
		KMyMath::Vector3 cameraPos;// カメラ座標
	};

	struct TransformBuffer
	{
		Microsoft::WRL::ComPtr<ID3D12Resource>buff;
		TransformBufferData data;
		TransformBufferData* pMappingData;
	};

	struct SkinBuffer
	{
		Microsoft::WRL::ComPtr<ID3D12Resource>buff;
		std::array<KMyMath::Matrix4, MAX_BONE>m_bones;
		KMyMath::Matrix4* pMappingData;
	};

	struct Mesh
	{
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;
		std::vector<TextureData> textureDatas;
		Material material;
		SkinBuffer skinBuffer;
	};

public:

	void Update(ViewProjection* pViewProjection, Transform& transform);

	void Draw();

private:

	void _SetModelData(const MCBM::M_MODEL_OUT& data);

	void _CreateVertexBuffer();
	void _CreateIndexBuffer();
	void _CreateSkinBuffer();
	void _CreateTransformBuffer();

	void _UpdateSkinBuffer();
	void _UpdateTransformBuffer();

	KMyMath::Vector2 _ConvertVector2(const MCBM::MVector2& vec);
	KMyMath::Vector3 _ConvertVector3(const MCBM::MVector3& vec);
	KMyMath::Vector4 _ConvertVector4(const MCBM::MVector4& vec);
	KMyMath::Matrix4 _ConvertMatrix4(const MCBM::Matrix& mat);

private:

	KGPlin* pPipeline = nullptr;
	std::vector<Mesh> m_meshes;
	TransformBuffer m_transformBuffer;
};

