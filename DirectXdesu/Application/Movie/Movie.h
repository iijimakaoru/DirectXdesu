#pragma once
#include "GameManager.h"
#include "KInput.h"
#include "Sprite.h"
#include <array>
#include <memory>
#include <stdint.h>
#include "Ease.h"

class BaseMovie {
public:
	virtual ~BaseMovie();

	void Init();

	virtual void Update() = 0;

	void Draw();

	// ムービーバー初期化
	virtual void MovieBarInInit();
	virtual void MovieBarOutInit();

	// ムービーバーにょっき
	virtual void MovieBarOut(const float timer_);

	// ムービーバーにょっき
	virtual void MovieBarIn(const float timer_);

	const bool& GetIsFinish() const;

	void SetIsFinish(bool isFinish);

private:
	bool isFinish_ = false;

protected:
	KInput* input_ = nullptr;
	GameManager* gameManager_ = nullptr;
	float phaseTimer_ = 0;
	float phaseTime_ = 0;

	//
	std::array<std::unique_ptr<Sprite>, 2> movieBar;
	std::array<KMyMath::Vector2, 2> movieBarPos;
};
