#include "world.hpp"

void World::initializeTestLevel()
{
    octopus = Octopus{
        .headRadius = 0.5f,
        .legs = {
            {{-1, -1}, {-1.5, -2}},
            {{1, -1}, {1.5, -2}},
            {{0.5, 1}, {1.5, 0.5}, {2, 0}},
        },
    };
}

void World::update(double delta)
{
    (void)delta;
}
