#include "Object3D.h"

Object3D::Object3D(HRESULT result, Object3d* object, ID3D12Device* dev) {

}

void Object3D::Initialize(HRESULT result, Object3d* object, ID3D12Device* dev) {

}

void Object3D::Update(Object3d* object, XMMATRIX& matView, XMMATRIX& matProjection) {

}

void Object3D::Draw(Object3d* object, ID3D12GraphicsCommandList* cmdList, D3D12_VERTEX_BUFFER_VIEW& vbview,
	D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices) {

}
