#pragma once

#include <cmath>

#include <SDL.h>

#include "IDrawable.hpp"
#include "IProcessable.hpp"
#include "Animation.hpp"
#include "Vector2.hpp"

const float DEGREES_PER_RADIAN = 180.0 / M_PI;

class AnimatedSprite : IDrawable, IProcessable {
    private:
        Animation _animation;
        Vector2 _offset = Vector2::ZERO;
        Vector2 _scaleOrigin = {0, 0};
        SDL_Point _rotateOrigin {0, 0};
        SDL_RendererFlip _flip = SDL_FLIP_NONE;
        float _rotation = 0.0;

    public:
        Vector2 position = Vector2::ZERO;
        Vector2 scale {1.0, 1.0};

        AnimatedSprite(Animation animation, Vector2 offset = Vector2::ZERO, Vector2 scaleOrigin = Vector2::ZERO, SDL_Point rotateOrigin = SDL_Point {0, 0}) :
        _animation(animation), _offset(offset), _scaleOrigin(scaleOrigin), _rotateOrigin(rotateOrigin)
        { }

        AnimatedSprite(Animation animation, Vector2 offset = Vector2::ZERO, Vector2 scaleOrigin = Vector2::ZERO, Vector2 rotateOrigin = Vector2::ZERO) :
        AnimatedSprite(animation, offset, scaleOrigin, SDL_Point{int(rotateOrigin.x), int(rotateOrigin.y)})
        { }

        void Process(float delta) override {
            _animation.Process(delta);
        }

        void Draw(SDL_Renderer* renderer) const override {

            SDL_Rect source = _animation.GetSourceRect();

            Vector2 sizeScaled = _animation.GetFrameSize() * scale;

            SDL_Rect destination{
                int(std::round(position.x - _scaleOrigin.x * scale.x + _scaleOrigin.x + _offset.x)) - _rotateOrigin.x,
                int(std::round(position.y - _scaleOrigin.y * scale.y + _scaleOrigin.y + _offset.y)) - _rotateOrigin.y,
                int(std::round(sizeScaled.x)),
                int(std::round(sizeScaled.y))
            };

            int error = SDL_RenderCopyEx(
                renderer,
                _animation.GetTexture(),
                &source,
                &destination,
                _rotation,
                &_rotateOrigin,
                _flip
            );

            if (error < 0) {
                std::cerr << "ERROR: couldn't draw AnimatedSprite: " << SDL_GetError() << std::endl;
            }
        }

        void SetRotationDegrees(float degrees) {
            _rotation = degrees;
        }

        void SetRotationRadians(float radians) {
            _rotation = radians * DEGREES_PER_RADIAN;
        }

        void SetFlip(SDL_RendererFlip flip) {
            _flip = flip;
        }
};