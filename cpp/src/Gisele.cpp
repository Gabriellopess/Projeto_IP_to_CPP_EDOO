#include "game/Gisele.hpp"

namespace game {

Gisele::Gisele() = default;

void Gisele::resetar(Vec2 pos, int novo_chao) {
    player_x = pos.x;
    player_y = pos.y;
    chao = novo_chao;
    vel_y = 0;
    pulando = false;
}

void Gisele::atualizar_fisica() {
    vel_y += gravidade;
    player_y += vel_y;

    if (player_y >= chao) {
        player_y = chao;
        vel_y = 0;
        pulando = false;
    }
}

void Gisele::pular() {
    if (!pulando) {
        vel_y = -14;
        pulando = true;
    }
}

Rect Gisele::get_rect() const noexcept {
    const int ajuste_x = (sprite_w_ - largura_hitbox_) / 2;
    const int ajuste_y = (sprite_h_ - altura_hitbox_);
    return Rect{player_x + ajuste_x, player_y + ajuste_y, largura_hitbox_, altura_hitbox_};
}

}
