#pragma once

#include "world.hpp"

#include <SDL.h>

#include <memory>

class Camera {
public:
    void focusAt(const Point& worldCenter);
    void setScreenSize(int width, int height);
    [[nodiscard]] SDL_FPoint project(Point worldPoint) const;
    [[nodiscard]] float pixelDistance(float worldDistance) const;

private:
    Point _center;
    Vector _pixelCenter;
    float _pixelZoom = 4;
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

    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> _octopusHeadTexture {
        nullptr, SDL_DestroyTexture};
};
