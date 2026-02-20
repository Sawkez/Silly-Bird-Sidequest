#pragma once

#include <iostream>

#include <SDL.h>

#include "Vector2.hpp"
#include "Action.hpp"

enum ActionID {
        ACTION_JUMP,
        ACTION_DIVE,
        ACTION_LEFT,
        ACTION_RIGHT,
        ACTION_UP,
        ACTION_DOWN,
        ACTION_INTERACT,
        ACTION_COUNT
    };

class InputManager {

    private:
        Vector2 _dir = Vector2(0.0, 0.0);

        Action _actions[ACTION_COUNT] {
            Action(SDL_SCANCODE_SPACE, SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_A, SDL_CONTROLLER_BUTTON_INVALID), // jump
            Action(SDL_SCANCODE_LSHIFT, SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_LEFTSHOULDER, SDL_CONTROLLER_BUTTON_INVALID), // dive
            Action(SDL_SCANCODE_A, SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_DPAD_LEFT, SDL_CONTROLLER_BUTTON_INVALID), // left
            Action(SDL_SCANCODE_D, SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_DPAD_RIGHT, SDL_CONTROLLER_BUTTON_INVALID), // right
            Action(SDL_SCANCODE_W, SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_DPAD_UP, SDL_CONTROLLER_BUTTON_INVALID), // up
            Action(SDL_SCANCODE_S, SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_DPAD_DOWN, SDL_CONTROLLER_BUTTON_INVALID), // down
            Action(SDL_SCANCODE_E, SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_X, SDL_CONTROLLER_BUTTON_INVALID) // interact
        };


    public:
        InputManager() :
        _dir(Vector2(0.0, 0.0))
        { }

        bool HandleEvent(SDL_Event event) {
            switch (event.type) {
                case SDL_CONTROLLERDEVICEADDED:
                case SDL_CONTROLLERDEVICEREMOVED:
                    HandleEvent(event.cdevice);
                    return true;
                
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    HandleEvent(event.key);
                    return true;
                
                case SDL_CONTROLLERBUTTONDOWN:
                case SDL_CONTROLLERBUTTONUP:
                    HandleEvent(event.cbutton);
                    return true;
                
            }

            return false;
        }

        void HandleEvent(SDL_KeyboardEvent event) {
            
            int actionId = -1;

            for (int i = 0; i < ACTION_COUNT; i++) {
                
                if (_actions[i].HasKey(event.keysym.scancode)) {
                    actionId = i;
                    break;
                }

            }

            if (actionId < 0) return;

            _actions[actionId].SetDown(event.state == SDL_PRESSED);

        }

        void HandleEvent(SDL_ControllerButtonEvent event) {

            int actionId = -1;

            for (int i = 0; i < ACTION_COUNT; i++) {

                if (_actions[i].HasButton(event.button)) {
                    actionId = i;
                    break;
                }

            }

            if (actionId < 0) return;

            _actions[actionId].SetDown(event.state == SDL_PRESSED);

        }

        void HandleEvent(SDL_ControllerDeviceEvent event) {
            std::cout << SDL_GameControllerOpen(event.which) << std::endl;
        }

        void UpdateDir() {
            _dir.x = float(IsDown(ACTION_RIGHT)) - float(IsDown(ACTION_LEFT));
            _dir.y = float(IsDown(ACTION_DOWN)) - float(IsDown(ACTION_UP));
        }

        void UpdateTapStates() {
            for (int i = 0; i < ACTION_COUNT; i++) {
                _actions[i].UpdateTapState();
            }
        }

        bool IsDown(int id) const {
            return _actions[id].IsDown();
        }

        bool IsTapped(int id) const {
            return _actions[id].IsTapped();
        }

        Vector2 GetDir() const {
            return _dir;
        }
};