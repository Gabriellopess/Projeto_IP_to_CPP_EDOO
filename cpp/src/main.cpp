#include "game/Game.hpp"

#include <iostream>

int main() {
    using namespace game;

    GameConfig cfg;
    cfg.meta_metros = 500;        
    cfg.pixels_por_metro = 20;
    cfg.acelerar_a_cada_ticks = 300;

    Game game{cfg};

    // Simulação fixa
    game.reset(12345);

    const GameResult result = game.rodar_simulacao();

    std::cout << "Simulation ended with: "
              << (result == GameResult::Win ? "WIN" : result == GameResult::Lose ? "LOSE" : "RUNNING")
              << "\n";

    return 0;
}
