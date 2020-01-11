#pragma once

#include <unordered_map>
#include <utility>
#include <SFML/Graphics.hpp>

#include <cellz/Chunk.hpp>
#include <cellz/Rules.hpp>
#include <cellz/Utils.hpp>

namespace cellz {

class World {
public:


    World(state_t const _default_state, std::size_t const _chunk_size, Option const& _option = {});

    void update(Rules const& rules, unsigned const additional_thread = 0);

    state_t get(Position const& position) const;
    void set(Position const& position, state_t const state);

    void to_texture(Position const& chunk_position, sf::Texture& texture, std::unordered_map<state_t, sf::Color> const& colors) const;

private:

    void set_and_check_empty(WorldPosition const& world_position, state_t const state);

    Position wrap_chunk_position(Position const& position) const;
    WorldPosition to_world_position(Position const& position) const;
    WorldPosition get_chunk_coords(Position const& position) const;
    WorldPosition get_wrapped_chunk_coords(Position position) const;

    const state_t default_state;
    const std::size_t chunk_size;
    Option option;
    std::unordered_map<Position, Chunk> chunks;

};

}