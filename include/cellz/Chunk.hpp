#pragma once

#include <vector>
#include <unordered_map>

#include <cellz/Neighbors.hpp>
#include <cellz/Utils.hpp>
#include <SFML/Graphics.hpp>

namespace cellz {

class Chunk {
public:

    Chunk(state_t const _default_state, std::size_t const _width, std::size_t const _height);

    state_t get(Position const& position) const;
    void set(Position const& position, state_t const state);
    template<typename It>
    void set_row(int const row, It iterator);

    unsigned updated_cell_count() const;

    std::size_t get_width() const;
    std::size_t get_height() const;

    state_t get_default_state() const;

    Neighbors get_neighbors(Position const& position) const;

    void sync_hidden_north_border(Chunk const& from_chunk);
    void sync_hidden_south_border(Chunk const& from_chunk);
    void sync_hidden_east_border(Chunk const& from_chunk);
    void sync_hidden_west_border(Chunk const& from_chunk);

    void sync_hidden_north_west_corner(Chunk const& from_chunk);
    void sync_hidden_south_west_corner(Chunk const& from_chunk);
    void sync_hidden_south_east_corner(Chunk const& from_chunk);
    void sync_hidden_north_east_corner(Chunk const& from_chunk);

    void to_texture(sf::Texture& texture, std::unordered_map<state_t, sf::Color> const& colors) const;

private:

    std::size_t from_coords(Position const& position) const;

    std::size_t real_width() const;
    std::size_t real_height() const;

    state_t const default_state;
    const std::size_t width, height;
    std::vector<state_t> states;
    unsigned updated;

};




template<typename It>
void Chunk::set_row(int const row, It iterator) {
    std::size_t index = from_coords({0, row}); 
    //std::cout << "R[" << row << "] " << updated << " => ";
    updated -= width - std::count(std::begin(states) + index, std::begin(states) + index + width, default_state);

    std::copy_n(iterator, width, std::begin(states) + index);

    updated += width - std::count(std::begin(states) + index, std::begin(states) + index + width, default_state);
    //std::cout << updated << "\n";
}


}