#pragma once
#include <stdint.h>

class ActState
{
public:
	virtual ~ActState();

	// 更新
	virtual void Update() = 0;

	const bool GetIsFinish() const;

protected:
	// アクション終わり
	bool isFinish = false;

	// アクション時間
	uint32_t actTimer = 0;
	uint32_t actTime = 0;

	// アクションスタートタイム
	float actStartTimer = 0;
	float actStartTime = 0;
};

