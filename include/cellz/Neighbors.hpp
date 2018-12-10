#pragma once

#include <array>

#include <cellz/Utils.hpp>

namespace cellz {


class Neighbors {
public:

    Neighbors(std::array<std::array<state_t, 3>, 3> const& neighbors);

    state_t north_west() const;
    state_t north() const;
    state_t north_east() const;

    state_t west() const;
    state_t there() const;
    state_t east() const;

    state_t south_west() const;
    state_t south() const;
    state_t south_east() const;

    unsigned count(state_t const state) const;
    bool all(state_t const state) const;
    bool any(state_t const state) const;
    bool none(state_t const state) const;

private:

    std::array<std::array<state_t, 3>, 3> neighbors;

};


}