#pragma once

#include "game/Gisele.hpp"
#include "game/Coletavel.hpp"
#include "game/Efeitos.hpp"
#include "game/Types.hpp"

#include <memory>
#include <random>
#include <string>
#include <vector>

namespace game {

struct GameConfig {
    int largura_tela{900};
    int altura_tela{700};

    int pixels_por_metro{20};
    int meta_metros{500};

    int velocidade_cenario{7};
    int vel_coletavel{7};

    int acelerar_a_cada_ticks{300};
    int vel_coletavel_max{20};

    int offset_finalizacao_metros{50};
    int duracao_andar_final_ticks{120};

    
    int rosas_minimas_para_vitoria{6};
};

enum class GameResult { Running, Win, Lose };

class Game {
public:
    explicit Game(GameConfig cfg);

    void reset(std::uint32_t seed = 42);

    // Simulação fixa. Retorna Win/Lose
    GameResult rodar_simulacao();

private:
    void spawn_initial_collectibles();
    void respawn_coletavel(std::size_t idx);

    [[nodiscard]] TipoColetavel random_choice_tipo_coletavel();
    [[nodiscard]] int random_int(int lo, int hi);

    void acelerar();
    void checar_finalizando();

    // Colisão + aplicação de efeito
    void checar_colisoes();

    void log_tick_header() const;
    void log_pickup(const Coletavel& c) const;

private:
    GameConfig cfg_;

    std::mt19937 rng_;

    Gisele gisele_{};
    Efeitos efeitos_;
    Counters contadores_;

    std::vector<std::unique_ptr<Coletavel>> coletaveis_;

    int tick_{0};
    int distancia_pixels_{0};

    bool finalizando_{false};
    int ticks_finalizando_{0};

    int vel_coletavel_{7};
};

}
