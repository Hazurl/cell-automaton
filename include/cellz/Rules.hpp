#pragma once

#include <vector>
#include <cellz/Neighbors.hpp>
#include <cellz/Utils.hpp>

namespace cellz {

/*

    auto const game_of_life = Rules{}
        .on(dead_cell,  [] (Neighbors const& neighbors) { return neighbors.count(alive_cell) == 3 ? alive_cell : dead_cell; })
        .on(alive_cell, [] (Neighbors const& neighbors) { auto const count = neighbors.count(alive_cell); return count == 2 || count == 3 ? alive_cell : dead_cell; });

    auto cont rule_113 = Rules{}
        .on(black, [] (Neighbors const& neighbors) { return black; })
        .on(white, [] (Neighbors const& neighbors) { return neighbors.north_west() && !neighbors.north() || 
                                                            !neighbors.north() && !neighbors.north_east() ||
                                                            neighbors.north_west() && !neighbors.north_east(); });

*/

class Rules {
public:

    Rules(std::size_t const state_count);
    Rules& on(state_t const state, rule_t const rule);
    rule_t get(state_t const state) const;

private:

    static state_t default_rule(Neighbors const& neighbors);

    std::vector<rule_t> rules;

};

}