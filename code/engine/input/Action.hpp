#pragma once

#include <algorithm>

class Action {
   public:
	static const int BINDINGS_PER_ACTION = 2;

	static const auto LEFT_TRIGGER_BUTTON = (SDL_GameControllerButton)(SDL_CONTROLLER_BUTTON_MAX + 1);
	static const auto RIGHT_TRIGGER_BUTTON = (SDL_GameControllerButton)(SDL_CONTROLLER_BUTTON_MAX + 2);

   private:
	SDL_Scancode _keys[BINDINGS_PER_ACTION];
	SDL_GameControllerButton _buttons[BINDINGS_PER_ACTION];

	bool _tap = false;
	bool _down = false;

   public:
	Action(SDL_Scancode key0, SDL_Scancode key1, SDL_GameControllerButton btn0, SDL_GameControllerButton btn1)
		: _keys{key0, key1}, _buttons{btn0, btn1} {}

	bool HasKey(int key) const {
		for (int i = 0; i < BINDINGS_PER_ACTION; i++) {
			if (_keys[i] == key) return true;
		}

		return false;
	}

	bool HasButton(int button) const {
		for (int i = 0; i < BINDINGS_PER_ACTION; i++) {
			if (_buttons[i] == button) return true;
		}

		return false;
	}

	void SetDownNoTap(bool down) { _down = down; }

	void SetDown(bool down) {
		if (down && !_down) _tap = true;
		SetDownNoTap(down);
	}

	void ResetToKeyboardState(const Uint8* state) {
		for (int i = 0; i < BINDINGS_PER_ACTION; i++) {
			if (state[_keys[i]]) {
				SetDownNoTap(true);
				return;
			}
		}
		SetDownNoTap(false);
	}

	void ResetToControllerState(SDL_GameController* controller) {
		for (int i = 0; i < BINDINGS_PER_ACTION; i++) {
			if (SDL_GameControllerGetButton(controller, _buttons[i])) {
				SetDownNoTap(true);
				return;
			};
		}
		SetDownNoTap(false);
	}

	void UpdateTapState() { _tap = false; }

	bool IsDown() const { return _down; }

	bool IsTapped() const { return _tap; }
};
