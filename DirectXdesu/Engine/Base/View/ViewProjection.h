#pragma once
#include <DirectXMath.h>
#include <DirectXTex.h>
#include "Vector3.h"
#include "KWinApp.h"
#include "MyMath.h"

using namespace DirectX;

class ViewProjection
{
public:
	ViewProjection();
	void Initialize();
	void Update();

private:
	// �J�����̋���
	float lenZ = 0;
	float nearZ = 0;
	float farZ = 0;
	// �r���[�ϊ��s��
	KMyMath::Matrix4 matView{};
	KMyMath::Vector3 eye{};
	KMyMath::Vector3 target{};
	KMyMath::Vector3 up{};
	// �ˉe�ϊ��s��̌v�Z
	KMyMath::Matrix4 matProjection{};

	float aspect = 0; // �A�X�y�N�g��

public:
	const KMyMath::Matrix4& GetMatView() const;
	const KMyMath::Matrix4& GetMatPro() const;

	const KMyMath::Vector3& GetEye() const;
	const KMyMath::Vector3& GetTarget() const;
	const KMyMath::Vector3& GetUp() const;

	void SetMatView(const KMyMath::Matrix4 mat_);

	void SetEye(const KMyMath::Vector3& eye_);
	void SetTarget(const KMyMath::Vector3& target_);
	void SetUp(const KMyMath::Vector3& up_);
	void SetFarZ(const float& farZ_);
	void SetNearZ(const float& nearZ_);
};

