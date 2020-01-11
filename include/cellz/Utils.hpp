#pragma once

#include <optional>
#include <cstddef>
#include <iostream>

namespace cellz {

struct Position {
    int x, y;
};

static inline bool operator == (Position const& lhs, Position const& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

static inline std::ostream& operator << (std::ostream& os, Position const& position) {
    return os << "(" << position.x << ", " << position.y << ")";
}

}

namespace std {
    template<> struct hash<::cellz::Position> {
        inline size_t operator()(::cellz::Position const& position) const noexcept {
            auto const x = hash<decltype(position.x)>{}(position.x);
            auto const y = hash<decltype(position.y)>{}(position.y);
            return x ^ (y + 0x9e3779b9 + (x << 13) + (x >> 5));
        }
    };
}

namespace cellz {


struct WorldPosition {
    Position chunk;
    Position in_chunk;
};

static inline std::ostream& operator << (std::ostream& os, WorldPosition const& position) {
    return os << position.chunk << "@" << position.in_chunk;
}




using state_t = unsigned;




class Neighbors;
using rule_t = state_t(*)(Neighbors const&);





class Option {
public:

    constexpr Option() : horizontal_count(std::nullopt), vertical_count{std::nullopt}, hori_wrap{false}, vert_wrap{false} {}
    
    constexpr void horizontal_wrap(std::size_t const _horizontal_count) { hori_wrap = true; this->horizontal_count = _horizontal_count; }
    constexpr void vertical_wrap(std::size_t const _vertical_count) { vert_wrap = true; this->vertical_count = _vertical_count; }

    constexpr void finite_horizontal_count(std::size_t const _horizontal_count) { this->horizontal_count = _horizontal_count; }
    constexpr void finite_vertical_count(std::size_t const _vertical_count) { this->vertical_count = _vertical_count; }

    constexpr std::optional<std::size_t> get_horizontal_count() const { return horizontal_count; }
    constexpr std::optional<std::size_t> get_vertical_count() const { return vertical_count; }

    constexpr bool do_horizontal_wrap() const { return hori_wrap; }
    constexpr bool do_vertical_wrap() const { return vert_wrap; }

    constexpr bool is_horizontal_finite() const { return static_cast<bool>(horizontal_count); }
    constexpr bool is_vertical_finite() const { return static_cast<bool>(vertical_count); }

private:

    std::optional<std::size_t> horizontal_count, vertical_count;
    bool hori_wrap, vert_wrap;

};


}