#include "view.hpp"

#include "sdl.hpp"

#include <build-info.hpp>

#include <SDL_image.h>

#include <cmath>

namespace {

constexpr int pixelsPerUnit = 16;

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
    return {
        _pixelCenter.x + (worldPoint.x - _center.x) * pixelsPerUnit * _pixelZoom,
        _pixelCenter.y + (_center.y - worldPoint.y) * pixelsPerUnit * _pixelZoom
    };
}

float Camera::pixelDistance(float worldDistance) const
{
    return worldDistance * pixelsPerUnit * _pixelZoom;
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

    _octopusHeadTexture.reset(sdlCheck(IMG_LoadTexture(
        _renderer, bi::sourcePath("assets/img/octopus-head.png").c_str())));
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
    auto octopusHeadPixelRadius =
        _camera.pixelDistance(world.octopus.headRadius);

    SDL_FRect octopusHeadRect {
        .x = octopusScreenPosition.x - octopusHeadPixelRadius,
        .y = octopusScreenPosition.y - octopusHeadPixelRadius,
        .w = 2 * octopusHeadPixelRadius,
        .h = 2 * octopusHeadPixelRadius,
    };
    sdlCheck(SDL_RenderCopyF(
        _renderer, _octopusHeadTexture.get(), nullptr, &octopusHeadRect));

    static const float sq32 = static_cast<float>(std::sqrt(3)) / 2.f;

    static const std::vector<Vector> circlePositions {
        {1, 0}, {sq32, 0.5f}, {0.5f, sq32}, {0, 1}, {-0.5f, sq32},
        {-sq32, 0.5f}, {-1, 0}, {-sq32, -0.5f}, {-0.5, -sq32}, {0, -1},
        {0.5, -sq32}, {sq32, -0.5},
    };

    std::vector<SDL_Vertex> vertices;
    vertices.push_back({
        .position = octopusScreenPosition,
        .color = SDL_Color{200, 200, 200, 0}
    });
    for (const auto& circlePosition : circlePositions) {
        vertices.push_back({
            .position = _camera.project(world.octopus.position +
                world.octopus.headRadius * circlePosition),
            .color = SDL_Color{200, 200, 200, 0}
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

    sdlCheck(SDL_SetRenderDrawColor(_renderer, 200, 50, 50, 255));

    static constexpr float legJointMarkRadius = 3.f;
    std::vector<SDL_FRect> allLegJoints;
    for (const auto& leg : world.octopus.legs) {
        std::vector<SDL_FPoint> points;
        points.reserve(1 + leg.size());
        points.push_back(_camera.project(world.octopus.position));
        for (const auto& legPoint : leg) {
            auto cameraLegJoint =
                _camera.project(world.octopus.position + legPoint);
            points.push_back(cameraLegJoint);
            allLegJoints.push_back(SDL_FRect{
                .x = cameraLegJoint.x - legJointMarkRadius,
                .y = cameraLegJoint.y - legJointMarkRadius,
                .w = 2 * legJointMarkRadius,
                .h = 2 * legJointMarkRadius
            });
        }
        sdlCheck(SDL_RenderDrawLinesF(
            _renderer, points.data(), static_cast<int>(points.size())));
    }
    sdlCheck(SDL_RenderFillRectsF(
        _renderer, allLegJoints.data(), static_cast<int>(allLegJoints.size())));

    SDL_RenderPresent(_renderer);
}
