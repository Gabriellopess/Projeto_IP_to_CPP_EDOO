#pragma once

#include "game/Types.hpp"

namespace game {

class Gisele {
public:
	Gisele();

	void resetar(Vec2 pos, int chao);
	void atualizar_fisica();
	void pular();

	[[nodiscard]] Rect get_rect() const noexcept;
	[[nodiscard]] Vec2 posicao() const noexcept { return {player_x, player_y}; }

	int player_x{120};
	int player_y{0};
	int chao{525};
	int vel_y{0};
	int gravidade{1};
	bool pulando{false};

private:
	int largura_hitbox_{50};
	int altura_hitbox_{70};
	int sprite_w_{80};
	int sprite_h_{80};
};

}
