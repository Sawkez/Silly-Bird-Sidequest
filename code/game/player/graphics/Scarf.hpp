#pragma once

#include <SDL3/SDL.h>

#include <cmath>
#include <functional>
#include <vector>

#include "engine/IProcessable.hpp"
#include "engine/Math.hpp"
#include "engine/graphics/FColor.hpp"
#include "engine/graphics/IDrawableRect.hpp"
#include "engine/physics/CollisionRect.hpp"

using namespace std;

class Scarf : IProcessable, IDrawableRect {
   private:
	static constexpr int SEGMENT_COUNT = 10;
	static constexpr float MOVE_SPEED = 45.0;
	static constexpr float LOW_WIND_SPEED = 30.0;
	static constexpr float SEGMENT_DIST = 2.0;
	static constexpr float MAX_DIST = 2.5;
	static constexpr float TIME_SCALE = 4.2;
	static constexpr float SINE_SCALE = 0.2;
	static constexpr float WIND_DIF = -0.25;

	static constexpr float COLOR_CHANGE_SPEED = 2.5;

	static constexpr int GEOMETRY_INDEX_COUNT = 18 * 3;
	static constexpr int GEOMETRY_INDICES[GEOMETRY_INDEX_COUNT] = {0,  1,  2,  1,  2,  3,  2,  3,  4,  3,  4,  5,  4,  5,  6,  5,  6,  7,
																   6,  7,  8,  7,  8,  9,  8,  9,  10, 9,  10, 11, 10, 11, 12, 11, 12, 13,
																   12, 13, 14, 13, 14, 15, 14, 15, 16, 15, 16, 17, 16, 17, 18, 17, 18, 19};

	FColor _chargedColor{198, 15, 64};
	FColor _emptyColor{255, 240, 35};
	FColor _activeColor{255, 255, 255};

	FColor _currentColor;
	FColor _targetColor;

	Vector2 _segmentPositions[SEGMENT_COUNT];
	float _windStrength = 60.0;
	float _windAngle = M_PI / 4.0;
	float _time = 0.0;
	reference_wrapper<const vector<CollisionRect>> _staticColliders;

   public:
	Scarf(const vector<CollisionRect>& colliders) : _staticColliders(ref(colliders)), _currentColor(_chargedColor), _targetColor(_chargedColor) {
		for (int i = 0; i < SEGMENT_COUNT; i++) {
			_segmentPositions[i] = Vector2(0.1, 0.1) * i;
		}
	}

	void Pin(Vector2 pinPosition) { _segmentPositions[0] = pinPosition; }

	void Process(float delta) {
		_windStrength = (M_PI * 0.5) * fabsf(M_PI * 0.5 - _windAngle);
		_time += delta * _windStrength * TIME_SCALE;

		_currentColor.MoveToward(_targetColor, COLOR_CHANGE_SPEED * delta);

		for (int i = 1; i < SEGMENT_COUNT; i++) {
			Vector2& pos = _segmentPositions[i];
			const Vector2& target = _segmentPositions[i - 1];

			if (isnan(pos.x) || isnan(pos.y)) {
				pos = target;
				return;
			}

			float _segmentWindAngle = _windAngle + sinf(_time + i * WIND_DIF) * _windStrength * SINE_SCALE;
			Vector2 wind{cosf(_segmentWindAngle), sinf(_segmentWindAngle)};
			wind *= SEGMENT_DIST;

			/* more faithful to the Godot version but probably unnecessary
			float dist = pos.Distance(target);
			float speed = MOVE_SPEED * (dist / MAX_DIST) + max(0.0, LOW_WIND_SPEED * (1.0 - _windStrength));
			*/

			float speed = MOVE_SPEED + max(0.0, LOW_WIND_SPEED * (1.0 - _windStrength));

			bool colliding = false;

			for (auto collider : _staticColliders.get()) {
				if (SDL_PointInRectFloat(&pos, &collider)) {
					colliding = true;
					break;
				}
			}

			if (!colliding) {
				pos.MoveToward(target + wind, speed * delta);
			}

			if (pos.DistanceSquared(target) > MAX_DIST * MAX_DIST) {
				pos = target + target.DirectionTo(pos) * MAX_DIST;
			}
		}
	}

	SDL_Color GetColor() const { return _currentColor.GetIntColor(); }

	SDL_FRect GetRect() const {
		if (isnan(_segmentPositions[SEGMENT_COUNT - 1].x) || isnan(_segmentPositions[SEGMENT_COUNT - 1].y)) return {0.0, 0.0, 0.0, 0.0};

		SDL_FRect rect;
		Vector2 drawPoints[2] = {_segmentPositions[0], _segmentPositions[SEGMENT_COUNT - 1]};
		SDL_GetRectEnclosingPointsFloat(drawPoints, 2, NULL, &rect);
		return rect;
	}

	void SetColliders(const vector<CollisionRect>& colliders) { _staticColliders = ref(colliders); }

	bool Draw(SDL_Renderer* renderer, const SDL_FRect& drawTargetRect, Vector2 drawOffset) const override {
		SDL_FRect rect = GetRect();
		rect.x += drawOffset.x;
		rect.y += drawOffset.y;

		if (!SDL_HasRectIntersectionFloat(&rect, &drawTargetRect)) {
			return false;
		}

		SDL_Vertex vertices[SEGMENT_COUNT * 2];

		Vector2 firstOffset = {0.0, (_segmentPositions[1] - _segmentPositions[0]).x > 0.0f ? 1.0f : -1.0f};

		vertices[0] = {_segmentPositions[0] + drawOffset + firstOffset, _currentColor};
		vertices[1] = {_segmentPositions[0] + drawOffset - firstOffset, _currentColor};

		for (int i = 1; i < SEGMENT_COUNT; i++) {
			Vector2 dir = _segmentPositions[i].DirectionTo(_segmentPositions[i - 1]);
			// rotating 90 degrees quickly
			dir = Vector2{-dir.y, dir.x};

			vertices[i * 2] = SDL_Vertex{_segmentPositions[i] - dir + drawOffset, _currentColor};

			vertices[i * 2 + 1] = SDL_Vertex{_segmentPositions[i] + dir + drawOffset, _currentColor};
		}

		if (SDL_RenderGeometry(renderer, NULL, vertices, SEGMENT_COUNT * 2, GEOMETRY_INDICES, GEOMETRY_INDEX_COUNT) < 0) {
			cout << "ERROR rendering scarf geometry: " << SDL_GetError() << endl;
			return false;
		};

		return true;
	}

	void Load() {
		_currentColor = _activeColor;
		_targetColor = _chargedColor;
	}

	void Unload() {
		_currentColor = _activeColor;
		_targetColor = _emptyColor;
	}
};