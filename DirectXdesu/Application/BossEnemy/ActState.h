#pragma once
#include <stdint.h>

class ActState
{
public:
	virtual ~ActState();

	// 更新
	virtual void Update() = 0;

	const bool GetIsFinish() const;

	static ActState* nowActState;

protected:
	// アクション終わり
	bool isFinish = false;

	// アクションフェーズ
	uint32_t actPhase = 0;

	// アクション時間
	float actTimer = 0;
	float actTime = 0;
};

