#pragma once

#include <SDL.h>

#include "IDrawable.hpp"
#include "IProcessable.hpp"

class IParticle : public IDrawable, public IProcessable {
  protected:
	Vector2 _position = Vector2::ZERO;
	SDL_Texture* _texture;

  public:
	IParticle(SDL_Texture* texture) : _texture(texture) {}
	virtual void Start(Vector2 position) = 0;
	virtual bool IsActive() const = 0;
	static float GetLifeTime();
};