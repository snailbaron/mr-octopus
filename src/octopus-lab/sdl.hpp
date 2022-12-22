#pragma once

#include "error.hpp"

#include <SDL.h>

inline void sdlCheck(int code)
{
    if (code != 0) {
        throw Error{} << SDL_GetError();
    }
}

template <class T>
T* sdlCheck(T* ptr)
{
    if (!ptr) {
        throw Error{} << SDL_GetError();
    }
    return ptr;
}
