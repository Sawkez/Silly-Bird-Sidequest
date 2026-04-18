#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <string>

#include "engine/Math.hpp"
#include "engine/Vector2.hpp"
#include "yyjson.h"

class Animation {
   private:
	SDL_Texture* _texture;
	int _frameCount;
	int _frameWidth;
	int _frameHeight;
	int _frame = 0;
	float _frameDuration;
	float _frameTime = 0.0;
	float _speed = 1.0;
	bool _looping;

   public:
	Animation(SDL_Texture* texture, int frameCount, float fps, bool looping = true)
		: _texture(texture), _frameCount(frameCount), _frameDuration(1.0 / fps), _looping(looping) {
		float w, h;
		SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
		SDL_GetTextureSize(texture, &w, &h);
		_frameWidth = int(w);
		_frameHeight = int(h);

		_frameWidth /= _frameCount;
	}

	Animation(SDL_Texture* texture, yyjson_val* animJson)
		: Animation(texture, yyjson_get_int(yyjson_obj_get(animJson, "frame_count")), yyjson_get_num(yyjson_obj_get(animJson, "fps")),
					yyjson_get_bool(yyjson_obj_get(animJson, "looping"))) {}

	Animation(SDL_Renderer* renderer, std::string path, int frameCount, float fps, bool looping = true)
		: Animation(IMG_LoadTexture(renderer, path.data()), frameCount, fps, looping) {
		if (_texture == NULL) {
			std::cerr << "ERROR: couldn't load texture " << path << ": " << SDL_GetError() << std::endl;
		}
	}

	void Process(float delta) {
		_frameTime += delta * _speed;

		while (_frameTime >= _frameDuration) {
			_frame++;
			_frameTime -= _frameDuration;
		}

		if (_frame >= _frameCount) {
			if (_looping) {
				_frame = Math::Wrap(_frame, 0, _frameCount - 1);
			} else {
				_frame = _frameCount - 1;
			}
		}
	}

	SDL_FRect GetSourceRect() const { return SDL_FRect{float(_frame) * _frameWidth, 0, float(_frameWidth), float(_frameHeight)}; }

	SDL_Texture* GetTexture() const { return _texture; }

	int GetFrameWidth() const { return _frameWidth; }

	int GetFrameHeight() const { return _frameHeight; }

	Vector2 GetFrameSize() const { return Vector2{float(_frameWidth), float(_frameHeight)}; }

	void SetSpeed(float speed) { _speed = speed; }

	void SetFrame(int frame) { _frame = frame; }

	void SetLastFrame() { _frame = _frameCount - 1; }

	void Restart() { SetFrame(0); }

	int GetFrame() const { return _frame; }
};