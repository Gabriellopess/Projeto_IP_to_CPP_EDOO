#include "game/Game.hpp"

#include <iostream>

int main() {
    using namespace game;

    GameConfig cfg;
    cfg.meta_metros = 500;
    cfg.pixels_por_metro = 20;
    cfg.acelerar_a_cada_ticks = 300;

    std::cout << "\n=== CENARIO 1: vitoria por terminar a passarela com rosas suficientes ===\n";
    {
        Game game{cfg};
        game.reset(178);

        const GameResult result = game.rodar_simulacao();

        std::cout << "Simulation ended with: "
                  << (result == GameResult::Win ? "WIN" : result == GameResult::Lose ? "LOSE" : "RUNNING")
                  << "\n";
    }

    std::cout << "\n=== CENARIO 2: derrota por acumular sabotagens ===\n";
    {
        Game game{cfg};
        game.reset(12345);

        const GameResult result = game.rodar_simulacao();

        std::cout << "Simulation ended with: "
                  << (result == GameResult::Win ? "WIN" : result == GameResult::Lose ? "LOSE" : "RUNNING")
                  << "\n";
    }

    return 0;
}
