#pragma once
/*
#include <cellz/Automate.hpp>
#include <cellz/Grid.hpp>
#include <SFML/Graphics.hpp>
#include <utility>

namespace cellz::example {

class GameOfLife {
public:

    struct Colors {
        static const sf::Color default_alive;
        static const sf::Color default_dead;

        sf::Color alive = default_alive;
        sf::Color dead = default_dead;
    };

    Colors colors;

    struct States {
        static constexpr Automate::state_t default_alive = 1;
        static constexpr Automate::state_t default_dead = 0;

        Automate::state_t alive = default_alive;
        Automate::state_t dead = default_dead;
    };

    States states;

    struct Size {
        static constexpr unsigned default_width = 100;
        static constexpr unsigned default_height = 100;
        static constexpr float default_scale = 8;

        unsigned width = default_width;
        unsigned heigth = default_height;
        float scale = default_scale;
    };

    Size size;

    std::pair<Automate, Grid> make() const noexcept;

    struct Patterns {
        static void block(Automate& app, sf::Vector2i pos, Automate::state_t state);
        static void beehive(Automate& app, sf::Vector2i pos, Automate::state_t state);
        static void loaf(Automate& app, sf::Vector2i pos, Automate::state_t state);
        static void boat(Automate& app, sf::Vector2i pos, Automate::state_t state);
        static void tub(Automate& app, sf::Vector2i pos, Automate::state_t state);

        static void blinker(Automate& app, sf::Vector2i pos, Automate::state_t state);
        static void toad(Automate& app, sf::Vector2i pos, Automate::state_t state);
        static void beacon(Automate& app, sf::Vector2i pos, Automate::state_t state);
        static void pulsar(Automate& app, sf::Vector2i pos, Automate::state_t state);
        static void pentadecathlon(Automate& app, sf::Vector2i pos, Automate::state_t state);

        static void glider(Automate& app, sf::Vector2i pos, Automate::state_t state);
        static void l_spaceship(Automate& app, sf::Vector2i pos, Automate::state_t state);

        static void place(Automate& app, sf::Vector2i pos, Automate::state_t state, std::vector<sf::Vector2i> const& pattern);
    };

};

}*/