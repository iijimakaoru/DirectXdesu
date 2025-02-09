#include <Math/MVector3.h>
#include <cmath>


using namespace MCBM;
MCBM::MVector3::MVector3(const MVector3& start, const MVector3& end)
{
	MVector3 temp = end - start;
	x = temp.x;
	y = temp.y;
	z = temp.z;
}

MCBM::MVector3::MVector3()
{
	x = 0;
	y = 0;
	z = 0;
}

MCBM::MVector3::MVector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
MVector3 MCBM::MVector3::Vector3Substruct(const MVector3& start, const MVector3& end)
{
	MVector3 temp(start, end);
	return temp;
}

float MCBM::MVector3::V3Len() const
{
	return sqrtf(((x * x) + (y * y) + (z * z)));
}

void MCBM::MVector3::V3Norm()
{
	float VecLen = V3Len();
	if (VecLen == 0.0f)
	{
		//assert(0);
		return;
	}

	x = x / VecLen;
	y = y / VecLen;
	z = z / VecLen;

}

MVector3 MCBM::MVector3::GetV3Norm()
{
	MVector3 ans = *this;
	ans.V3Norm();
	return ans;
}

float MCBM::MVector3::GetV3Dot(const MVector3& vector)
{
	return (x * vector.x) + (y * vector.y) + (z * vector.z);
}

MVector3 MCBM::MVector3::GetV3Cross(const MVector3& vector)
{
	MVector3 temp;
	temp.x = y * vector.z - z * vector.y;
	temp.y = z * vector.x - x * vector.z;
	temp.z = x * vector.y - y * vector.x;
	return temp;
}

MVector3 MCBM::MVector3::GetV3Cross(const MVector3& avector, const MVector3& bvector)
{
	MVector3 temp;
	temp.x = avector.y * bvector.z - avector.z * bvector.y;
	temp.y = avector.z * bvector.x - avector.x * bvector.z;
	temp.z = avector.x * bvector.y - avector.y * bvector.x;
	return temp;
}

MVector3 MCBM::MVector3::GetUpVec(MVector3 RightVec, MVector3 frontVec)
{
	MVector3 ans;
	RightVec.V3Norm();
	frontVec.V3Norm();
	ans = ans.GetV3Cross(frontVec, RightVec);
	ans.V3Norm();

	return ans;
}


MVector3 MCBM::MVector3::GetRightVec(MVector3 frontVec, MVector3 UpVec)
{
	MVector3 ans;
	frontVec.V3Norm();
	UpVec.V3Norm();
	ans = ans.GetV3Cross(UpVec, frontVec);
	ans.V3Norm();

	return ans;
}

MVector3 MCBM::MVector3::GetV3Normal(MVector3 v0, MVector3 v1, MVector3 v2)
{
	MVector3 ans;
	MVector3 vv1, vv2;
	MVector3 v0n, v1n, v2n;
	v0n = v0; //v0n.V3Norm();
	v1n = v1; //v1n.V3Norm();
	v2n = v2; //v2n.V3Norm();
	vv1 = vv1.Vector3Substruct(v0n, v1n);
	vv2 = vv2.Vector3Substruct(v1n, v2n);
	vv1.V3Norm();
	vv2.V3Norm();
	ans = vv1.GetV3Cross(vv2);
	ans.V3Norm();
	return ans;
}

MVector3 MCBM::MVector3::Qlamp(const MVector3& vec, const MVector3& min, const MVector3& max)
{
	MVector3 ret = vec;
	if (vec.x < min.x)
	{
		ret.x = min.x;
	}
	else if (vec.x > max.x)
	{
		ret.x = max.x;
	}

	if (vec.y < min.y)
	{
		ret.y = min.y;
	}
	else if (vec.y > max.y)
	{
		ret.y = max.y;
	}

	if (vec.z < min.z)
	{
		ret.z = min.z;
	}
	else if (vec.z > max.z)
	{
		ret.z = max.z;
	}

	return ret;
}



float MCBM::MVector3::GetInnerProduct(MVector3 vector)
{
	MVector3 temp;
	temp = *this;
	temp.V3Norm();
	vector.V3Norm();
	return acos(temp.GetV3Dot(vector));
}

MVector3& MCBM::MVector3::operator+=(const MVector3& Vec)
{
	x += Vec.x;
	y += Vec.y;
	z += Vec.z;
	return *this;
}

MVector3& MCBM::MVector3::operator-=(const MVector3& Vec)
{
	x -= Vec.x;
	y -= Vec.y;
	z -= Vec.z;
	return *this;
}

MVector3& MCBM::MVector3::operator*=(float k)
{
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

MVector3 MCBM::MVector3::Clamp(const MVector3& vec1, const MVector3& min, const MVector3& max, bool* isClamped)
{
	MVector3 ret;
	bool isClamp = false;
	if (vec1.x < min.x)
	{
		ret.x = min.x;
		isClamp = true;
	}
	else if (vec1.x > max.x)
	{
		ret.x = max.x;
		isClamp = true;
	}


	if (vec1.y < min.y)
	{
		ret.y = min.y;
		isClamp = true;
	}
	else if (vec1.y > max.y)
	{
		ret.y = max.y;
		isClamp = true;
	}

	if (vec1.z < min.z)
	{
		ret.z = min.z;
		isClamp = true;
	}
	else if (vec1.z > max.z)
	{
		ret.z = max.z;
		isClamp = true;
	}
	if (isClamped != nullptr)
	{
		*isClamped = isClamp;
	}

	return ret;
}

MVector3 MCBM::operator+(const MVector3& vecA, const MVector3& vecB)
{
	MVector3 temp;
	temp = vecA;
	temp += vecB;
	return temp;
}

bool MCBM::operator!=(const MVector3& vecA, const MVector3& vecB)
{
	return (vecA.x != vecB.x || vecA.y != vecB.x || vecA.z != vecB.x);
}

MVector3 MCBM::operator-(const MVector3& vecA, const MVector3& vecB)
{
	MVector3 temp;
	temp = vecA;
	temp -= vecB;
	return temp;
}

MVector3 MCBM::operator*(const MVector3& vecA, float k)
{
	MVector3 temp;
	temp = vecA;
	temp *= k;
	return temp;
}

MVector3 MCBM::operator*(float k, const MVector3& vecA)
{
	MVector3 temp;
	temp = vecA;
	temp *= k;
	return temp;
}

MVector3 MCBM::operator/(const MVector3& vecA, float k)
{
	MVector3 temp;
	temp.x = vecA.x / k;
	temp.y = vecA.y / k;
	temp.z = vecA.z / k;
	return temp;
}

MVector3 MCBM::operator-(const MVector3& vecA)
{
	MVector3 temp;
	temp.x = vecA.x * -1;
	temp.y = vecA.y * -1;
	temp.z = vecA.z * -1;
	return temp;
}



MVector3 MVector3::adjustVectorLength(const MVector3& base, const MVector3& target) {
	// ベースベクトルの長さの2乗を計算
	double baseLengthSquared = base.x * base.x + base.y * base.y;

	// targetのxy成分の長さの2乗を計算（zは0）
	double targetXYLengthSquared = target.x * target.x + target.y * target.y;

	// 必要なz成分を計算（sqrt計算を1回に削減）
	double requiredZ = std::sqrt(baseLengthSquared - targetXYLengthSquared);

	// 新しいベクトルを返す
	return MVector3(target.x, target.y, requiredZ);
}