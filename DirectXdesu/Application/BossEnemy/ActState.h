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
	float actTimer = 0;
	float actTime = 0;
};

