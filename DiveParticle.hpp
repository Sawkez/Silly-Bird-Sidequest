#pragma once

#include <SDL.h>

#include "IParticle.hpp"

class DiveParticle : public IParticle {
  private:
	float _time = 0.0;

  public:
	DiveParticle(SDL_Texture* texture) : IParticle(texture) {}

	void Start(Vector2 position) {
		_position = position;
		_time = GetLifeTime();
	}

	void Process(float delta) override { _time -= delta; }

	void Draw(SDL_Renderer* renderer, Vector2 drawOffset) const override {
		SDL_FRect destination{_position.x + drawOffset.x, _position.y + drawOffset.y, 8.0, 8.0};

		SDL_RenderCopyF(renderer, _texture, NULL, &destination);
	}

	bool IsActive() const override { return _time > 0.0; }

	static float GetLifeTime() { return 30.0; }
};