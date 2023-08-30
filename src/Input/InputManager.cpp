// InputManager.cpp

#include "InputManager.h"

InputManager::InputManager() {
    mousePosition.x = 0;
    mousePosition.y = 0;
}

void InputManager::PollEvents() {
    // Store the previous state.
    for (size_t i = 0; i < SDL_NUM_SCANCODES; i++) {
        keyStates[i].prevState = keyStates[i].state;
    }

    // Handle SDL events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            quit = true;
            break;
        case SDL_EVENT_KEY_DOWN:
            keyStates[event.key.keysym.scancode].state = true;
            break;
        case SDL_EVENT_KEY_UP:
            keyStates[event.key.keysym.scancode].state = false;
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            mouseStates[event.button.button].state = true;
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            mouseStates[event.button.button].state = false;
            break;
        case SDL_EVENT_MOUSE_MOTION:
            mousePosition.x = event.motion.x;
            mousePosition.y = event.motion.y;
            break;
        default:
            break;
        }
    }
}

bool InputManager::IsQuitRequested() const{
    return quit;
}

bool InputManager::IsDown(inputState* inputStates, int key) {
    return inputStates[key].state;
}

bool InputManager::IsUp(inputState* inputStates, int key) {
    return !inputStates[key].state;
}

bool InputManager::IsPressed(inputState* inputStates, int key) {
    if (inputStates[key].state) {
        if (!inputStates[key].prevState) {
            if (!inputStates[key].pressedHandled) {
                inputStates[key].pressedHandled = true;
                return true;
            }
        }
        else {
            inputStates[key].pressedHandled = false;
        }
    }
    return false;
}

bool InputManager::IsReleased(inputState* inputStates, int key) {
    if (!inputStates[key].state) {
        if (inputStates[key].prevState) {
            if (!inputStates[key].releasedHandled) {
                inputStates[key].releasedHandled = true;
                return true;
            }
        }
        else {
            inputStates[key].releasedHandled = false;
        }
    }
    return false;
}

SDL_Point InputManager::GetMousePosition() {
    return mousePosition;
}