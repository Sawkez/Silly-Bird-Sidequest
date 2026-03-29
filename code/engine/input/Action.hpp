#pragma once

#include <algorithm>

class Action {
    public:
        static const int BINDINGS_PER_ACTION = 2;

    private:
        int _keys[BINDINGS_PER_ACTION];
        int _buttons[BINDINGS_PER_ACTION];

        bool _tap = false;
        bool _down = false;
    
    public:
        Action(int key0, int key1, int btn0, int btn1) :
        _keys{key0, key1}, _buttons{btn0, btn1}
        { }

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

        void SetDown(bool down) {
            if (down && !_down) _tap = true;
            _down = down;
        }

        void UpdateTapState() {
            _tap = false;
        }

        bool IsDown() const {
            return _down;
        }

        bool IsTapped() const {
            return _tap;
        }
};

