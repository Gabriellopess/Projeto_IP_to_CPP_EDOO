#pragma once

#include "game/Types.hpp"

#include <memory>
#include <string>

namespace game {

enum class TipoColetavel { Banana, Camera, Rosa };

inline std::string to_string(TipoColetavel t) {
	switch (t) {
		case TipoColetavel::Banana: return "banana";
		case TipoColetavel::Camera: return "camera";
		case TipoColetavel::Rosa: return "rosa";
	}
	return "unknown";
}

class Efeitos;

using Contadores = Counters;

class Coletavel {
public:
	Coletavel(TipoColetavel tipo, Rect rect);
	virtual ~Coletavel() = default;

	Coletavel(const Coletavel&) = delete;
	Coletavel& operator=(const Coletavel&) = delete;
	Coletavel(Coletavel&&) noexcept = default;
	Coletavel& operator=(Coletavel&&) noexcept = default;

	[[nodiscard]] TipoColetavel tipo() const noexcept { return tipo_; }
	[[nodiscard]] const Rect& rect() const noexcept { return rect_; }
	[[nodiscard]] Rect& rect() noexcept { return rect_; }

	virtual void efeito(Efeitos& efeitos, Contadores& contadores) const = 0;
	[[nodiscard]] virtual std::string nome() const = 0;

protected:
	TipoColetavel tipo_;
	Rect rect_;
};

class Banana final : public Coletavel {
public:
	explicit Banana(Rect rect);
	void efeito(Efeitos& efeitos, Contadores& contadores) const override;
	[[nodiscard]] std::string nome() const override { return "Banana"; }
};

class Camera final : public Coletavel {
public:
	explicit Camera(Rect rect);
	void efeito(Efeitos& efeitos, Contadores& contadores) const override;
	[[nodiscard]] std::string nome() const override { return "Camera"; }
};

class Rosa final : public Coletavel {
public:
	explicit Rosa(Rect rect);
	void efeito(Efeitos& efeitos, Contadores& contadores) const override;
	[[nodiscard]] std::string nome() const override { return "Rosa"; }
};

std::unique_ptr<Coletavel> criar_coletavel(TipoColetavel tipo, Rect rect);

}
