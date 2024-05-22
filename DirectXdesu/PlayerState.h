#pragma once
class PlayerState {
public:
	PlayerState();
	~PlayerState(){};

	virtual void Init() = 0;

	virtual void Update() = 0;

protected:
};
