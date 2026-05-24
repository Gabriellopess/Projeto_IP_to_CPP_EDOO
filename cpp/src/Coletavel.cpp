#include "game/Coletavel.hpp"
#include "game/Efeitos.hpp"

#include <stdexcept>

namespace game {

Coletavel::Coletavel(TipoColetavel tipo, Rect rect) : tipo_(tipo), rect_(rect) {}

Banana::Banana(Rect rect) : Coletavel(TipoColetavel::Banana, rect) {}
Camera::Camera(Rect rect) : Coletavel(TipoColetavel::Camera, rect) {}
Rosa::Rosa(Rect rect) : Coletavel(TipoColetavel::Rosa, rect) {}

void Banana::efeito(Efeitos&, Contadores& contadores) const {
    contadores.rosa = 0;
    ++contadores.banana;
}

void Camera::efeito(Efeitos& efeitos, Contadores& contadores) const {
    ++contadores.camera;
    efeitos.iniciar_flash();
}

void Rosa::efeito(Efeitos&, Contadores& contadores) const {
    ++contadores.rosa;
}

std::unique_ptr<Coletavel> criar_coletavel(TipoColetavel tipo, Rect rect) {
    switch (tipo) {
        case TipoColetavel::Banana: return std::make_unique<Banana>(rect);
        case TipoColetavel::Camera: return std::make_unique<Camera>(rect);
        case TipoColetavel::Rosa: return std::make_unique<Rosa>(rect);
    }
    throw std::invalid_argument("unknown TipoColetavel");
}

}
