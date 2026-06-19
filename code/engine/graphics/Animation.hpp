#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <string>

#include "engine/Math.hpp"
#include "engine/Vector2.hpp"
#include "engine/graphics/PlaybackPosition.hpp"
#include "yyjson.h"

// Animation based on a sprite sheet.
// Sprite sheet must have power of 2 width.
// Each frame of the animation must be square.
class Animation {
   private:
	SDL_Texture* _texture;
	int _frameCount;
	int _frameCountHorizontal;
	int _frameSize;
	float _frameDuration;
	bool _looping;

   public:
	Animation(SDL_Texture* texture, int frameCount, float fps, bool looping = true)
		: _texture(texture),
		  _frameCount(frameCount),
		  _frameCountHorizontal(1),
		  _frameDuration(1.0 / fps),
		  _looping(looping) {
		float w;
		SDL_GetTextureSize(texture, &w, nullptr);
		int textureWidth = int(w);

		while (_frameCountHorizontal * _frameCountHorizontal < _frameCount) {
			_frameCountHorizontal = _frameCountHorizontal << 1;
		}

		_frameSize = textureWidth / _frameCountHorizontal;
	}

	Animation(SDL_Texture* texture, yyjson_val* animJson)
		: Animation(texture, int(yyjson_get_num(yyjson_obj_get(animJson, "frame_count"))),
					yyjson_get_num(yyjson_obj_get(animJson, "fps")),
					yyjson_get_bool(yyjson_obj_get(animJson, "looping"))) {}

	Animation(SDL_Renderer* renderer, std::string path, int frameCount, float fps, bool looping = true)
		: Animation(IMG_LoadTexture(renderer, path.data()), frameCount, fps, looping) {
		if (_texture == NULL) {
			dc::err << "ERROR: couldn't load texture " << path << ": " << SDL_GetError() << dc::endl;
		}
	}

	void Process(float delta, PlaybackPosition& playbackPosition, float speed) {
		playbackPosition.frameTime += delta * speed;

		while (playbackPosition.frameTime >= _frameDuration) {
			playbackPosition.frame++;
			playbackPosition.frameTime -= _frameDuration;
		}

		if (playbackPosition.frame >= _frameCount) {
			if (_looping) {
				playbackPosition.frame = Math::Wrap(playbackPosition.frame, 0, _frameCount - 1);
			} else {
				playbackPosition.frame = _frameCount - 1;
			}
		}
	}

	SDL_FRect GetSourceRect(const PlaybackPosition& playbackPosition) const {
		int row = playbackPosition.frame / _frameCountHorizontal;
		int collumn = playbackPosition.frame - row * _frameCountHorizontal;

		return SDL_FRect{float(collumn * _frameSize), float(row * _frameSize), float(_frameSize), float(_frameSize)};
	}

	SDL_Texture* GetTexture() const { return _texture; }

	int GetFrameWidth() const { return _frameSize; }

	int GetFrameHeight() const { return _frameSize; }

	int GetFrameCount() const { return _frameCount; }

	Vector2 GetFrameSize() const { return Vector2{float(_frameSize), float(_frameSize)}; }
};