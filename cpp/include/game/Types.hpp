#pragma once

#include <cstddef>
#include <ostream>

namespace game {

struct Rect {
    int x{};
    int y{};
    int w{};
    int h{};

    [[nodiscard]] constexpr bool intersects(const Rect& other) const noexcept {
        return !(x + w <= other.x || other.x + other.w <= x || y + h <= other.y || other.y + other.h <= y);
    }
};

struct Vec2 {
    int x{};
    int y{};
};

struct Counters {
    int banana{0};
    int camera{0};
    int rosa{0};
};

inline std::ostream& operator<<(std::ostream& os, const Counters& c) {
    os << "Counters{banana=" << c.banana << ", camera=" << c.camera << ", rosa=" << c.rosa << "}";
    return os;
}

}
