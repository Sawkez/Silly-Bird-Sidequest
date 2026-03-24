#pragma once

class Player;

class IMovementState {
   public:
	virtual void Init(Player& p) const = 0;
	virtual void Process(Player& p, float delta) const = 0;
	virtual void Deinit(Player& p) const = 0;
};