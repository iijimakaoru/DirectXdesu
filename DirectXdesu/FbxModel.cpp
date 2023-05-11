#include "FbxModel.h"

void FbxModel::CreateModel()
{
	vertexs.reset(new KVertex(KDirectXCommon::GetInstance()->GetDev(), vertices, indices));
}

void FbxModel::Draw()
{
}
