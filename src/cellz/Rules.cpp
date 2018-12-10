#include <cellz/Rules.hpp>

#include <cassert>

namespace cellz {

Rules::Rules(std::size_t const state_count) : rules(state_count, Rules::default_rule) {}

Rules& Rules::on(state_t const state, rule_t const rule) {
    rules[state] = rule;
    return *this;
}

rule_t Rules::get(state_t const state) const {
    assert(state <= rules.size());
    return rules[state];
}

state_t Rules::default_rule(Neighbors const& neighbors) {
    return neighbors.there();
}

}