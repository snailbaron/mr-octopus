#pragma once

#include "world.hpp"

#include <SDL.h>

class Camera {
public:
    void focusAt(const Point& worldCenter);
    void setScreenSize(int width, int height);
    SDL_FPoint project(Point worldPoint) const;

private:
    Point _center;
    Vector _pixelCenter;
};

class View {
public:
    View();

    bool processInput() const;
    void present(const World& world) const;

private:
    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
    Camera _camera;
};