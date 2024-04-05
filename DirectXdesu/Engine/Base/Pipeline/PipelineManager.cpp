#include "PipelineManager.h"

PipelineManager* PipelineManager::GetInstance() {
	static PipelineManager instance;

	return &instance;
}

void PipelineManager::Init() {
	// Obj
	shaders_["Obj"].Init(L"Resources/Shader/lightOBJVS.hlsl", L"Resources/Shader/lightOBJPS.hlsl");
	pipelines_["Obj"].reset(KGPlin::Create(shaders_["Obj"], "Obj"));

	// Sprite
	shaders_["Sprite"].Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	pipelines_["Sprite"].reset(KGPlin::Create(shaders_["Sprite"], "Sprite"));

	// fbx
	shaders_["Fbx"].Init(L"Resources/Shader/FbxVS.hlsl", L"Resources/Shader/FbxPS.hlsl");
	pipelines_["Fbx"].reset(KGPlin::Create(shaders_["Fbx"], "Fbx"));

	// post
	shaders_["PostEffect"].Init(
	    L"Resources/Shader/PostEffectVS.hlsl", L"Resources/Shader/PostEffectPS.hlsl");
	pipelines_["PostEffect"].reset(KGPlin::Create(shaders_["PostEffect"], "PostEffect"));

	// Vignette
	shaders_["Vignette"].Init(
	    L"Resources/Shader/VignetteVS.hlsl", L"Resources/Shader/VignettePS.hlsl");
	pipelines_["Vignette"].reset(KGPlin::Create(shaders_["Vignette"], "Vignette"));
}

KGPlin* PipelineManager::GetPipeline(std::string pipelineName) { return pipelines_[pipelineName].get(); }
