#ifndef	__RENDER_P_MODELDATA_H__
#define	__RENDER_P_MODELDATA_H__

#include<P_Defined.h>
#include<PVector2.h>
#include<PVector3.h>
#include<PVector4.h>
#include<PQuaternion.h>
#include<PMatrix4.h>

PHONON_SUPPRESS_WARNINGS_BEGIN

#include<string>
#include<vector>
#include<array>

PHONON_SUPPRESS_WARNINGS_END

PHONON_LOADER_NAMESPACE_BEGIN

struct P_POS_NORM_UV_TANGE_COL_SKIN
{
	PVector4 position = { 0.0f,0.f,0.0f,1.0f };
	PVector3 normal;
	PVector2 uv;
	PVector3 tangent;
	PVector4 color;

	std::array<uint32_t,4> boneIndex;
	std::array<float,4> boneWeight;
};

struct P_BONE
{
	std::string name;
	PMatrix4 matrix;
	PMatrix4 offsetMatrix;
	uint32_t index = 0;

private:
	uint32_t pad = 0;

};

struct P_NODE
{
	std::string name;
	PMatrix4 transform;
	PMatrix4 globalTransform;
	PVector3 position;
	PVector4 rotation;
	PVector3 scale;
	std::string parentNodeName;
	std::vector<std::string>childrenNodeNames;
};

struct P_MODEL_MATERIAL
{
	std::string name;
	std::string textureFileName;
	PVector3 ambient = { 0.06f, 0.06f, 0.06f };
	PVector3 diffuse = { 0.0f,0.0f,0.0f };
	PVector3 specular = { 0.0f,0.0f,0.0f };
	PVector3 emission = { 0.0f,0.0f,0.0f };
	float shininess = 1.0f;
	float alpha = 1.0f;
};

struct P_VECTOR_KEY
{
	float time = 0.0f;
	PVector3 value;

private:

	uint32_t pad = 0;
};

struct P_QUATERNION_KEY
{
	float time = 0.0f;
	PQuaternion value;
};

struct P_MOTION_NODE
{
	std::string name;
	std::vector<P_VECTOR_KEY>scalingKeys;
	std::vector<P_QUATERNION_KEY>rotationKeys;
	std::vector<P_VECTOR_KEY>positionKeys;
};

struct P_ANIMATION_DATA
{
	std::string name;
	float ticksPerSecond = 0.0f;
	float duration = 0.0f;
	std::vector<P_MOTION_NODE> channels;
};

struct P_MODEL_MESH
{
	std::string name;
	std::string nodeName;
	std::vector<P_POS_NORM_UV_TANGE_COL_SKIN> vertices;
	std::vector<uint32_t> indices;
	std::vector<P_BONE> bones;
	std::vector <std::string> textures;
	std::vector <std::string> texturesNormal;
	P_NODE* pNode = nullptr;
	P_MODEL_MATERIAL material;
};

struct P_MODEL_DATA
{
	std::string name;
	std::vector<P_MODEL_MESH> meshes;
	std::vector<P_NODE> nodes;
	std::vector<P_ANIMATION_DATA> animations;
	PMatrix4 globalInverseTransform;
	bool canAnimation = false;
private:
	uint8_t pad[ 7 ] = {};
};

PHONON_LOADER_NAMESPACE_END

#endif	//__RENDER_P_MODELDATA_H__
