#pragma once

#include <SDL.h>
#include <utility>
#include <vector>

#include "IDrawable.hpp"
#include "IProcessable.hpp"
#include "Vector2.hpp"

template <typename ParticleType, Uint8 count> class ParticleSpawner : public IProcessable, public IDrawableRect {
  private:
	float _time = 0.0;
	SDL_FRect _boundingBox;
	SDL_Texture* _particleTexture;
	ParticleType _particles[count];
	Uint8 _particlePool[count];
	Uint8 _poolTop;
	float _spawnDelay;

	template <std::size_t... Is>
	ParticleSpawner<ParticleType, count>(SDL_FRect boundingBox, SDL_Texture* particleTexture,
										 std::index_sequence<Is...>)
		: _boundingBox(boundingBox), _particleTexture(particleTexture), _particles{((void)Is, particleTexture)...},
		  _spawnDelay(ParticleType::GetLifeTime() / float(count)), _poolTop(count) {
		for (Uint8 i = 0; i < count; i++) {
			_particlePool[i] = i;
		}
	}

  public:
	ParticleSpawner<ParticleType, count>(SDL_FRect boundingBox, SDL_Texture* particleTexture)
		: ParticleSpawner(boundingBox, particleTexture, std::make_index_sequence<count>{}) {}

	Vector2 position = Vector2::ZERO;

	void SpawnParticle() {
		if (_poolTop == 0) {
			return;
		}
		_poolTop--;
		_particles[_particlePool[_poolTop]].Start(position);
	}

	void Process(float delta) override {
		_time -= delta;

		while (_time <= 0.0) {
			_time += _spawnDelay;
			SpawnParticle();
		}

		for (int i = 0; i < count; i++) {
			if (_particles[i].IsActive()) {
				_particles[i].Process(delta);

				if (!_particles[i].IsActive()) {
					_poolTop++;
					_particlePool[_poolTop - 1] = i;
				}
			}
		}
	}

	void Draw(SDL_Renderer* renderer, Vector2 drawOffset) const override {
		for (int i = 0; i < count; i++) {
			if (_particles[i].IsActive())
				_particles[i].Draw(renderer, drawOffset);
		}

		SDL_FRect dest = GetRect();
		dest.x += drawOffset.x;
		dest.y += drawOffset.y;

		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 128);
		SDL_RenderDrawRectF(renderer, &dest);
	}

	SDL_FRect GetRect() const override {
		return SDL_FRect{position.x + _boundingBox.x, position.y + _boundingBox.y, _boundingBox.w, _boundingBox.h};
	}
};