#pragma once
#include <Math/MVector2.h>
#include <Math/MVector3.h>
#include <Math/MVector4.h>
#include <Math/Matrix.h>
#include <vector>
#include <string>

namespace MCBM
{
	struct M_POS_NORM_UV_TANGE_COL_SKIN
	{
		MCBM::MVector4 position = { 0.0f,0.f,0.0f,1.0f };
		MCBM::MVector3 normal;
		MCBM::MVector2 uv;
		MCBM::MVector3 tangent;
		MCBM::MVector4 color;

		std::array<uint32_t, 4> boneIndex;
		std::array<float, 4> boneWeight;
	};

	struct M_MODEL_MATERIAL
	{
		std::string textureFileName;
		MCBM::MVector3 ambient = { 0.06f, 0.06f, 0.06f };
		MCBM::MVector3 diffuse = { 0.0f,0.0f,0.0f };
		MCBM::MVector3 specular = { 0.0f,0.0f,0.0f };
		MCBM::MVector3 emission = { 0.0f,0.0f,0.0f };
		float shininess = 1.0f;
		float alpha = 1.0f;
	};

	struct M_BONE
	{
		std::string name;
		Matrix matrix;
		Matrix offsetMatrix;
		uint32_t index = 0;

	private:
		uint32_t pad = 0;

	};

	struct M_MODEL_MESH
	{
		std::vector<M_POS_NORM_UV_TANGE_COL_SKIN> vertices;
		std::vector<uint32_t> indices;
		std::vector<M_BONE> bones;
		std::vector <std::string> textures;
		std::vector <std::string> texturesNormal;
		M_MODEL_MATERIAL material;
	};
}