#include "InputManager.h"
#include <SDL2/SDL.h>

InputManager::InputManager() = default;

InputManager::~InputManager() = default;

bool InputManager::isKeyPressed(int scancode)
{
    auto* states = SDL_GetKeyboardState(nullptr);
    return states[scancode] == 1;
}
