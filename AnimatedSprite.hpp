#pragma once

#include <SDL.h>
#include <cmath>
#include <vector>

#include "Animation.hpp"
#include "IDrawable.hpp"
#include "IProcessable.hpp"
#include "Vector2.hpp"

const float DEGREES_PER_RADIAN = 180.0 / M_PI;

using namespace std;

class AnimatedSprite : IDrawable, IProcessable {
  private:
	vector<Animation> _animations;
	Vector2 _offset = Vector2::ZERO;
	Vector2 _scaleOrigin = Vector2::ZERO;
	Vector2 _rotateOrigin = Vector2::ZERO;
	SDL_RendererFlip _flip = SDL_FLIP_NONE;
	float _rotation = 0.0;
	int _current = 0;

  public:
	Vector2 position = Vector2::ZERO;
	Vector2 scale{1.0, 1.0};

	AnimatedSprite(const vector<Animation>& animations, Vector2 offset = Vector2::ZERO,
				   Vector2 scaleOrigin = Vector2::ZERO, Vector2 rotateOrigin = Vector2{0.0, 0.0})
		: _animations(animations), _offset(offset), _scaleOrigin(scaleOrigin), _rotateOrigin(rotateOrigin) {}

	void Process(float delta) override { _animations.at(_current).Process(delta); }

	void Draw(SDL_Renderer* renderer, Vector2 drawOffset = {}) const override {
		const Animation& animation = _animations.at(_current);

		SDL_Rect source = animation.GetSourceRect();

		Vector2 sizeScaled = animation.GetFrameSize() * scale;

		SDL_FRect destination{
			position.x - _scaleOrigin.x * scale.x + _scaleOrigin.x + _offset.x - _rotateOrigin.x + drawOffset.x,
			position.y - _scaleOrigin.y * scale.y + _scaleOrigin.y + _offset.y - _rotateOrigin.y + drawOffset.y,
			sizeScaled.x, sizeScaled.y};

		int error = SDL_RenderCopyExF(renderer, animation.GetTexture(), &source, &destination, _rotation,
									  &_rotateOrigin, _flip);

		if (error < 0) {
			std::cerr << "ERROR: couldn't draw AnimatedSprite: " << SDL_GetError() << std::endl;
		}
	}

	void SetRotationDegrees(float degrees) { _rotation = degrees; }

	void SetRotationRadians(float radians) { _rotation = radians * DEGREES_PER_RADIAN; }

	void SetFlip(SDL_RendererFlip flip) { _flip = flip; }

	void Play(int animationID, float speed = 1.0) {
		Animation& newAnimation = _animations.at(animationID);

		if (_current != animationID)
			newAnimation.Restart();

		// TODO transitions

		newAnimation.SetSpeed(speed);
		_current = animationID;
	}

	void PlayFromStart(int animationID, float speed = 1.0) {
		Play(animationID, speed);
		_animations[animationID].SetFrame(0);
	}

	void PlayLastFrame(int animationID, float speed = 1.0) {
		Play(animationID, speed);
		_animations[animationID].SetLastFrame();
	}
};