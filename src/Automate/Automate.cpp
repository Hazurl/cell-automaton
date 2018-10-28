#include <cellz/Automate/Automate.hpp>

namespace cellz {

Automate::CellLookup::CellLookup(Automate const& app, unsigned id) : app(app), id(id), coords(*app.to_coords(id)) {}

unsigned Automate::CellLookup::current() const {
    return app.get(coords.x, coords.y);
}

Automate::CellLookup::Around Automate::CellLookup::arounds() const {
    Automate::CellLookup::Around cells;
    cells.North     = app.get(coords.x    , coords.y - 1);
    cells.NorthEast = app.get(coords.x + 1, coords.y - 1);
    cells.East      = app.get(coords.x + 1, coords.y);
    cells.SouthEast = app.get(coords.x + 1, coords.y + 1);
    cells.South     = app.get(coords.x    , coords.y + 1);
    cells.SouthWest = app.get(coords.x - 1, coords.y + 1);
    cells.West      = app.get(coords.x - 1, coords.y);
    cells.NorthWest = app.get(coords.x - 1, coords.y - 1);
    return cells;
}

void Automate::CellLookup::Around::for_each(std::function<void(unsigned)> const& f) const {
    f(North);
    f(NorthEast);
    f(East);
    f(SouthEast);
    f(South);
    f(SouthWest);
    f(West);
    f(NorthWest);
}

Automate::CellLookup::Neighbour Automate::CellLookup::neighbours() const {
    Automate::CellLookup::Neighbour cells;
    cells.North     = app.get(coords.x    , coords.y - 1);
    cells.East      = app.get(coords.x + 1, coords.y);
    cells.South     = app.get(coords.x    , coords.y + 1);
    cells.West      = app.get(coords.x - 1, coords.y);
    return cells;
}

void Automate::CellLookup::Neighbour::for_each(std::function<void(unsigned)> const& f) const {
    f(North);
    f(East);
    f(South);
    f(West);
}

Automate::Automate(unsigned cells_width_count, unsigned cells_height_count, unsigned start_state) 
    : default_state(start_state), mode(Automate::WrapMode::None),
      width(cells_width_count), height(cells_height_count),
      cells(cells_width_count * cells_height_count, start_state) {}

void Automate::update_all() {
    std::vector<unsigned> new_cells(cells.size());
    unsigned id = 0;
    std::transform(cells.begin(), cells.end(), new_cells.begin(), [&id, this] (unsigned c) {
        return this->rules[c](Automate::CellLookup(*this, id++)); // TODO : check rule exists
    });
    cells = std::move(new_cells);
}

void Automate::update_selected(std::vector<sf::Vector2i> const& selected) {
    std::vector<unsigned> new_cells(cells);
    for(auto const& coords : selected) {
        auto id = *from_coords(coords.x, coords.y);
        new_cells[id] = rules[get(coords.x, coords.y)](Automate::CellLookup(*this, id)); // TODO : check rule exists
    }
    cells = std::move(new_cells);
}

void Automate::define_rule(unsigned state, std::function<unsigned(CellLookup const&)> rule) {
    rules.insert({ state, rule });
}

void Automate::define_wrap_mode(WrapMode mode, unsigned default_state) {
    this->mode = mode;
    this->default_state = default_state;
}

unsigned Automate::get_width() const {
    return width;
}

unsigned Automate::get_height() const {
    return height;
}

void Automate::set(int x, int y, unsigned state) {
    auto id = from_coords(x, y);
    if (id)
        cells[*id] = state;
}

void Automate::set_all(unsigned state) {
    std::fill(cells.begin(), cells.end(), state);
}

void Automate::set_all(std::function<unsigned(int, int)> const& generator) {
    unsigned id = 0;
    std::generate(cells.begin(), cells.end(), [generator, &id, this] () {
        auto coords = this->to_coords(id);
        return generator(coords->x, coords->y);
    });
}

std::optional<sf::Vector2i> Automate::to_coords(unsigned id) const {
    if (id >= width * height)
        return {};
    return sf::Vector2i{ static_cast<int>(id % width), static_cast<int>(id / width) };
}

std::optional<unsigned> Automate::from_coords(int x, int y) const {
    if (x < 0) {
        if (mode == Automate::WrapMode::Horizontal || mode == Automate::WrapMode::Both)
            x = width - (-x % width) - 1;
        else
            return default_state;
    } else if (x >= static_cast<int>(width)) {
        if (mode == Automate::WrapMode::Horizontal || mode == Automate::WrapMode::Both)
            x = x % width;
        else
            return {};
    }
    if (y < 0) {
        if (mode == Automate::WrapMode::Vertical || mode == Automate::WrapMode::Both)
            y = height - (-y % height) - 1;
        else
            return default_state;
    } else if (y >= static_cast<int>(height)) {
        if (mode == Automate::WrapMode::Vertical || mode == Automate::WrapMode::Both)
            y = y % height;
        else
            return {};
    }
    return y * width + x;
}

unsigned Automate::get(int x, int y) const {
    auto id = from_coords(x, y);
    return id ? cells[*id] : default_state;
}

Automate Automate::game_of_life(unsigned width, unsigned height) {
    static constexpr unsigned state_alive = 1;
    static constexpr unsigned state_dead = 0;

    Automate app(width, height, state_dead);

    app.define_rule(state_dead, 
    [] (Automate::CellLookup const& lookup) {
        unsigned counter = 0;
        lookup.arounds().for_each([&counter] (unsigned c) { counter += c; });
        return counter == 3 ? state_alive : state_dead;
    });

    app.define_rule(state_alive, 
    [] (Automate::CellLookup const& lookup) {
        unsigned counter = 0;
        lookup.arounds().for_each([&counter] (unsigned c) { counter += c; });
        return counter == 3 || counter == 2 ? state_alive : state_dead;
    });

    return app;
}

Automate Automate::langton_ant(unsigned width, unsigned height, Automate::LangtonAnt& ant) {
    Automate app(width, height, 0);

    ant.x = width / 2;
    ant.y = height / 2;
    ant.dir = 0; // North

    app.define_rule(0, 
    [&ant] (Automate::CellLookup const&) {
        ant.dir = (ant.dir + 1) % 4;
        switch(ant.dir) {
            case 0: ant.y--; break;
            case 1: ant.x++; break;
            case 2: ant.y++; break;
            case 3: ant.x--; break;
            default: break;
        }
        return 1;
    });

    app.define_rule(1, 
    [&ant] (Automate::CellLookup const&) {
        ant.dir = (ant.dir + 3) % 4;
        switch(ant.dir) {
            case 0: ant.y--; break;
            case 1: ant.x++; break;
            case 2: ant.y++; break;
            case 3: ant.x--; break;
            default: break;
        }
        return 0;
    });

    return app;
}

}