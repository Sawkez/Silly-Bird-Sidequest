#pragma once

#include <SDL3/SDL.h>

#include <cmath>
#include <vector>

#include "engine/IProcessable.hpp"
#include "engine/Math.hpp"
#include "engine/Vector2.hpp"
#include "engine/devconsole/DevConsole.hpp"
#include "engine/graphics/Animation.hpp"
#include "engine/graphics/IDrawableRect.hpp"
#include "engine/graphics/PlaybackPosition.hpp"

const float DEGREES_PER_RADIAN = 180.0 / M_PI;

using namespace std;

class AnimatedSprite : IDrawableRect, IProcessable {
   protected:
	SDL_FRect _source{};
	SDL_FRect _destination{};
	vector<Animation> _animations;
	Vector2 _offset = Vector2::ZERO;
	Vector2 _scaleOrigin = Vector2::ZERO;
	Vector2 _rotateOrigin = Vector2::ZERO;
	float _rotation = 0.0;
	SDL_FlipMode _flip = SDL_FLIP_NONE;

	float _speed = 1.0f;
	PlaybackPosition _playbackPosition;

   public:
	Vector2 position = Vector2::ZERO;
	Vector2 scale{1.0, 1.0};

	AnimatedSprite(const vector<Animation>& animations, Vector2 offset = Vector2::ZERO,
				   Vector2 scaleOrigin = Vector2::ZERO, Vector2 rotateOrigin = Vector2{0.0, 0.0})
		: _animations(animations), _offset(offset), _scaleOrigin(scaleOrigin), _rotateOrigin(rotateOrigin) {}

	void Process(float delta) override {
		Animation& animation = _animations.at(_playbackPosition.animation);
		animation.Process(delta, _playbackPosition, _speed);
		_source = animation.GetSourceRect(_playbackPosition);
		_destination = GetRect();
	}

	bool Draw(SDL_Renderer* renderer, const SDL_FRect& drawTargetRect, Vector2 drawOffset = {}) const override {
		SDL_FRect destination = _destination;
		destination.x += drawOffset.x;
		destination.y += drawOffset.y;

		if (!SDL_HasRectIntersectionFloat(&drawTargetRect, &destination)) {
			return false;
		}

		const Animation& animation = _animations.at(_playbackPosition.animation);

		int error = SDL_RenderTextureRotated(renderer, animation.GetTexture(), &_source, &destination, _rotation,
											 &_rotateOrigin, _flip);

		if (error < 0) {
			dc::err << "ERROR: couldn't draw AnimatedSprite: " << SDL_GetError() << dc::endl;
			return false;
		}

		return true;
	}

	void SetRotationDegrees(float degrees) { _rotation = degrees; }

	void SetRotationRadians(float radians) { _rotation = radians * DEGREES_PER_RADIAN; }

	float GetRotationRadians() const { return _rotation / DEGREES_PER_RADIAN; }

	void SetFlip(SDL_FlipMode flip) { _flip = flip; }

	void Play(int animationID, float speed = 1.0) {
		Animation& newAnimation = _animations.at(animationID);

		if (_playbackPosition.animation != animationID) Restart();

		SetSpeed(speed);
		_playbackPosition.animation = animationID;
	}

	void PlayFromStart(int animationID, float speed = 1.0) {
		Play(animationID, speed);
		Restart();
	}

	void PlayLastFrame(int animationID, float speed = 1.0) {
		Play(animationID, speed);
		SetLastFrame();
	}

	void SetFrame(int frame) { _playbackPosition.frame = frame; }
	void SetLastFrame() { SetFrame(_animations[_playbackPosition.animation].GetFrameCount() - 1); }
	void Restart() { SetFrame(0); }

	void SetSpeed(float speed) { _speed = speed; }

	SDL_FRect GetRect() const {
		Vector2 frameSize = _animations.at(_playbackPosition.animation).GetFrameSize();
		SDL_FRect rect{position.x, position.y, frameSize.x, frameSize.y};
		rect = Math::ScaleRect(rect, _scaleOrigin, scale);
		rect.x += _offset.x - _rotateOrigin.x;
		rect.y += _offset.y - _rotateOrigin.y;

		return rect;
	}

	Vector2 TransformPoint(Vector2 point) const {
		if ((_flip & SDL_FLIP_HORIZONTAL) != 0) point.x = -point.x;
		point *= scale * 0.5;
		return point.Rotated(GetRotationRadians());
	}

	const PlaybackPosition& GetPlaybackPosition() const { return _playbackPosition; }
};