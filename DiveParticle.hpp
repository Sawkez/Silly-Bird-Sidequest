#pragma once

#include <SDL.h>

#include "Curve.hpp"
#include "IParticle.hpp"
#include "Random.hpp"
#include "Vector2.hpp"

class DiveParticle : public IParticle {
  private:
	static constexpr float TEXTURE_SIZE = 8.0;
	static constexpr float MIN_INITIAL_SPEED = 35.0;
	static constexpr float MAX_INITIAL_SPEED = 50.0;
	static constexpr float MAX_ROTATION_SPEED = M_PI * 2.0;
	static constexpr float MIN_LIFE_TIME_MULT = 0.6;
	static constexpr float DAMPENING = 0.25;

	Vector2 _velocity;
	float _angle = 0.0;
	float _rotationSpeed = 0.0;
	float _time = 0.0;
	static Curve _scaleCurve;

  public:
	DiveParticle(SDL_Texture* texture) : IParticle(texture) {}

	void Start(Vector2 position) {
		_position = position;
		_velocity = Random::Direction() * Random::Float(MIN_INITIAL_SPEED, MAX_INITIAL_SPEED);
		_angle = Random::Angle();
		_rotationSpeed = Random::Float(-MAX_ROTATION_SPEED, MAX_ROTATION_SPEED);
		_time = GetMaxLifeTime() * Random ::Float(MIN_LIFE_TIME_MULT, 1.0);
	}

	void Process(float delta) override {
		_time -= delta;
		_position += _velocity * delta;
		_angle += _rotationSpeed;
		_velocity *= powf(DAMPENING, delta);
	}

	void Draw(SDL_Renderer* renderer, Vector2 drawOffset) const override {
		float lifeTimeRatio = 1.0f - _time / GetMaxLifeTime();
		float scale = _scaleCurve.LerpSample(lifeTimeRatio);

		Vector2 center(TEXTURE_SIZE - TEXTURE_SIZE / 2.0, TEXTURE_SIZE - TEXTURE_SIZE / 2.0);

		SDL_FRect destination{_position.x + drawOffset.x - center.x, _position.y - center.y + drawOffset.y,
							  TEXTURE_SIZE, TEXTURE_SIZE};
		destination = Math::ScaleRect(destination, center, scale);

		SDL_RenderCopyExF(renderer, _texture, NULL, &destination, _angle, NULL, SDL_FLIP_NONE);
	}

	bool IsActive() const override { return _time > 0.0; }

	static float GetMaxLifeTime() { return 0.75; }
};

Curve DiveParticle::_scaleCurve = Curve("content/curves/particles/dive_scale.curve");