#include "PipelineManager.h"

PipelineManager* PipelineManager::GetInstance() {
	static PipelineManager instance;

	return &instance;
}

void PipelineManager::Init() {
	// Obj
	shaders_["Obj"].Init(
		L"Resources/Shader/LightObj/lightOBJVS.hlsl",
		L"Resources/Shader/LightObj/lightOBJPS.hlsl");
	pipelines_["Obj"].reset(KGPlin::Create(shaders_["Obj"], "Obj"));

	// Sprite
	shaders_["Sprite"].Init(
		L"Resources/Shader/Sprite/SpriteVS.hlsl",
		L"Resources/Shader/Sprite/SpritePS.hlsl");
	pipelines_["Sprite"].reset(KGPlin::Create(shaders_["Sprite"], "Sprite"));

	// fbx
	shaders_["Fbx"].Init(
		L"Resources/Shader/Fbx/FbxVS.hlsl",
		L"Resources/Shader/Fbx/FbxPS.hlsl");
	pipelines_["Fbx"].reset(KGPlin::Create(shaders_["Fbx"], "Fbx"));

	// post
	shaders_["PostEffect"].Init(
		L"Resources/Shader/PostEffect/PostEffectVS.hlsl", 
		L"Resources/Shader/PostEffect/PostEffectPS.hlsl");
	pipelines_["PostEffect"].reset(KGPlin::Create(shaders_["PostEffect"], "PostEffect"));

	// Vignette
	shaders_["Vignette"].Init(
	    L"Resources/Shader/Vignette/VignetteVS.hlsl",
		L"Resources/Shader/Vignette/VignettePS.hlsl");
	pipelines_["Vignette"].reset(KGPlin::Create(shaders_["Vignette"], "Vignette"));

	// MultiTexture
	shaders_["MultiTexture"].Init(
	    L"Resources/Shader/MultiTexture/MultiTextureVS.hlsl",
		L"Resources/Shader/MultiTexture/MultiTexturePS.hlsl");
	pipelines_["MultiTexture"].reset(KGPlin::Create(shaders_["MultiTexture"], "MultiTexture"));
}

KGPlin* PipelineManager::GetPipeline(std::string pipelineName) { return pipelines_[pipelineName].get(); }
