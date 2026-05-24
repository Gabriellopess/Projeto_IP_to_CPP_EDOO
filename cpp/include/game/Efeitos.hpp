#pragma once

namespace game {

// Modelo mínimo só da "lógica" do flash e de eventos.
class Efeitos {
public:
	void iniciar_flash(int duracao_ticks = 30) {
		flash_restante_ = duracao_ticks;
	}

	void tick() {
		if (flash_restante_ > 0) {
			--flash_restante_;
		}
	}

	[[nodiscard]] bool flash_ativo() const noexcept { return flash_restante_ > 0; }
	[[nodiscard]] int flash_restante() const noexcept { return flash_restante_; }

private:
	int flash_restante_{0};
};

}
