#pragma once
#include "KGPlin.h"

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

	KGPlin* GetObjPipeline();
	KGPlin* GetSpritePipeline();
	KGPlin* GetFbxPipeline();

private:
	// パイプライン
	std::unique_ptr<KGPlin> objPipeline;
	std::unique_ptr<KGPlin> spritePipeline;
	std::unique_ptr<KGPlin> fbxPipeline;

	// シェーダー
	KShader objShader;
	KShader spriteShader;
	KShader fbxShader;
};
