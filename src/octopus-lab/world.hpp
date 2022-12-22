#pragma once

#include <ve.hpp>

template <class T> struct XYModel {
    T x;
    T y;
};
using Point = ve::Point<XYModel, float>;
using Vector = ve::Vector<XYModel, float>;

struct Octopus {
    Point position;
    float headRadius = 0.f;
    std::vector<std::vector<Point>> legs;
};

struct World {
    void initializeTestLevel();

    void update(double delta);

    Octopus octopus;
};
