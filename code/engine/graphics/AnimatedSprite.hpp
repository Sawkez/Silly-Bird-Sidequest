#pragma once

#include <SDL.h>

#include <cmath>
#include <vector>

#include "engine/IProcessable.hpp"
#include "engine/Math.hpp"
#include "engine/Vector2.hpp"
#include "engine/graphics/Animation.hpp"
#include "engine/graphics/IDrawableRect.hpp"
#include "engine/graphics/PlaybackPosition.hpp"

const float DEGREES_PER_RADIAN = 180.0 / M_PI;

using namespace std;

class AnimatedSprite : IDrawableRect, IProcessable {
   protected:
	SDL_Rect _source{};
	SDL_FRect _destination{};
	vector<Animation> _animations;
	Vector2 _offset = Vector2::ZERO;
	Vector2 _scaleOrigin = Vector2::ZERO;
	Vector2 _rotateOrigin = Vector2::ZERO;
	float _rotation = 0.0;
	int _current = 0;
	SDL_RendererFlip _flip = SDL_FLIP_NONE;

   public:
	Vector2 position = Vector2::ZERO;
	Vector2 scale{1.0, 1.0};

	AnimatedSprite(const vector<Animation>& animations, Vector2 offset = Vector2::ZERO, Vector2 scaleOrigin = Vector2::ZERO,
				   Vector2 rotateOrigin = Vector2{0.0, 0.0})
		: _animations(animations), _offset(offset), _scaleOrigin(scaleOrigin), _rotateOrigin(rotateOrigin) {}

	void Process(float delta) override {
		Animation& animation = _animations.at(_current);
		animation.Process(delta);
		_source = animation.GetSourceRect();
		_destination = GetRect();
	}

	bool Draw(SDL_Renderer* renderer, const SDL_FRect& drawTargetRect, Vector2 drawOffset = {}) const override {
		SDL_FRect destination = _destination;
		destination.x += drawOffset.x;
		destination.y += drawOffset.y;

		if (!SDL_HasIntersectionF(&drawTargetRect, &destination)) {
			return false;
		}

		const Animation& animation = _animations.at(_current);

		int error = SDL_RenderCopyExF(renderer, animation.GetTexture(), &_source, &destination, _rotation, &_rotateOrigin, _flip);

		if (error < 0) {
			std::cerr << "ERROR: couldn't draw AnimatedSprite: " << SDL_GetError() << std::endl;
			return false;
		}

		return true;
	}

	void SetRotationDegrees(float degrees) { _rotation = degrees; }

	void SetRotationRadians(float radians) { _rotation = radians * DEGREES_PER_RADIAN; }

	float GetRotationRadians() const { return _rotation / DEGREES_PER_RADIAN; }

	void SetFlip(SDL_RendererFlip flip) { _flip = flip; }

	void Play(int animationID, float speed = 1.0) {
		Animation& newAnimation = _animations.at(animationID);

		if (_current != animationID) newAnimation.Restart();

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

	SDL_FRect GetRect() const {
		Vector2 frameSize = _animations.at(_current).GetFrameSize();
		SDL_FRect rect{position.x, position.y, frameSize.x, frameSize.y};
		rect = Math::ScaleRect(rect, _scaleOrigin, scale);
		rect.x += _offset.x - _rotateOrigin.x;
		rect.y += _offset.y - _rotateOrigin.y;

		return rect;

		/*
		Vector2 sizeScaled = _animations.at(_current).GetFrameSize() * scale;
		return SDL_FRect{position.x - _scaleOrigin.x * scale.x + _scaleOrigin.x + _offset.x - _rotateOrigin.x,
						 position.y - _scaleOrigin.y * scale.y + _scaleOrigin.y + _offset.y - _rotateOrigin.y,
						 sizeScaled.x, sizeScaled.y};
		*/
	}

	Vector2 TransformPoint(Vector2 point) const {
		if (_flip & SDL_FLIP_HORIZONTAL != 0) point.x = -point.x;
		point *= scale * 0.5;
		return point.Rotated(GetRotationRadians());
	}

	PlaybackPosition GetPlaybackPosition() const { return {_current, _animations[_current].GetFrame()}; }
};