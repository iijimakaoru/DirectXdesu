#pragma once
#include "KGPlin.h"
#include <map>

class PipelineManager {
public:
	static PipelineManager* GetInstance();

private:
	PipelineManager() = default;
	~PipelineManager() = default;
	PipelineManager(const PipelineManager&) = delete;
	const PipelineManager& operator=(const PipelineManager&) = delete;

public:
	void Init();

	KGPlin* GetSpritePipeline();
	KGPlin* GetFbxPipeline();
	KGPlin* GetPostPipeline();

	KGPlin* GetPipeline(std::string pipelineName);

private:
	// パイプライン
	std::unique_ptr<KGPlin> objPipeline;
	std::unique_ptr<KGPlin> spritePipeline;
	std::unique_ptr<KGPlin> fbxPipeline;
	std::unique_ptr<KGPlin> postPipeline;

	std::map<std::string, std::unique_ptr<KGPlin>> pipelines_;

	// シェーダー
	KShader objShader;
	KShader spriteShader;
	KShader fbxShader;
	KShader postShader;

	std::map<std::string, KShader> shaders_;
};
