#pragma once
#include <string>
#include <Math/MVector4.h>
#include <Math/MVector3.h>
#include <Math/MVector2.h>
#include <vector>
#include <Animation/AnimationMesh.h>

namespace MCBM
{
	struct OUT_BONE
	{
		std::string name;
		std::vector<std::string> childrenNames;
		std::string parentName;

	};

	struct M_MODEL_OUT
	{
		std::string name;
		std::vector<M_MODEL_MESH> meshs_;
		std::vector<OUT_BONE> bones_;
		
	};
}