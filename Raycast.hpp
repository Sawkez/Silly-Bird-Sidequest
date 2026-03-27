#pragma once

#include <vector>

#include "CollisionRect.hpp"
#include "CollisionResult.hpp"
#include "Vector2.hpp"

class Raycast {
   public:
	enum Axis { NONE, LEFT, RIGHT, UP, DOWN };

   private:
	Axis _axis = NONE;
	float _length;
	Vector2 _position;
	Vector2 _localTarget;

	bool CheckAxisCollision(const std::vector<CollisionRect>& colliders) const {
		for (const auto& rect : colliders) {
			switch (_axis) {
				case LEFT:
				case RIGHT:
					if (_position.y < rect.y || _position.y > rect.y + rect.h) continue;
					break;

				case UP:
				case DOWN:
					if (_position.x < rect.x || _position.x > rect.x + rect.w) continue;
					break;
			}

			float relative;

			switch (_axis) {
				case LEFT:
					relative = _position.x - (rect.x + rect.w);
					break;

				case RIGHT:
					relative = rect.x - _position.x;
					break;

				case UP:
					relative = _position.y - (rect.y + rect.h);
					break;

				case DOWN:
					relative = rect.y - _position.y;
					break;
			}

			if (relative >= 0.0 && relative <= _length) return true;
		}

		return false;
	}

   public:
	Raycast(float x, float y, float xTarget, float yTarget) : _position(x, y) { SetLocalTarget(Vector2(xTarget, yTarget)); }

	Raycast(Vector2 position, Vector2 localTarget) : _position(position) { SetLocalTarget(localTarget); }

	Raycast(Vector2 position, Axis axis, float length) : _axis(axis), _position(position), _length(length), _localTarget(Vector2(0.0, 0.0)) {}

	void SetPosition(Vector2 position) { _position = position; }

	void SetLocalTarget(Vector2 target) {
		_localTarget = target;

		_axis = NONE;

		if (target.x == 0.0) {
			if (target.y > 0.0) {
				_axis = DOWN;
				_length = target.y;
			}

			else if (target.y < 0.0) {
				_axis = UP;
				_length = -target.y;
			}
		}

		else if (target.y == 0.0) {
			if (target.x > 0.0) {
				_axis = RIGHT;
				_length = target.x;
			}

			else if (target.x < 0.0) {
				_axis = LEFT;
				_length = -target.x;
			}
		}

		else
			_length = target.Length();
	}

	void SetGlobalTarget(Vector2 target) { SetLocalTarget(target - _position); }

	bool CheckCollision(const std::vector<CollisionRect>& colliders) const {
		if (_axis != NONE) {
			return CheckAxisCollision(colliders);
		}

		else {
			std::cerr << "ERROR: generalized raycasts are not implemented!" << std::endl;
			return false;  // TODO implement
		}
	}
};