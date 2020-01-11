#include <cellz/World.hpp>

#include <thread>
#include <mutex>
#include <algorithm>

namespace cellz {

World::World(state_t const _default_state, std::size_t const _chunk_size, Option const& _option)
    : default_state{_default_state}, chunk_size{_chunk_size}, option{_option} {}




state_t World::get(Position const& position) const {
    auto const world_position = get_wrapped_chunk_coords(position);
    auto const chunk = chunks.find(world_position.chunk);

    if (chunk != std::end(chunks)) {
        return chunk->second.get(world_position.in_chunk);
    }

    return default_state;
}

void World::set(Position const& position, state_t const state) {
    auto const world_position = to_world_position(position);

    std::cerr << position << " => " << world_position << '\n';

    set_and_check_empty(world_position, state);

    if (world_position.in_chunk.y == 0) {
        auto const chunk = wrap_chunk_position({ world_position.chunk.x, world_position.chunk.y - 1 });
        auto const cell = Position { world_position.in_chunk.x, static_cast<int>(chunk_size) };

        set_and_check_empty({ chunk, cell }, state);
    } else if (world_position.in_chunk.y == static_cast<int>(chunk_size) - 1) {
        auto const chunk = wrap_chunk_position({ world_position.chunk.x, world_position.chunk.y + 1 });
        auto const cell = Position { world_position.in_chunk.x, -1 };

        set_and_check_empty({ chunk, cell }, state);
    }


    if (world_position.in_chunk.x == 0) {
        auto const chunk = wrap_chunk_position({ world_position.chunk.x - 1, world_position.chunk.y });
        auto const cell = Position { static_cast<int>(chunk_size), world_position.in_chunk.y };

        set_and_check_empty({ chunk, cell }, state);
    } else if (world_position.in_chunk.x == static_cast<int>(chunk_size) - 1) {
        auto const chunk = wrap_chunk_position({ world_position.chunk.x + 1, world_position.chunk.y });
        auto const cell = Position { -1, world_position.in_chunk.y };

        set_and_check_empty({ chunk, cell }, state);
    }

    if (world_position.in_chunk.y == 0 && world_position.in_chunk.x == 0) {
        auto const chunk = wrap_chunk_position({ world_position.chunk.x - 1, world_position.chunk.y - 1 });
        auto const cell = Position { static_cast<int>(chunk_size), static_cast<int>(chunk_size) };

        set_and_check_empty({ chunk, cell }, state);
    } else if (world_position.in_chunk.y == static_cast<int>(chunk_size) - 1 && world_position.in_chunk.x == 0) {
        auto const chunk = wrap_chunk_position({ world_position.chunk.x - 1, world_position.chunk.y + 1 });
        auto const cell = Position { static_cast<int>(chunk_size), -1 };

        set_and_check_empty({ chunk, cell }, state);
    } else if (world_position.in_chunk.y == 0 && world_position.in_chunk.x == static_cast<int>(chunk_size) - 1) {
        auto const chunk = wrap_chunk_position({ world_position.chunk.x + 1, world_position.chunk.y - 1 });
        auto const cell = Position { -1, static_cast<int>(chunk_size) };

        set_and_check_empty({ chunk, cell }, state);
    } else if (world_position.in_chunk.y == static_cast<int>(chunk_size) - 1 && world_position.in_chunk.x == static_cast<int>(chunk_size) - 1) {
        auto const chunk = wrap_chunk_position({ world_position.chunk.x + 1, world_position.chunk.y + 1 });
        auto const cell = Position { -1, -1 };

        set_and_check_empty({ chunk, cell }, state);
    }
}

void World::set_and_check_empty(WorldPosition const& world_position, state_t const state) {
    auto const chunk = chunks.find(world_position.chunk);

    if (state == default_state) {
        if (chunk != std::end(chunks)) {
            //std::cout << "case default_state & chunk already exists\n";
            chunk->second.set(world_position.in_chunk, state);
            if (chunk->second.updated_cell_count() <= 0) {
                std::cout << "Put asleep " << world_position.chunk << '\n';
                chunks.erase(chunk);
            }
        } else {
            //std::cout << "case default_state & chunk doesn't exists\n";
        }
    } else {
        if (chunk != std::end(chunks)) {
            //std::cout << "case not default_state & chunk already exists\n";
            chunk->second.set(world_position.in_chunk, state);
        } else {
            //std::cout << "case not default_state & chunk doesn't exists\n";
            std::cout << "Awake: " << world_position.chunk << '\n';
            chunks.emplace(world_position.chunk, Chunk { default_state, chunk_size, chunk_size }).first->second.set(world_position.in_chunk, state);
        }
    }

    auto c = chunks.find(world_position.chunk);
    if (c == std::end(chunks)) {
        //std::cerr << "CHUNK " << world_position.chunk << " is full of " << default_state << '\n';
        return;
    }
    /*auto const& chunk_ref = c->second;
    std::cerr << "CHUNK " << world_position.chunk << ":\n";
    std::cerr << "\t" << chunk_ref.get({ -1, -1 }) << " " << chunk_ref.get({ 0, -1 }) << " " << chunk_ref.get({ 1, -1 }) << " " << chunk_ref.get({ 2, -1 }) << '\n';
    std::cerr << "\t" << chunk_ref.get({ -1,  0 }) << " " << chunk_ref.get({ 0,  0 }) << " " << chunk_ref.get({ 1,  0 }) << " " << chunk_ref.get({ 2,  0 }) << '\n';
    std::cerr << "\t" << chunk_ref.get({ -1,  1 }) << " " << chunk_ref.get({ 0,  1 }) << " " << chunk_ref.get({ 1,  1 }) << " " << chunk_ref.get({ 2,  1 }) << '\n';
    std::cerr << "\t" << chunk_ref.get({ -1,  2 }) << " " << chunk_ref.get({ 0,  2 }) << " " << chunk_ref.get({ 1,  2 }) << " " << chunk_ref.get({ 2,  2 }) << '\n';*/
}





void World::to_texture(Position const& chunk_position, sf::Texture& texture, std::unordered_map<state_t, sf::Color> const& colors) const {
    auto const chunk = chunks.find(chunk_position);

    //std::cout << "Copy chunk " << chunk_position << " to texture\n";

    if (chunk != std::end(chunks)) {
        //std::cout << "Chunk found !\n";
        return chunk->second.to_texture(texture, colors);
    } else {
        //std::cout << "No chunk => fill texture with default color\n";
        std::vector<sf::Uint8> pixels( 4 * chunk_size * chunk_size );
        //auto const color_iter = colors.find(default_state);
        sf::Color color = sf::Color{120, 80, 120}; // color_iter == std::end(colors) ? sf::Color{} : color_iter->second;
        for(std::size_t i{0}; i < pixels.size(); i += 4) {
            pixels[i + 0] = color.r;
            pixels[i + 1] = color.g;
            pixels[i + 2] = color.b;
            pixels[i + 3] = color.a;
        }
        texture.update(pixels.data());
    }
}





int pos_mod(int const a, int const b) {
    int m = a % b;
    return m + (m < 0 ? b : 0);
}

Position World::wrap_chunk_position(Position const& position) const {
    auto const wrap_axis = [] (int axis, std::optional<std::size_t> const max, bool wrap) -> int {
        if (wrap) {
            int const value = (axis + static_cast<int>(*max)) % (static_cast<int>(*max) * 2 + 1);
            return value - *max;
        } else {
            return max ? std::max(std::min(axis, static_cast<int>(*max)), -static_cast<int>(*max)) : axis;
        }
    };

    return Position {
        wrap_axis(position.x, option.get_horizontal_count(), option.do_horizontal_wrap()),
        wrap_axis(position.y, option.get_vertical_count(), option.do_vertical_wrap())
    };
}

WorldPosition World::to_world_position(Position const& position) const {
    WorldPosition world_position;
    world_position.in_chunk.x = pos_mod(position.x, static_cast<int>(chunk_size));
    world_position.chunk.x = (position.x - world_position.in_chunk.x) / static_cast<int>(chunk_size);

    world_position.in_chunk.y = pos_mod(position.y, static_cast<int>(chunk_size));
    world_position.chunk.y = (position.y - world_position.in_chunk.y) / static_cast<int>(chunk_size);

    world_position.chunk = wrap_chunk_position(world_position.chunk);

    return world_position;
}


WorldPosition World::get_chunk_coords(Position const& position) const {
    WorldPosition world_position;
    world_position.in_chunk.x = pos_mod(position.x, static_cast<int>(chunk_size));
    world_position.chunk.x = (position.x - world_position.in_chunk.x) / static_cast<int>(chunk_size);

    world_position.in_chunk.y = pos_mod(position.y, static_cast<int>(chunk_size));
    world_position.chunk.y = (position.y - world_position.in_chunk.y) / static_cast<int>(chunk_size);

    return world_position;
}


WorldPosition World::get_wrapped_chunk_coords(Position position) const {
    if (option.do_horizontal_wrap()) {
        position.x %= *option.get_horizontal_count() * chunk_size;
    }
    if (option.do_vertical_wrap()) {
        position.y %= *option.get_vertical_count() * chunk_size;
    }

    return get_chunk_coords(position);
}






using update_chunk_iterator_t = std::unordered_map<Position, Chunk>::iterator;
using sync_chunk_iterator_t = std::unordered_map<Position, Chunk*>::iterator;


std::pair<Position, Chunk*> get_next_chunk_and_position(update_chunk_iterator_t& begin, update_chunk_iterator_t const& end, std::mutex& begin_mutex) {
    std::lock_guard<std::mutex> lock{ begin_mutex };
    if (begin == end)
        return { {0, 0}, nullptr };

    auto const it = begin++;
    return { it->first, &it->second };
}

Chunk* get_next_chunk(update_chunk_iterator_t& begin, update_chunk_iterator_t const& end, std::mutex& begin_mutex) {
    return get_next_chunk_and_position(begin, end, begin_mutex).second;
}

std::pair<Position, Chunk*> get_next_chunk_and_position(sync_chunk_iterator_t& begin, sync_chunk_iterator_t const& end, std::mutex& begin_mutex) {
    std::lock_guard<std::mutex> lock{ begin_mutex };
    if (begin == end)
        return { {0, 0}, nullptr };

    return *begin++;
}

void update_chunk(Rules const& rules, update_chunk_iterator_t& begin, update_chunk_iterator_t const& end, std::mutex& begin_mutex) {
    for(auto* chunk = get_next_chunk(begin, end, begin_mutex); chunk != nullptr; chunk = get_next_chunk(begin, end, begin_mutex)) {
        std::vector<state_t> row(chunk->get_width());
        std::vector<state_t> prev_row(chunk->get_width());

        for(int y{0}; static_cast<std::size_t>(y) < chunk->get_height(); ++y) {
            for(int x{0}; static_cast<std::size_t>(x) < chunk->get_width(); ++x) {
                auto const neighbors = chunk->get_neighbors({x, y});
                row[x] = rules.get(neighbors.there())(neighbors);
            }
            
            if (y > 0) {
                chunk->set_row(y - 1, std::begin(prev_row));
            }

            row.swap(prev_row);

        }

        chunk->set_row(chunk->get_height() - 1, std::begin(prev_row));
    } 
}

void sync_chunk_with(std::unordered_map<Position, Chunk>& chunks, Chunk& current, Position const& target, void(Chunk::*func)(Chunk const&)) {
    auto const it = const_cast<std::unordered_map<Position, Chunk> const&>(chunks).find(target);
    if (it != std::end(const_cast<std::unordered_map<Position, Chunk> const&>(chunks))) {
        (current.*func)(it->second);
    }
}

void sync_chunk(std::unordered_map<Position, Chunk>& chunks, sync_chunk_iterator_t& begin, sync_chunk_iterator_t const& end, std::mutex& begin_mutex) {
    for(auto chunk = get_next_chunk_and_position(begin, end, begin_mutex); chunk.second != nullptr; chunk = get_next_chunk_and_position(begin, end, begin_mutex)) {
        auto const position = chunk.first;
        auto& chunk_ref = *chunk.second;

        //std::cout << "Current Chunk: " << position << ", <" << chunk_ref.get({0, 0}) << ", " << chunk_ref.get({1, 0}) << "> <" << chunk_ref.get({0, 1}) << ", " << chunk_ref.get({1, 1}) << ">\n";

        //std::cerr << "Sync north border of " << Position{ position.x, position.y - 1 } << " with " << position << '\n';
        sync_chunk_with(chunks, chunk_ref, { position.x, position.y - 1 }, &Chunk::sync_hidden_north_border);

        //std::cerr << "Sync east border of " << Position{ position.x + 1, position.y } << " with " << position << '\n';
        sync_chunk_with(chunks, chunk_ref, { position.x + 1, position.y }, &Chunk::sync_hidden_east_border);

        //std::cerr << "Sync west border of " << Position{ position.x - 1, position.y } << " with " << position << '\n';
        sync_chunk_with(chunks, chunk_ref, { position.x - 1, position.y }, &Chunk::sync_hidden_west_border);

        //std::cerr << "Sync south border of " << Position{ position.x, position.y + 1 } << " with " << position << '\n';
        sync_chunk_with(chunks, chunk_ref, { position.x, position.y + 1 }, &Chunk::sync_hidden_south_border);

        //std::cerr << "Sync north west corner of " << Position{ position.x - 1, position.y - 1 } << " with " << position << '\n';
        sync_chunk_with(chunks, chunk_ref, { position.x - 1, position.y - 1 }, &Chunk::sync_hidden_north_west_corner);

        //std::cerr << "Sync north east corner of " << Position{ position.x + 1, position.y - 1 } << " with " << position << '\n';
        sync_chunk_with(chunks, chunk_ref, { position.x + 1, position.y - 1 }, &Chunk::sync_hidden_north_east_corner);

        //std::cerr << "Sync south east corner of " << Position{ position.x + 1, position.y + 1 } << " with " << position << '\n';
        sync_chunk_with(chunks, chunk_ref, { position.x + 1, position.y + 1 }, &Chunk::sync_hidden_south_east_corner);

        //std::cerr << "Sync south west corner of " << Position{ position.x - 1, position.y + 1 } << " with " << position << '\n';
        sync_chunk_with(chunks, chunk_ref, { position.x - 1, position.y + 1 }, &Chunk::sync_hidden_south_west_corner);

        //std::cerr << "CHUNK " << position << ":\n";
        //std::cerr << "\t" << chunk_ref.get({ -1, -1 }) << " " << chunk_ref.get({ 0, -1 }) << " " << chunk_ref.get({ 1, -1 }) << " " << chunk_ref.get({ 2, -1 }) << '\n';
        //std::cerr << "\t" << chunk_ref.get({ -1,  0 }) << " " << chunk_ref.get({ 0,  0 }) << " " << chunk_ref.get({ 1,  0 }) << " " << chunk_ref.get({ 2,  0 }) << '\n';
        //std::cerr << "\t" << chunk_ref.get({ -1,  1 }) << " " << chunk_ref.get({ 0,  1 }) << " " << chunk_ref.get({ 1,  1 }) << " " << chunk_ref.get({ 2,  1 }) << '\n';
        //std::cerr << "\t" << chunk_ref.get({ -1,  2 }) << " " << chunk_ref.get({ 0,  2 }) << " " << chunk_ref.get({ 1,  2 }) << " " << chunk_ref.get({ 2,  2 }) << '\n';
    }
}







void World::update(Rules const& rules, unsigned const additional_thread) {
    update_chunk_iterator_t begin = std::begin(chunks);
    update_chunk_iterator_t const end = std::end(chunks);
    std::mutex begin_mutex;
/*
    for(auto const& p : chunks) {
        std::cout << p.first << " : " << p.second.updated_cell_count() << ", ";
    }
    std::cout << '\n';*/
/*
    std::cout << "### START ###\n";
    for(int cy = -1; cy <= 1; ++cy) {
        for(int y = -1; y <= (int)chunk_size; ++y) {
            std::cerr << '\t';
            for(int cx = -1; cx <= 1; ++cx) {
                auto chunk_iter = chunks.find({cx, cy});
                for(int x = -1; x <= (int)chunk_size; ++x) {
                    if (x >= 0 && x < chunk_size && y >= 0 && y < chunk_size) {
                        std::cerr << "\033[1m";
                    }

                    if (chunk_iter != std::end(chunks)) {
                        std::cerr << chunk_iter->second.get({x, y});
                    } else {
                        std::cerr << '#';
                    }

                    if (x >= 0 && x < chunk_size && y >= 0 && y < chunk_size) {
                        std::cerr << "\033[0m";
                    }
                }
                std::cerr << ' ';
            }
            std::cerr << '\n';
        }
        std::cerr << '\n';
    }
*/
    std::vector<std::thread> threads;
    threads.reserve(additional_thread);
    /*for(unsigned t{0}; t < additional_thread; ++t)
        threads.emplace_back(update_chunk, std::cref(rules), std::ref(begin), end, std::ref(begin_mutex));*/

    update_chunk(rules, begin, end, begin_mutex);

    for(auto& thread : threads)
        thread.join();

    threads.clear();
/*
    std::cout << "### UPDATED ###\n";
    for(int cy = -1; cy <= 1; ++cy) {
        for(int y = -1; y <= static_cast<int>(chunk_size); ++y) {
            std::cerr << '\t';
            for(int cx = -1; cx <= 1; ++cx) {
                auto chunk_iter = chunks.find({cx, cy});
                for(int x = -1; x <= static_cast<int>(chunk_size); ++x) {
                    if (x >= 0 && x < chunk_size && y >= 0 && y < chunk_size) {
                        std::cerr << "\033[1m";
                    }

                    if (chunk_iter != std::end(chunks)) {
                        std::cerr << chunk_iter->second.get({x, y});
                    } else {
                        std::cerr << '#';
                    }

                    if (x >= 0 && x < chunk_size && y >= 0 && y < chunk_size) {
                        std::cerr << "\033[0m";
                    }
                }
                std::cerr << ' ';
            }
            std::cerr << '\n';
        }
        std::cerr << '\n';
    }
*/
    std::unordered_map<Position, Chunk*> chunks_list(chunks.size());
    std::vector<Position> position_to_add;
    for(auto& chunk : chunks) {
        //std::cout << "WITH " << chunk.first << '\n';
        chunks_list.emplace(chunk.first, &chunk.second);
        position_to_add.push_back({ chunk.first.x + 1, chunk.first.y - 1 });
        position_to_add.push_back({ chunk.first.x + 1, chunk.first.y     });
        position_to_add.push_back({ chunk.first.x + 1, chunk.first.y + 1 });

        position_to_add.push_back({ chunk.first.x    , chunk.first.y - 1 });
        position_to_add.push_back({ chunk.first.x    , chunk.first.y + 1 });

        position_to_add.push_back({ chunk.first.x - 1, chunk.first.y - 1 });
        position_to_add.push_back({ chunk.first.x - 1, chunk.first.y     });
        position_to_add.push_back({ chunk.first.x - 1, chunk.first.y + 1 });
    }
    
    for(auto p : position_to_add) {
        auto it = chunks.emplace(p, Chunk {default_state, chunk_size, chunk_size}).first;
        chunks_list.emplace(it->first, &it->second);
        //std::cout << "+ " << p << '\n';
    }

    auto begin_list = std::begin(chunks_list);
    auto const end_list = std::end(chunks_list);

    begin = std::begin(chunks);

    /*for(unsigned t{0}; t < additional_thread; ++t)
        threads.emplace_back(sync_chunk, std::ref(chunks), std::ref(begin), std::cref(end), std::ref(begin_mutex));*/

    sync_chunk(chunks, begin_list, end_list, begin_mutex);

    for(auto& thread : threads)
        thread.join();
/*
    std::cout << "### SYNCHRONIZED ###\n";
    for(int cy = -1; cy <= 1; ++cy) {
        for(int y = -1; y <= (int)chunk_size; ++y) {
            std::cerr << '\t';
            for(int cx = -1; cx <= 1; ++cx) {
                auto chunk_iter = chunks.find({cx, cy});
                for(int x = -1; x <= static_cast<int>(chunk_size); ++x) {
                    if (x >= 0 && x < chunk_size && y >= 0 && y < chunk_size) {
                        std::cerr << "\033[1m";
                    }

                    if (chunk_iter != std::end(chunks)) {
                        std::cerr << chunk_iter->second.get({x, y});
                    } else {
                        std::cerr << '#';
                    }

                    if (x >= 0 && x < chunk_size && y >= 0 && y < chunk_size) {
                        std::cerr << "\033[0m";
                    }
                }
                std::cerr << ' ';
            }
            std::cerr << '\n';
        }
        std::cerr << '\n';
    }
    std::cout << "### END ###\n";
*/
    for(auto it = std::begin(chunks); it != std::end(chunks);) {
        if (it->second.updated_cell_count() == 0) {
            //std::cout << "Put asleep " << it->first << '\n';
            it = chunks.erase(it);
        } else {
            ++it;
        }
    }
/*
    for(auto const& p : chunks) {
        std::cout << p.first << " : " << p.second.updated_cell_count() << ", ";
    }
    std::cout << '\n';
*/
/*
    for(auto const& c : chunks_list) {
        if (c.second->updated_cell_count() > 0) {
            std::cout << "Awaken " << c.first << '\n';
            chunks.emplace(c.first, *c.second);
        }
    }*/
}

}