#include "Collision.h"

using namespace DirectX;

bool Collision::CheckSphere2Plane(
    const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter) {
	// 座標系の原点から球の中心座標への距離
	DirectX::XMVECTOR distV = DirectX::XMVector3Dot(sphere.center, plane.normal);
	// 平面の原点距離を減算することで、平面と球の中心との距離が出る
	float dist = distV.m128_f32[0] - plane.distance;
	// 距離の絶対値が半径より大きければ当たってない
	if (fabsf(dist) > sphere.radius) {
		return false;
	}
	// 疑似交点を計算
	if (inter) {
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}

void Collision::ClosestPtPoint2Triangle(
    const DirectX::XMVECTOR& point, const ColTriangle& triangle, DirectX::XMVECTOR* closest) {
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
	DirectX::XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	DirectX::XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	DirectX::XMVECTOR p0_pt = point - triangle.p0;

	DirectX::XMVECTOR d1 = DirectX::XMVector3Dot(p0_p1, p0_pt);
	DirectX::XMVECTOR d2 = DirectX::XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f) {
		// p0が最近傍
		*closest = triangle.p0;
		return;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	DirectX::XMVECTOR p1_pt = point - triangle.p1;

	DirectX::XMVECTOR d3 = DirectX::XMVector3Dot(p0_p1, p1_pt);
	DirectX::XMVECTOR d4 = DirectX::XMVector3Dot(p0_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0]) {
		// p1が最近傍
		*closest = triangle.p1;
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f) {
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	DirectX::XMVECTOR p2_pt = point - triangle.p2;

	DirectX::XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	DirectX::XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);
	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0]) {
		*closest = triangle.p2;
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f) {
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
	if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f &&
	    (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f) {
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) /
		          ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Triangle(
    const Sphere& sphere, const ColTriangle& triangle, DirectX::XMVECTOR* inter) {
	DirectX::XMVECTOR p;
	// 球の中心に対する最近接点である三角形上にある点pを見つける
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	// 点pと球の中心の差分ベクトル
	DirectX::XMVECTOR v = p - sphere.center;
	// 距離の二乗を求める
	// (同じベクトル同士の内積は三平方の定理のルート内部の式と一致する)
	v = DirectX::XMVector3Dot(v, v);
	// 球と三角形の距離が半径以下なら当たってない
	if (v.m128_f32[0] > sphere.radius * sphere.radius) {
		return false;
	}
	// 疑似交点を計算
	if (inter) {
		*inter = p;
	}

	return true;
}

bool Collision::CheckRay2Plane(
    const Ray& ray, const Plane& plane, float* distance, DirectX::XMVECTOR* inter) {
	// 誤差吸収用の微小な値
	const float epsilon = 1.0e-5f;
	// 面法線とレイの方向ベクトルの内積
	float d1 = DirectX::XMVector3Dot(plane.normal, ray.dir).m128_f32[0];
	// 裏面には当たらない
	if (d1 > -epsilon) {
		return false;
	}
	// 始点と原点の距離(平面の法線方向)
	// 面法線とレイの始点座標(位置ベクトル)の内積
	float d2 = DirectX::XMVector3Dot(plane.normal, ray.start).m128_f32[0];
	// 始点と面積の距離(面積の法線方向)
	float dist = d2 - plane.distance;
	// 始点の平面の距離(レイ方向)
	float t = dist / -d1;
	// 交点が始点より後ろにあるので当たらない
	if (t < 0) {
		return false;
	}
	// 距離を書き込む
	if (distance) {
		*distance = t;
	}
	// 交点を計算
	if (inter) {
		*inter = ray.start + t * ray.dir;
	}

	return true;
}

bool Collision::CheckRay2Triangle(
    const Ray& ray, const ColTriangle& triangle, float* distance, DirectX::XMVECTOR* inter) {
	// 三角形が乗ってる平面を算出
	Plane plane;
	DirectX::XMVECTOR interPlane;
	plane.normal = triangle.normal;
	plane.distance = DirectX::XMVector3Dot(triangle.normal, triangle.p0).m128_f32[0];
	// レイと平面が当たってなければ当たってない
	if (!CheckRay2Plane(ray, plane, distance, &interPlane)) {
		return false;
	}
	//// レイと平面が当たっていたから距離と交点が書き込まれる
	/// レイと平面の交点が三角形の中にあるか判定
	// 誤差吸収用の微小な値
	const float epsilon = 1.0e-5f;
	DirectX::XMVECTOR m;
	// 辺p0_p1
	DirectX::XMVECTOR pt_p0 = triangle.p0 - interPlane;
	DirectX::XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	m = DirectX::XMVector3Cross(pt_p0, p0_p1);
	// 辺の外側であれば当たってないので処理を終わらせる
	if (DirectX::XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}
	// 辺p1_p2
	DirectX::XMVECTOR pt_p1 = triangle.p1 - interPlane;
	DirectX::XMVECTOR p1_p2 = triangle.p2 - triangle.p1;
	m = DirectX::XMVector3Cross(pt_p1, p1_p2);
	// 辺の外側であれば当たってないので処理を終わらせる
	if (DirectX::XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}
	// 辺p2_p0
	DirectX::XMVECTOR pt_p2 = triangle.p2 - interPlane;
	DirectX::XMVECTOR p2_p0 = triangle.p0 - triangle.p2;
	m = DirectX::XMVector3Cross(pt_p2, p2_p0);
	// 辺の外側であれば当たってないので処理を終わらせる
	if (DirectX::XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}
	// 全部内側なので当たってる
	if (inter) {
		*inter = interPlane;
	}

	return true;
}

bool Collision::CheckRay2Sphere(
    const Ray& ray, const Sphere& sphere, float* distance, DirectX::XMVECTOR* inter) {
	DirectX::XMVECTOR m = ray.start - sphere.center;
	float b = DirectX::XMVector3Dot(m, ray.dir).m128_f32[0];
	float c = DirectX::XMVector3Dot(m, m).m128_f32[0] - sphere.radius * sphere.radius;
	// レイの始点が球の外側かつ、レイが球の外側に向いてたら当たってない
	if (c > 0 && b > 0) {
		return false;
	}
	float discr = b * b - c;
	// 負の判別は当たってない
	if (discr < 0) {
		return false;
	}
	// レイは球と交差してる
	// 交差の最小値を算出
	float t = -b - sqrtf(discr);
	// tが負の場合、レイは球の内側から開始してるのでtを0にクランプ
	if (t < 0) {
		t = 0;
	}
	if (distance) {
		*distance = t;
	}
	if (inter) {
		*inter = ray.start + t * ray.dir;
	}

	return true;
}

namespace MyCollisions {
bool CheckSphereToSphere(
    const KMyMath::Vector3& posA, const KMyMath::Vector3& posB, const float radiusA,
    const float radiusB) {
	float disX = posB.x - posA.x;
	float disY = posB.y - posA.y;
	float disZ = posB.z - posA.z;
	float d = disX * disX + disY * disY + disZ * disZ;
	float r = radiusA + radiusB;
	return d < r * r;
}
bool CheckBoxToBox(
    const KMyMath::Vector3& boxPosA, const KMyMath::Vector3& boxPosB,
    const KMyMath::Vector3& boxScaleA, const KMyMath::Vector3& boxScaleB) {

	KMyMath::Vector3 posAplus = {
	    boxPosA.x + boxScaleA.x, boxPosA.y + boxScaleA.y, boxPosA.z + boxScaleA.z};
	KMyMath::Vector3 posAminus = {
	    boxPosA.x - boxScaleA.x, boxPosA.y - boxScaleA.y, boxPosA.z - boxScaleA.z};

	KMyMath::Vector3 posBplus = {
	    boxPosB.x + boxScaleB.x, boxPosB.y + boxScaleB.y, boxPosB.z + boxScaleB.z};
	KMyMath::Vector3 posBminus = {
	    boxPosB.x - boxScaleB.x, boxPosB.y - boxScaleB.y, boxPosB.z - boxScaleB.z};

	if (posAminus.x <= posBplus.x && // posA左とposB右の判定
	    posAplus.x >= posBminus.x && // posA右とposB左の判定
	    posAminus.y <= posBplus.y && // posA下とposB上の判定
	    posAplus.y >= posBminus.y && // posA上とposB下の判定
	    posAminus.z <= posBplus.z && // posA前とposB奥の判定
	    posAplus.z >= posBminus.z)   // posA奥とposB前の判定
	{
		return true;
	}

	return false;
}
} // namespace MyCollisions