#include <cellz/Chunk.hpp>

#include <algorithm>
#include <cassert>

namespace cellz {

Chunk::Chunk(state_t const _default_state, std::size_t const _width, std::size_t const _height) 
    : default_state{_default_state}, width{_width}, height{_height}, states((_width + 2) * (_height + 2), _default_state), updated{0} {}





state_t Chunk::get(Position const& position) const {
    return states[from_coords(position)];
}

void Chunk::set(Position const& position, state_t const state) {
    auto const index = from_coords(position);
    if (state == default_state && states[index] != default_state) {
        --updated;
    } else if (state != default_state && states[index] == default_state) {
        ++updated;
    }

    states[index] = state;
}




unsigned Chunk::updated_cell_count() const {
    return updated;
}




state_t Chunk::get_default_state() const {
    return default_state;
}



std::size_t Chunk::from_coords(Position const& position) const {
    return (position.y + 1) * real_width() + position.x + 1;
}





std::size_t Chunk::get_width() const {
    return width;
}

std::size_t Chunk::get_height() const {
    return height;
}





std::size_t Chunk::real_width() const {
    return width + 2;
}

std::size_t Chunk::real_height() const {
    return height + 2;
}





Neighbors Chunk::get_neighbors(Position const& position) const {
    std::size_t const row0 = (position.y) * (width + 2);
    std::size_t const row1 = row0 + (width + 2);
    std::size_t const row2 = row1 + (width + 2);

    return Neighbors { std::array<std::array<state_t, 3>, 3> {
        std::array<state_t, 3> { states[row0 + position.x], states[row0 + position.x + 1], states[row0 + position.x + 2] },
        std::array<state_t, 3> { states[row1 + position.x], states[row1 + position.x + 1], states[row1 + position.x + 2] },
        std::array<state_t, 3> { states[row2 + position.x], states[row2 + position.x + 1], states[row2 + position.x + 2] }
    }};
}





void Chunk::sync_hidden_north_border(Chunk const& from_chunk) {
    std::size_t const to = 1;
    std::size_t const from = real_width() * (real_height() - 2) + 1;
    updated -= width - std::count(std::begin(states) + to, std::begin(states) + to + width, default_state);
    std::copy_n(std::begin(from_chunk.states) + from, width, std::begin(states) + to);
    updated += width - std::count(std::begin(states) + to, std::begin(states) + to + width, default_state);
}

void Chunk::sync_hidden_south_border(Chunk const& from_chunk) {
    std::size_t const to = real_width() * (real_height() - 1) + 1;
    std::size_t const from = real_width() + 1;
    updated -= width - std::count(std::begin(states) + to, std::begin(states) + to + width, default_state);
    std::copy_n(std::begin(from_chunk.states) + from, width, std::begin(states) + to);
    updated += width - std::count(std::begin(states) + to, std::begin(states) + to + width, default_state);
}

void Chunk::sync_hidden_east_border(Chunk const& from_chunk) {
    std::size_t to = real_width() * 2 - 1;
    std::size_t const step = real_width();

    std::size_t from = real_width() + 1;
    for(; to < (real_height() - 1) * step; (to += step), (from += step)) {
        if (states[to] != default_state && from_chunk.states[from] == default_state) {
            --updated;
        } else if (states[to] == default_state && from_chunk.states[from] != default_state) {
            ++updated;
        }
        states[to] = from_chunk.states[from];
    }
}

void Chunk::sync_hidden_west_border(Chunk const& from_chunk) {
    std::size_t to = real_width();
    std::size_t const step = real_width();

    std::size_t from = real_width() * 2 - 2;
    for(; to < (real_height() - 1) * step; (to += step), (from += step)) {
        if (states[to] != default_state && from_chunk.states[from] == default_state) {
            --updated;
        } else if (states[to] == default_state && from_chunk.states[from] != default_state) {
            ++updated;
        }
        states[to] = from_chunk.states[from];
    }
}



void Chunk::sync_hidden_south_west_corner(Chunk const& from_chunk) {
    std::size_t const to = real_width() * (real_height() - 1);

    std::size_t const from =  2 * real_width() - 2;

    if (states[to] != default_state && from_chunk.states[from] == default_state) {
        --updated;
    } else if (states[to] == default_state && from_chunk.states[from] != default_state) {
        ++updated;
    }

    states[to] = from_chunk.states[from];
}

void Chunk::sync_hidden_north_west_corner(Chunk const& from_chunk) {
    std::size_t const to = 0;

    std::size_t const from = real_width() * (real_height() - 1) - 2;

    if (states[to] != default_state && from_chunk.states[from] == default_state) {
        --updated;
    } else if (states[to] == default_state && from_chunk.states[from] != default_state) {
        ++updated;
    }

    states[to] = from_chunk.states[from];
}

void Chunk::sync_hidden_north_east_corner(Chunk const& from_chunk) {
    std::size_t const to = real_width() - 1;

    std::size_t const from = real_width() * (real_height() - 2) + 1;
    
    if (states[to] != default_state && from_chunk.states[from] == default_state) {
        --updated;
    } else if (states[to] == default_state && from_chunk.states[from] != default_state) {
        ++updated;
    }

    states[to] = from_chunk.states[from];
}

void Chunk::sync_hidden_south_east_corner(Chunk const& from_chunk) {
    std::size_t const to = real_width() * real_height() - 1;
    std::size_t const from = real_width() + 1;

    if (states[to] != default_state && from_chunk.states[from] == default_state) {
        --updated;
    } else if (states[to] == default_state && from_chunk.states[from] != default_state) {
        ++updated;
    }

    states[to] = from_chunk.states[from];
}







void Chunk::to_texture(sf::Texture& texture, std::unordered_map<state_t, sf::Color> const& colors) const {
    std::vector<sf::Uint8> pixels( 4 * width * height );

    std::size_t index{ width + 2 + 1 };
    std::size_t x{0};
    for(std::size_t i{0}; i < pixels.size(); i += 4) {
        auto const color_iter = colors.find(states[index]);
        sf::Color color = color_iter == std::end(colors) ? sf::Color{} : color_iter->second;

        ++x;
        ++index;

        if (x >= width) {
            x = 0;
            index += 2;
        }

        pixels[i + 0] = color.r;
        pixels[i + 1] = color.g;
        pixels[i + 2] = color.b;
        pixels[i + 3] = color.a;
    }
    texture.update(pixels.data());
}


}