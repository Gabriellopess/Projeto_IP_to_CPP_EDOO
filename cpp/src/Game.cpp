#include "game/Game.hpp"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace game {

static constexpr int kCollectibleSize = 40;
static constexpr int kMinDistanceX = 250;
static constexpr int kSpawnMinXOffset = 200;
static constexpr int kSpawnMaxXOffset = 900;

static constexpr int kHeights[] = {565, 480, 460};

Game::Game(GameConfig cfg) : cfg_(cfg) {
    reset(42);
}

void Game::reset(std::uint32_t seed) {
    rng_.seed(seed);

    tick_ = 0;
    distancia_pixels_ = 0;
    finalizando_ = false;
    ticks_finalizando_ = 0;

    vel_coletavel_ = cfg_.vel_coletavel;

    contadores_ = Counters{};
    efeitos_ = Efeitos{};

    gisele_.resetar(Vec2{100, 500}, 525);

    coletaveis_.clear();
    spawn_initial_collectibles();
}

void Game::spawn_initial_collectibles() {
    coletaveis_.reserve(3);

    for (int i = 0; i < 3; ++i) {
        const auto type = random_choice_tipo_coletavel();

        Rect rect;
        rect.w = kCollectibleSize;
        rect.h = kCollectibleSize;

        std::vector<int> usedY;
        for (const auto& c : coletaveis_) {
            usedY.push_back(c->rect().y);
        }

        std::vector<int> heights(std::begin(kHeights), std::end(kHeights));
        std::vector<int> available;

        for (int h : heights) {
            if (std::find(usedY.begin(), usedY.end(), h) == usedY.end()) {
                available.push_back(h);
            }
        }

        rect.y = available.empty()
            ? heights[static_cast<std::size_t>(random_int(0, static_cast<int>(heights.size()) - 1))]
            : available[static_cast<std::size_t>(random_int(0, static_cast<int>(available.size()) - 1))];

        int attempts = 0;
        while (true) {
            rect.x = random_int(
                cfg_.largura_tela + kSpawnMinXOffset,
                cfg_.largura_tela + kSpawnMaxXOffset
            );

            bool ok = true;
            for (const auto& other : coletaveis_) {
                if (std::abs(rect.x - other->rect().x) < kMinDistanceX) {
                    ok = false;
                    break;
                }
            }

            if (ok || attempts++ > 10) {
                break;
            }
        }

        coletaveis_.push_back(criar_coletavel(type, rect));
    }
}

void Game::respawn_coletavel(std::size_t idx) {
    if (idx >= coletaveis_.size()) {
        throw std::out_of_range("respawn_coletavel idx");
    }

    const auto type = random_choice_tipo_coletavel();

    std::vector<int> usedY;
    for (std::size_t i = 0; i < coletaveis_.size(); ++i) {
        if (i == idx) {
            continue;
        }

        usedY.push_back(coletaveis_[i]->rect().y);
    }

    std::vector<int> heights(std::begin(kHeights), std::end(kHeights));
    std::vector<int> available;

    for (int h : heights) {
        if (std::find(usedY.begin(), usedY.end(), h) == usedY.end()) {
            available.push_back(h);
        }
    }

    Rect rect;
    rect.w = kCollectibleSize;
    rect.h = kCollectibleSize;

    rect.y = available.empty()
        ? heights[static_cast<std::size_t>(random_int(0, static_cast<int>(heights.size()) - 1))]
        : available[static_cast<std::size_t>(random_int(0, static_cast<int>(available.size()) - 1))];

    int attempts = 0;
    while (true) {
        rect.x = random_int(
            cfg_.largura_tela + kSpawnMinXOffset,
            cfg_.largura_tela + kSpawnMaxXOffset
        );

        bool ok = true;
        for (std::size_t i = 0; i < coletaveis_.size(); ++i) {
            if (i == idx) {
                continue;
            }

            if (std::abs(rect.x - coletaveis_[i]->rect().x) < kMinDistanceX) {
                ok = false;
                break;
            }
        }

        if (ok || attempts++ > 10) {
            break;
        }
    }

    coletaveis_[idx] = criar_coletavel(type, rect);
}

TipoColetavel Game::random_choice_tipo_coletavel() {
    const int r = random_int(0, 2);

    if (r == 0) {
        return TipoColetavel::Banana;
    }

    if (r == 1) {
        return TipoColetavel::Camera;
    }

    return TipoColetavel::Rosa;
}

int Game::random_int(int lo, int hi) {
    std::uniform_int_distribution<int> dist(lo, hi);
    return dist(rng_);
}

void Game::acelerar() {
    if (cfg_.acelerar_a_cada_ticks <= 0) {
        return;
    }

    if (tick_ > 0 && (tick_ % cfg_.acelerar_a_cada_ticks) == 0) {
        if (vel_coletavel_ < cfg_.vel_coletavel_max) {
            ++vel_coletavel_;
            std::cout << "[ACELERAR] vel_coletavel=" << vel_coletavel_ << "\n";
        }
    }
}

void Game::checar_finalizando() {
    const int distancia_metros = distancia_pixels_ / cfg_.pixels_por_metro;

    if (!finalizando_ && distancia_metros >= (cfg_.meta_metros - cfg_.offset_finalizacao_metros)) {
        finalizando_ = true;
        ticks_finalizando_ = 0;

        std::cout << "[FINALIZANDO] reached "
                  << distancia_metros
                  << "m, entering final walk...\n";
    }
}

void Game::checar_colisoes() {
    const Rect gisele_rect = gisele_.get_rect();

    for (std::size_t i = 0; i < coletaveis_.size(); ++i) {
        if (gisele_rect.intersects(coletaveis_[i]->rect())) {
            log_pickup(*coletaveis_[i]);
            coletaveis_[i]->efeito(efeitos_, contadores_);
            respawn_coletavel(i);
        }
    }
}

void Game::log_tick_header() const {
    const int distancia_metros = distancia_pixels_ / cfg_.pixels_por_metro;

    std::cout << "tick=" << tick_
              << " dist=" << distancia_metros << "m "
              << contadores_;

    if (efeitos_.flash_ativo()) {
        std::cout << " flash(" << efeitos_.flash_restante() << ")";
    }

    if (finalizando_) {
        std::cout << " [finalizando]";
    }

    std::cout << "\n";
}

void Game::log_pickup(const Coletavel& c) const {
    std::cout << "  pickup: " << c.nome()
              << " (" << to_string(c.tipo()) << ")\n";
}

GameResult Game::rodar_simulacao() {
    std::cout << "=== Simulation start ===\n";

    while (true) {
        ++tick_;

        if ((tick_ % 90) == 0) {
            gisele_.pular();
            std::cout << "  action: pular()\n";
        }

        gisele_.atualizar_fisica();

        distancia_pixels_ += cfg_.velocidade_cenario;

        acelerar();
        checar_finalizando();

        if (!finalizando_) {
            for (auto& c : coletaveis_) {
                c->rect().x -= vel_coletavel_;
            }
        } else {
            ++ticks_finalizando_;
        }

        if (!finalizando_) {
            checar_colisoes();
        }

        efeitos_.tick();

        if ((tick_ % 30) == 0 || finalizando_) {
            log_tick_header();
        }

        if (contadores_.banana >= 3 || contadores_.camera >= 3) {
            std::cout << "=== Result: LOSE (hazards) ===\n";
            return GameResult::Lose;
        }

        if (finalizando_ && ticks_finalizando_ >= cfg_.duracao_andar_final_ticks) {
            if (contadores_.rosa >= cfg_.rosas_minimas_para_vitoria) {
                std::cout << "=== Result: WIN (finished runway + enough roses) ===\n";
                return GameResult::Win;
            }

            std::cout << "=== Result: LOSE (few roses) ===\n";
            return GameResult::Lose;
        }

        if (tick_ > 5000) {
            throw std::runtime_error("simulation timeout");
        }

        if (!finalizando_) {
            for (std::size_t i = 0; i < coletaveis_.size(); ++i) {
                if (coletaveis_[i]->rect().x + coletaveis_[i]->rect().w < 0) {
                    respawn_coletavel(i);
                }
            }
        }
    }
}

}
