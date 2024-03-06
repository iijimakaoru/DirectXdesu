#include "Transform.h"

void Transform::TransUpdate() {// マトリックス
	KMyMath::Matrix4 matScale, matRot, matTrans;
	matScale = matRot = matTrans = MyMathUtility::MakeIdentity();

	// 親オブジェクト要素
	matScale = MyMathUtility::MakeScaling(scale);
	matRot = MyMathUtility::MakeRotation(
	    {DirectX::XMConvertToRadians(rot.x), DirectX::XMConvertToRadians(rot.y),
	     DirectX::XMConvertToRadians(rot.z)});
	matTrans = MyMathUtility::MakeTranslation(pos);

	// 行列初期化
	matWorld = MyMathUtility::MakeIdentity();
	//
	matWorld *= MyMathUtility::MakeWorld(matTrans, matScale, matRot);

	if (parent) {
		matWorld *= parent->matWorld;
	}
}

const KMyMath::Vector3& Transform::GetPos() const { return pos; }

const KMyMath::Vector3& Transform::GetRot() const { return rot; }

const KMyMath::Vector3& Transform::GetScale() const { return scale; }

const KMyMath::Matrix4& Transform::GetMatWorld() const { return matWorld; }

const KMyMath::Vector3 Transform::GetWorldPos() const { // ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result.x = matWorld.m[3][0];
	result.y = matWorld.m[3][1];
	result.z = matWorld.m[3][2];

	return result;
}

const Transform* Transform::GetParent() const { return parent; }

void Transform::SetPos(const KMyMath::Vector3& pos_) { pos = pos_; }

void Transform::SetRot(const KMyMath::Vector3& rot_) { rot = rot_; }

void Transform::SetScale(const KMyMath::Vector3& scale_) { scale = scale_; }

void Transform::SetMatWorld(const KMyMath::Matrix4& matWorld_) { matWorld = matWorld_; }

void Transform::SetParent(const Transform* parent_) { parent = parent_; }

void Transform::AddSetPos(const KMyMath::Vector3& pos_) { pos += pos_; }

void Transform::AddSetRot(const KMyMath::Vector3& rot_) { rot += rot_; }

void Transform::AddSetScale(const KMyMath::Vector3& scale_) { scale += scale_; }
