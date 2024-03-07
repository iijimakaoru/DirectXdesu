#include "PipelineManager.h"

PipelineManager* PipelineManager::GetInstance() {
	static PipelineManager instance;

	return &instance;
}

void PipelineManager::Init() {
	// Obj
	objShader.Init(L"Resources/Shader/lightOBJVS.hlsl", L"Resources/Shader/lightOBJPS.hlsl");
	objPipeline.reset(KGPlin::Create(objShader, "Obj"));

	// Sprite
	spriteShader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	spritePipeline.reset(KGPlin::Create(spriteShader, "Sprite"));

	// fbx
	fbxShader.Init(L"Resources/Shader/FbxVS.hlsl", L"Resources/Shader/FbxPS.hlsl");
	fbxPipeline.reset(KGPlin::Create(fbxShader, "Fbx"));

	// post
	postShader.Init(L"Resources/Shader/PostEffectVS.hlsl", L"Resources/Shader/PostEffectPS.hlsl");
	postPipeline.reset(KGPlin::Create(postShader, "PostEffect"));
}

KGPlin* PipelineManager::GetObjPipeline() { return objPipeline.get(); }

KGPlin* PipelineManager::GetSpritePipeline() { return spritePipeline.get(); }

KGPlin* PipelineManager::GetFbxPipeline() { return fbxPipeline.get(); }

KGPlin* PipelineManager::GetPostPipeline() { return postPipeline.get(); }
