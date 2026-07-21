#pragma once

#include <algorithm>
#include <functional>
#include <unordered_map>
#include <vector>

class Action {
   public:
	static inline const int BINDINGS_PER_ACTION = 2;
	static inline const Uint64 DOUBLE_TAP_WINDOW = 200000000;  // 200 ms

	static inline const auto LEFT_TRIGGER_BUTTON = (SDL_GamepadButton)(SDL_GAMEPAD_BUTTON_COUNT + 1);
	static inline const auto RIGHT_TRIGGER_BUTTON = (SDL_GamepadButton)(SDL_GAMEPAD_BUTTON_COUNT + 2);

   private:
	SDL_Scancode _keys[BINDINGS_PER_ACTION];
	SDL_GamepadButton _buttons[BINDINGS_PER_ACTION];

	bool _tap = false;
	bool _down = false;

	int _pressedCallbackID = 0;
	int _releasedCallbackID = 0;
	int _doubleTapCallbackID = 0;

	typedef std::function<void()> Callback;
	typedef std::unordered_map<unsigned int, Callback> CallbackMap;

	CallbackMap _pressedCallbacks;
	CallbackMap _releasedCallbacks;
	CallbackMap _doubleTapCallbacks;

	Uint64 _lastPressTime = 0;

   public:
	Action(SDL_Scancode key0, SDL_Scancode key1, SDL_GamepadButton btn0, SDL_GamepadButton btn1)
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

	void SetDown(bool down, Uint64 timeStamp) {
		if (down && !_down) {
			_tap = true;

			for (const auto& callback : _pressedCallbacks) {
				callback.second();
			}

			if (timeStamp - _lastPressTime < DOUBLE_TAP_WINDOW) {
				for (const auto& callback : _doubleTapCallbacks) {
					callback.second();
				}
			}

			_lastPressTime = timeStamp;
		}

		else if (!down && _down) {
			for (const auto& callback : _releasedCallbacks) {
				callback.second();
			}
		}

		SetDownNoTap(down);
	}

	void ResetToKeyboardState(const bool* state) {
		for (int i = 0; i < BINDINGS_PER_ACTION; i++) {
			if (state[_keys[i]]) {
				SetDownNoTap(true);
				return;
			}
		}
		SetDownNoTap(false);
	}

	void ResetToControllerState(SDL_Gamepad* controller) {
		for (int i = 0; i < BINDINGS_PER_ACTION; i++) {
			if (SDL_GetGamepadButton(controller, _buttons[i])) {
				SetDownNoTap(true);
				return;
			};
		}
		SetDownNoTap(false);
	}

	void UpdateTapState() { _tap = false; }

	bool IsDown() const { return _down; }

	bool IsTapped() const { return _tap; }

	int AddPressedCallback(Callback callback) {
		_pressedCallbackID++;
		_pressedCallbacks[_pressedCallbackID] = callback;
		return _pressedCallbackID;
	}

	int AddReleasedCallback(Callback callback) {
		_releasedCallbackID++;
		_releasedCallbacks[_releasedCallbackID] = callback;
		return _releasedCallbackID;
	}

	int AddDoubleTapCallback(Callback callback) {
		_doubleTapCallbackID++;
		_doubleTapCallbacks[_doubleTapCallbackID] = callback;
		return _doubleTapCallbackID;
	}

	void RemovePressedCallback(unsigned int id) { _pressedCallbacks.erase(id); }

	void RemoveReleasedCallback(unsigned int id) { _releasedCallbacks.erase(id); }

	void RemoveDoubleTapCallback(unsigned int id) { _doubleTapCallbacks.erase(id); }
};
