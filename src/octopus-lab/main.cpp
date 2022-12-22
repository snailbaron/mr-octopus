#include "error.hpp"
#include "sdl.hpp"
#include "view.hpp"
#include "world.hpp"

#include <tempo.hpp>

#include <iostream>

int main()
{
    try {
        sdlCheck(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS));

        auto world = World{};
        world.initializeTestLevel();
        auto view = View{};

        auto frameTimer = tempo::FrameTimer{60};
        for (;;) {
            if (!view.processInput()) {
                break;
            }

            if (int framesPassed = frameTimer(); framesPassed > 0) {
                for (int i = 0; i < framesPassed; i++) {
                    world.update(frameTimer.delta());
                }
                view.present(world);
            }
        }

        SDL_Quit();
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}
