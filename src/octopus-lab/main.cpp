#include "error.hpp"

#include <SDL.h>

#include <iostream>

int main()
{
    try {
        throw Error{} << "some message" << " and " << 123;
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}
