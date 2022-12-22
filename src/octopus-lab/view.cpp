#include "view.hpp"

#include "sdl.hpp"

#include <cmath>

namespace {

constexpr int pixelsPerUnit = 32;

} // namespace

void Camera::focusAt(const Point& worldCenter)
{
    _center = worldCenter;
}

void Camera::setScreenSize(int width, int height)
{
    _pixelCenter = ve::Vector<XYModel, int>{width, height} / 2.f;
}

SDL_FPoint Camera::project(Point worldPoint) const
{
    auto pixelPosition =
        _pixelCenter + (worldPoint - _center) * pixelsPerUnit;
    return {pixelPosition.x, pixelPosition.y};
}

View::View()
{
    static constexpr int screenWidth = 1024;
    static constexpr int screenHeight = 768;

    _window = sdlCheck(SDL_CreateWindow(
        "Mr. Octopus Lab",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        screenWidth,
        screenHeight,
        0));
    _renderer = sdlCheck(SDL_CreateRenderer(
        _window,
        -1,
        SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED));
    _camera.focusAt({0, 0});
    _camera.setScreenSize(screenWidth, screenHeight);
}

bool View::processInput() const
{
    for (auto e = SDL_Event{}; SDL_PollEvent(&e); ) {
        if (e.type == SDL_QUIT) {
            return false;
        }
    }
    return true;
}

void View::present(const World& world) const
{
    sdlCheck(SDL_SetRenderDrawColor(_renderer, 50, 50, 50, 255));
    sdlCheck(SDL_RenderClear(_renderer));

    auto octopusScreenPosition = _camera.project(world.octopus.position);

    static const float sq32 = static_cast<float>(std::sqrt(3)) / 2.f;

    static const std::vector<Vector> circlePositions {
        {1, 0}, {sq32, 0.5f}, {0.5f, sq32}, {0, 1}, {-0.5f, sq32},
        {-sq32, 0.5f}, {-1, 0}, {-sq32, -0.5f}, {-0.5, -sq32}, {0, -1},
        {0.5, -sq32}, {sq32, -0.5},
    };

    std::vector<SDL_Vertex> vertices;
    vertices.push_back({
        .position = octopusScreenPosition,
        .color = SDL_Color{200, 200, 200, 255}
    });
    for (const auto& circlePosition : circlePositions) {
        vertices.push_back({
            .position = SDL_FPoint{
                circlePosition.x * pixelsPerUnit + octopusScreenPosition.x,
                circlePosition.y * pixelsPerUnit + octopusScreenPosition.y
            },
            .color = SDL_Color{200, 200, 200, 255}
        });
    }
    std::vector<int> indices;
    for (size_t i = 1; i < vertices.size(); i++) {
        size_t j = (i + 1 < vertices.size() ? i + 1 : 1);
        indices.push_back(0);
        indices.push_back(static_cast<int>(i));
        indices.push_back(static_cast<int>(j));
    }

    sdlCheck(SDL_RenderGeometry(
        _renderer,
        nullptr,
        vertices.data(),
        static_cast<int>(vertices.size()),
        indices.data(),
        static_cast<int>(indices.size())));

    SDL_RenderPresent(_renderer);
}
