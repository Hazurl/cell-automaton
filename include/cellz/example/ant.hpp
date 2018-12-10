#pragma once

#include <cellz/Automate.hpp>
#include <cellz/Grid.hpp>

namespace cellz::example {

struct LangtonAnt {

    struct Colors {
        static const sf::Color default_on;
        static const sf::Color default_off;

        sf::Color on = default_on;
        sf::Color off = default_off;
    };

    Colors colors;

    struct States {
        static constexpr Automate::state_t default_ant_right_on = 0;
        static constexpr Automate::state_t default_ant_left_on = 1;
        static constexpr Automate::state_t default_ant_up_on = 2;
        static constexpr Automate::state_t default_ant_down_on = 3;
        static constexpr Automate::state_t default_ant_right_off = 4;
        static constexpr Automate::state_t default_ant_left_off = 5;
        static constexpr Automate::state_t default_ant_up_off = 6;
        static constexpr Automate::state_t default_ant_down_off = 7;
        static constexpr Automate::state_t default_on = 8;
        static constexpr Automate::state_t default_off = 9;

        Automate::state_t ant_right_on = default_ant_right_on;
        Automate::state_t ant_left_on = default_ant_left_on;
        Automate::state_t ant_up_on = default_ant_up_on;
        Automate::state_t ant_down_on = default_ant_down_on;
        Automate::state_t ant_right_off = default_ant_right_off;
        Automate::state_t ant_left_off = default_ant_left_off;
        Automate::state_t ant_up_off = default_ant_up_off;
        Automate::state_t ant_down_off = default_ant_down_off;
        Automate::state_t on = default_on;
        Automate::state_t off = default_off;
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

};

}