#pragma once

#include "error.hpp"

#include <SDL.h>

inline void sdlCheck(
    bool condition,
    Error::SourceLocation sourceLocation = Error::SourceLocation::current())
{
    if (!condition) {
        throw Error{"", sourceLocation} << SDL_GetError();
    }
}

inline void sdlCheck(
    int code,
    Error::SourceLocation sourceLocation = Error::SourceLocation::current())
{
    sdlCheck(code == 0, sourceLocation);
}

template <class T>
T* sdlCheck(
    T* ptr,
    Error::SourceLocation sourceLocation = Error::SourceLocation::current())
{
    if (!ptr) {
        throw Error{"", sourceLocation} << SDL_GetError();
    }
    return ptr;
}
