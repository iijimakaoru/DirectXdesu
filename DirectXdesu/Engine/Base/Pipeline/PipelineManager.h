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

	KGPlin* GetPipeline(std::string pipelineName);

private:
	// パイプライン
	std::map<std::string, std::unique_ptr<KGPlin>> pipelines_;

	// シェーダー
	std::map<std::string, KShader> shaders_;
};
