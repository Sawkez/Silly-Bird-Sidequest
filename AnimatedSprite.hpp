#pragma once

#include <cmath>

#include <SDL.h>

#include "Animation.hpp"
#include "Vector2.hpp"

class AnimatedSprite {
    private:
        Animation _animation;
        Vector2 _position = Vector2::ZERO;

    public:
        AnimatedSprite(Animation animation) :
        _animation(animation)
        { }

        void Process(float delta) {
            _animation.Process(delta);
        }

        void Draw(SDL_Renderer* renderer) const {

            SDL_Rect source = _animation.GetSourceRect();

            SDL_Rect destination{
                int(std::round(_position.x)),
                int(std::round(_position.y)),
                _animation.GetFrameWidth() * 4,
                _animation.GetFrameHeight() * 4
            };

            int error = SDL_RenderCopyEx(
                renderer,
                _animation.GetTexture(),
                &source,
                &destination,
                0.0,
                NULL,
                SDL_RendererFlip::SDL_FLIP_NONE
            );

            if (error < 0) {
                std::cerr << "ERROR: couldn't draw AnimatedSprite: " << SDL_GetError() << std::endl;
            }
        }

        Vector2 GetPosition() const {
            return _position;
        }

        void SetPosition(Vector2 position) {
            _position = position;
        }
};