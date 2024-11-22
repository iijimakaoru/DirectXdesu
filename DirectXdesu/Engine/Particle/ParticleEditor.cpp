#include "ParticleEditor.h"
#include <imgui.h>

void ParticleEditor::Init()
{
}

void ParticleEditor::Update()
{
	ImGui::Begin("ParticleEditor");
	ImGui::SetWindowPos({ 0, 0 });
	ImGui::SetWindowSize({ 400, 400 });
	ImGui::SliderInt("MaxParticle", &particleParamaters_.maxParticle, 0, 100000);
	ImGui::SliderInt("GridSize", &particleParamaters_.gridSize, 0, 200);
	ImGui::SliderFloat("EmissionRate", &particleParamaters_.emissionRate, 0, 100, "%.1f");
	ImGui::SliderFloat("LifeTime", &particleParamaters_.lifeTime, 0, 200, "%.1f");
	ImGui::SliderFloat3("Velocity", &particleParamaters_.velocity.x, 0, 200, "%.1f");
	ImGui::SliderFloat3("Acceleration", &particleParamaters_.acceleration.x, 0, 200, "%.1f");
	ImGui::SliderFloat4("StartColor", &particleParamaters_.startColor.x, 0, 1, "%.1f");
	ImGui::SliderFloat4("EndColor", &particleParamaters_.endColor.x, 0, 1, "%.1f");
	ImGui::End();
}

void ParticleEditor::Draw()
{
}
