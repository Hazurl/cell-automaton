#include <cellz/Neighbors.hpp>

namespace cellz {

Neighbors::Neighbors(std::array<std::array<state_t, 3>, 3> const& neighbors) : neighbors(neighbors) {}

state_t Neighbors::north_west() const { return neighbors[0][0]; }
state_t Neighbors::north() const { return neighbors[0][1]; }
state_t Neighbors::north_east() const { return neighbors[0][2]; }

state_t Neighbors::west() const { return neighbors[1][0]; }
state_t Neighbors::there() const { return neighbors[1][1]; }
state_t Neighbors::east() const { return neighbors[1][2]; }

state_t Neighbors::south_west() const { return neighbors[2][0]; }
state_t Neighbors::south() const { return neighbors[2][1]; }
state_t Neighbors::south_east() const { return neighbors[2][2]; }

unsigned Neighbors::count(state_t const state_checked) const {
    unsigned c{0};
    for(auto const& row : neighbors) 
        for(auto const state : row)
            if (state == state_checked)
                c++;

    return c - (there() == state_checked ? 1 : 0);
}

bool Neighbors::all(state_t const state) const {
    return count(state) == 9;
}

bool Neighbors::any(state_t const state) const {
    return count(state) > 0;
}

bool Neighbors::none(state_t const state) const {
    return count(state) == 0;
}


}