#pragma once
#include "GameManager.h"
#include "KInput.h"
#include "Sprite.h"
#include <array>
#include <memory>
#include <stdint.h>
#include "Ease.h"

class Movie {
public:
	void Init();

	virtual void Update() = 0;

	void Draw();

	// ムービーバー初期化
	void MovieBarInInit();
	void MovieBarOutInit();

	// ムービーバーにょっき
	void MovieBarOut(const float timer_);

	// ムービーバーにょっき
	void MovieBarIn(const float timer_);

	const bool& GetIsFinish() const;

	void SetIsFinish(bool isFinish);

private:
	//
	std::array<std::unique_ptr<Sprite>, 2> movieBar;
	std::array<KMyMath::Vector2, 2> movieBarPos;

	bool isFinish_ = false;

protected:
	KInput* input_ = nullptr;
	GameManager* gameManager_ = nullptr;
	float phaseTimer = 0;
	float phaseTime = 0;
};
