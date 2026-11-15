#include "game/Game.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>

/// @file main.cpp
/// @brief Entry point. The game must be run from the project folder, because
///        the assets are loaded with relative paths.
int main() {
    try {
        Game game;
        game.run();
    } catch (const std::exception &error) {
        std::cerr << "Error: " << error.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
