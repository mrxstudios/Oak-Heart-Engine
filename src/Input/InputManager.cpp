// InputManager.cpp

#include "InputManager.h"

InputManager::InputManager() {
    // Fills keyboardState with 512x false;
    std::fill_n(keyState, SDL_NUM_SCANCODES, false);
    std::fill_n(prevKeyState, SDL_NUM_SCANCODES, false);
    std::fill_n(keyPressedHandledState, SDL_NUM_SCANCODES, false);
    std::fill_n(keyReleasedHandledState, SDL_NUM_SCANCODES, false);
    std::fill_n(mouseButtonState, 5, false);
    std::fill_n(mouseHandledState, 5, false);

    mousePosition.x = 0;
    mousePosition.y = 0;
}

void InputManager::PollEvents() {
    // Store the previous state.
    for (size_t i = 0; i < SDL_NUM_SCANCODES; i++) {
        prevKeyState[i] = keyState[i];
    }

    // Handle SDL events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            quit = true;
            break;
        case SDL_EVENT_KEY_DOWN:
            keyState[event.key.keysym.scancode] = true;
            break;
        case SDL_EVENT_KEY_UP:
            keyState[event.key.keysym.scancode] = false;
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            mouseButtonState[event.button.button] = true;
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            mouseButtonState[event.button.button] = false;
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

bool InputManager::IsKeyDown(SDL_Scancode key) const {
    return keyState[key];
}

bool InputManager::IsKeyPressed(SDL_Scancode key) {
    if (keyState[key]) {
        if (!prevKeyState[key]) {
            if (!keyPressedHandledState[key]) {
                keyPressedHandledState[key] = true;
                return true;
            }
        }
        else {
            keyPressedHandledState[key] = false;
        }
    }
    return false;
}

bool InputManager::IsKeyReleased(SDL_Scancode key) {
    if (!keyState[key]) {
        if (prevKeyState[key]) {
            if (!keyReleasedHandledState[key]) {
                keyReleasedHandledState[key] = true;
                return true;
            }
        }
        else {
            keyReleasedHandledState[key] = false;
        }
    }
    return false;
}

bool InputManager::IsMouseButtonPressed(Uint8 button, bool handle) {
    if (mouseButtonState[button]) {
        if (mouseHandledState[button] == false) {
            if (handle) {
                mouseHandledState[button] = true;
            }
            return true;
        }
        else {
            return false; // Key is down but has already been "handled"
        }
    }
    else {
        mouseHandledState[button] = false; // Reset if the key is up
        return false;
    }
}

SDL_Point InputManager::GetMousePosition() {
    return mousePosition;
}