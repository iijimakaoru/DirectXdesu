#include "CaptureModel.h"

CaptureModel::CaptureModel()
{
	pPipeline = PipelineManager::GetInstance()->GetPipeline("Fbx");
}

CaptureModel::~CaptureModel()
{
}

void CaptureModel::Update(ViewProjection* pViewProjection, Transform& transform)
{
	{
		transform.TransUpdate();
		m_transformBuffer.data.world = transform.GetMatWorld();
		m_transformBuffer.data.viewproj = pViewProjection->GetMatView() * pViewProjection->GetMatPro();
		m_transformBuffer.data.cameraPos =  pViewProjection->GetEye();
		_UpdateTransformBuffer();
	}

	{
		//TODO:ボーン更新
		_UpdateSkinBuffer();
	}
}

void CaptureModel::Draw()
{
	ID3D12GraphicsCommandList* pCmdList = KDirectXCommon::GetInstance()->GetCommandList();

	pPipeline->Setting();
	pPipeline->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto& mesh : m_meshes)
	{
		// 定数バッファビューをセット
		pCmdList->SetGraphicsRootConstantBufferView(0, m_transformBuffer.buff->GetGPUVirtualAddress());
		pCmdList->SetGraphicsRootConstantBufferView(2, mesh.skinBuffer.buff->GetGPUVirtualAddress());

		// 頂点バッファビューの設定
		pCmdList->IASetVertexBuffers(0, 1, &mesh.vertexBuffer.view);

		// インデックスバッファビューの設定
		pCmdList->IASetIndexBuffer(&mesh.indexBuffer.view);

		// デスクリプタヒープのセット
		ID3D12DescriptorHeap* ppHeaps[] = { TextureManager::GetInstance()->GetSrvHeap()};
		pCmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		// シェーダーリソースビューをセット
		pCmdList->SetGraphicsRootDescriptorTable(1, mesh.textureDatas[0].gpuHandle);

		// 描画
		pCmdList->DrawIndexedInstanced((UINT)mesh.indexBuffer.indices.size(), 1, 0, 0, 0);
	}

}

void CaptureModel::_SetModelData(const MCBM::M_MODEL_OUT& data)
{
	m_meshes.resize(data.meshs_.size());

	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		Mesh& mesh = m_meshes[i];
		const MCBM::M_MODEL_MESH& modelMesh = data.meshs_[i];

		{
			mesh.indexBuffer.indices.resize(modelMesh.indices.size());

			for each(uint16_t var in modelMesh.indices)
			{
				mesh.indexBuffer.indices[i] = var;
			}
		}

		{
			mesh.vertexBuffer.vertices.resize(modelMesh.vertices.size());

			for each(MCBM::M_POS_NORM_UV_TANGE_COL_SKIN var in modelMesh.vertices)
			{
				VertexPosNormalUVSkin& vertex = mesh.vertexBuffer.vertices[i];

				vertex.position = _ConvertVector4(var.position);
				vertex.normal = _ConvertVector3(var.normal);
				vertex.uv = _ConvertVector2(var.uv);
				vertex.boneIndex = var.boneIndex;
				vertex.boneWeight = var.boneWeight;

			}
		}

		{
			mesh.material.ambient = _ConvertVector3(modelMesh.material.ambient);
			mesh.material.diffuse = _ConvertVector3(modelMesh.material.diffuse);
			mesh.material.specular = _ConvertVector3(modelMesh.material.specular);
			mesh.material.emission = _ConvertVector3(modelMesh.material.emission);
			mesh.material.shininess = modelMesh.material.shininess;
			mesh.material.alpha = modelMesh.material.alpha;
		}

		{
			for (size_t i = 0; i < mesh.skinBuffer.m_bones.size(); i++)
			{
				mesh.skinBuffer.m_bones[i] = _ConvertMatrix4(modelMesh.bones[i].matrix);
			}
		}

		{
			mesh.textureDatas.resize(modelMesh.textures.size());

			for (size_t i = 0; i < modelMesh.textures.size(); i++)
			{
				mesh.textureDatas[i] = TextureManager::Load(modelMesh.textures[i]);
			}
		}
	}
}

void CaptureModel::_CreateVertexBuffer()
{
	ID3D12Device* pDevice = KDirectXCommon::GetInstance()->GetDevice();

	CD3DX12_HEAP_PROPERTIES heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		VertexBuffer& buffer = m_meshes[i].vertexBuffer;

		UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUVSkin) * buffer.vertices.size());

		CD3DX12_RESOURCE_DESC vB = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

		HRESULT result = pDevice->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &vB, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&buffer.buff));
		assert(SUCCEEDED(result));

		VertexPosNormalUVSkin* vertMap = nullptr;
		result = buffer.buff->Map(0, nullptr, (void**)&vertMap);
		std::copy(buffer.vertices.begin(), buffer.vertices.end(), vertMap);
		buffer.buff->Unmap(0, nullptr);

		buffer.view.BufferLocation = buffer.buff->GetGPUVirtualAddress();
		buffer.view.SizeInBytes = sizeVB;
		buffer.view.StrideInBytes = sizeof(buffer.vertices[0]);
	}
}

void CaptureModel::_CreateIndexBuffer()
{
	ID3D12Device* pDevice = KDirectXCommon::GetInstance()->GetDevice();

	CD3DX12_HEAP_PROPERTIES heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		IndexBuffer& buffer = m_meshes[i].indexBuffer;

		UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * buffer.indices.size());

		CD3DX12_RESOURCE_DESC iB = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);

		HRESULT result = pDevice->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &iB, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&buffer.buff));

		uint16_t* pIndexMap = nullptr;
		result = buffer.buff->Map(0, nullptr, (void**)&pIndexMap);

		std::copy(buffer.indices.begin(), buffer.indices.end(), pIndexMap);
		buffer.buff->Unmap(0, nullptr);

		buffer.view.BufferLocation = buffer.buff->GetGPUVirtualAddress();
		buffer.view.Format = DXGI_FORMAT_R16_UINT;
		buffer.view.SizeInBytes = sizeIB;
	}
}

void CaptureModel::_CreateSkinBuffer()
{
	ID3D12Device* pDevice = KDirectXCommon::GetInstance()->GetDevice();

	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		SkinBuffer& buffer = m_meshes[i].skinBuffer;

		CD3DX12_RESOURCE_DESC buffSkin = CD3DX12_RESOURCE_DESC::Buffer((sizeof(buffer.m_bones) + 0xff) & ~0xff);

		HRESULT result = pDevice->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &buffSkin, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&buffer.buff));
		assert(SUCCEEDED(result));

		result = buffer.buff->Map(0, nullptr, (void**)&buffer.pMappingData);
		assert(SUCCEEDED(result));

	}
}

void CaptureModel::_CreateTransformBuffer()
{
	ID3D12Device* pDevice = KDirectXCommon::GetInstance()->GetDevice();

	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	CD3DX12_RESOURCE_DESC buff = CD3DX12_RESOURCE_DESC::Buffer((sizeof(TransformBufferData) + 0xff) & ~0xff);

	HRESULT result = pDevice->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &buff, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_transformBuffer.buff));
	assert(SUCCEEDED(result));

	result = m_transformBuffer.buff->Map(0, nullptr, (void**)&m_transformBuffer.pMappingData);
	assert(SUCCEEDED(result));

}

void CaptureModel::_UpdateSkinBuffer()
{
	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		SkinBuffer& buffer = m_meshes[i].skinBuffer;

		memcpy(buffer.pMappingData, buffer.m_bones.data(), MAX_BONE * sizeof(KMyMath::Matrix4));
	}
}

void CaptureModel::_UpdateTransformBuffer()
{
	memcpy(m_transformBuffer.pMappingData, &m_transformBuffer.data, sizeof(TransformBufferData));
}

KMyMath::Vector2 CaptureModel::_ConvertVector2(const MCBM::MVector2& vec)
{
	return KMyMath::Vector2(vec.GetX(), vec.GetY());
}

KMyMath::Vector3 CaptureModel::_ConvertVector3(const MCBM::MVector3& vec)
{
	return KMyMath::Vector3(vec.x, vec.y, vec.z);
}
KMyMath::Vector4 CaptureModel::_ConvertVector4(const MCBM::MVector4& vec)
{
	return KMyMath::Vector4(vec.GetX(), vec.GetY(), vec.GetZ(), vec.GetW());
}

KMyMath::Matrix4 CaptureModel::_ConvertMatrix4(const MCBM::Matrix& mat)
{
	return KMyMath::Matrix4(mat._11_, mat._12_, mat._13_, mat._14_, mat._21_, mat._22_, mat._23_, mat._24_, mat._31_, mat._32_, mat._33_, mat._34_, mat._41_, mat._42_, mat._43_, mat._44_);
}
