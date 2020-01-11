/*#include <cellz/example/gol.hpp>
#include <iostream>

namespace cellz::example {

const sf::Color GameOfLife::Colors::default_alive = sf::Color::Black;
const sf::Color GameOfLife::Colors::default_dead = sf::Color::White;

std::pair<Automate, Grid> GameOfLife::make() const noexcept {
    //Setup Grid
    Grid grid;

    grid.set_scale({size.scale, size.scale});

    grid.define_color(states.alive, colors.alive);
    grid.define_color(states.dead, colors.dead);


    // Setup Automata
    Automate app(size.width, size.heigth, states.dead);

    app.define_rule(states.dead, 
    [states = states] (Automate::CellLookup const& lookup) {
        unsigned counter = 0;
        lookup.arounds().for_each([&counter] (unsigned c) { counter += c; });
        return counter == 3 ? states.alive : states.dead;
    });

    app.define_rule(states.alive, 
    [states = states] (Automate::CellLookup const& lookup) {
        unsigned counter = 0;
        lookup.arounds().for_each([&counter] (unsigned c) { counter += c; });
        return counter == 3 || counter == 2 ? states.alive : states.dead;
    });

    return { app, grid };
}


void GameOfLife::Patterns::block(Automate& app, sf::Vector2i pos, Automate::state_t state) {
    place(app, pos, state, {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}
    });
}

void GameOfLife::Patterns::beehive(Automate& app, sf::Vector2i pos, Automate::state_t state) {
    place(app, pos, state, {
        {1, 0},
        {2, 0},
        {3, 1},
        {2, 2},
        {1, 2},
        {0, 1}
    });
}

void GameOfLife::Patterns::loaf(Automate& app, sf::Vector2i pos, Automate::state_t state) {
    place(app, pos, state, {
        {1, 0},
        {2, 0},
        {3, 1},
        {3, 2},
        {2, 3},
        {1, 2},
        {0, 1}
    });
}

void GameOfLife::Patterns::boat(Automate& app, sf::Vector2i pos, Automate::state_t state) {
    place(app, pos, state, {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 2},
        {2, 1}
    });
}

void GameOfLife::Patterns::tub(Automate& app, sf::Vector2i pos, Automate::state_t state) {
    place(app, pos, state, {
        {0, 1},
        {1, 0},
        {1, 2},
        {2, 1}
    });
}

void GameOfLife::Patterns::blinker(Automate& app, sf::Vector2i pos, Automate::state_t state) {
    place(app, pos, state, {
        {0, 0},
        {1, 0},
        {2, 0}
    });
}

void GameOfLife::Patterns::toad(Automate& app, sf::Vector2i pos, Automate::state_t state) {
    place(app, pos, state, {
        {1, 0},
        {2, 0},
        {3, 0},
        {0, 1},
        {1, 1},
        {2, 1}
    });
}

void GameOfLife::Patterns::beacon(Automate& app, sf::Vector2i pos, Automate::state_t state) {
    place(app, pos, state, {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1},
        {2, 2},
        {2, 3},
        {3, 2},
        {3, 3}
    });
}

void GameOfLife::Patterns::pulsar(Automate& app, sf::Vector2i pos, Automate::state_t state) {
    place(app, pos, state, {
        {0, 2},
        {0, 3},
        {0, 4},
        {0, 8},
        {0, 9},
        {0, 10},

        {2, 0},
        {3, 0},
        {4, 0},
        {8, 0},
        {9, 0},
        {10, 0},

        {5, 2},
        {5, 3},
        {5, 4},
        {5, 8},
        {5, 9},
        {5, 10},

        {2, 5},
        {3, 5},
        {4, 5},
        {8, 5},
        {9, 5},
        {10, 5},

        {7, 2},
        {7, 3},
        {7, 4},
        {7, 8},
        {7, 9},
        {7, 10},

        {2, 7},
        {3, 7},
        {4, 7},
        {8, 7},
        {9, 7},
        {10, 7},

        {12, 2},
        {12, 3},
        {12, 4},
        {12, 8},
        {12, 9},
        {12, 10},

        {2, 12},
        {3, 12},
        {4, 12},
        {8, 12},
        {9, 12},
        {10, 12}
    });
}

void GameOfLife::Patterns::pentadecathlon(Automate& app, sf::Vector2i pos, Automate::state_t state) {
    place(app, pos, state, {
        {0, 1},
        {1, 1},
        {2, 0},
        {2, 2},
        {3, 1},
        {4, 1},
        {5, 1},
        {6, 1},
        {7, 0},
        {7, 2},
        {8, 1},
        {9, 1},
    });
}

void GameOfLife::Patterns::glider(Automate& app, sf::Vector2i pos, Automate::state_t state) {
    place(app, pos, state, {
        {1, 0},
        {2, 1},
        {2, 2},
        {1, 2},
        {0, 2},
    });
}

void GameOfLife::Patterns::l_spaceship(Automate& app, sf::Vector2i pos, Automate::state_t state) {
    place(app, pos, state, {
        {3, 0},

        {1, 1},
        {5, 1},

        {0, 2},

        {0, 3},
        {5, 3},

        {0, 4},
        {1, 4},
        {2, 4},
        {3, 4},
        {4, 4},
    });
}

void GameOfLife::Patterns::place(Automate& app, sf::Vector2i pos, Automate::state_t state, std::vector<sf::Vector2i> const& pattern) {
    for(auto const& off : pattern) {
        const auto coords = pos + off;
        app.set(coords.x, coords.y, state);
    }
}



}*/