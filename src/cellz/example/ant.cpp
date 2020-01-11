/*#include <cellz/example/ant.hpp>
#include <iostream>

namespace cellz::example {

const sf::Color LangtonAnt::Colors::default_on = sf::Color::Black;
const sf::Color LangtonAnt::Colors::default_off = sf::Color::White;

std::pair<Automate, Grid> LangtonAnt::make() const noexcept {
    //Setup Grid
    Grid grid;

    grid.set_scale({size.scale, size.scale});

    grid.define_color(states.on, colors.on);
    grid.define_color(states.ant_down_on, colors.on);
    grid.define_color(states.ant_up_on, colors.on);
    grid.define_color(states.ant_right_on, colors.on);
    grid.define_color(states.ant_left_on, colors.on);

    grid.define_color(states.off, colors.off);
    grid.define_color(states.ant_down_off, colors.off);
    grid.define_color(states.ant_up_off, colors.off);
    grid.define_color(states.ant_right_off, colors.off);
    grid.define_color(states.ant_left_off, colors.off);


    // Setup Automata
    Automate app(size.width, size.heigth, states.off);
    app.set(size.width / 2, size.heigth / 2, states.ant_left_off);

    app.define_rule(states.ant_down_on, 
    [states = states] (Automate::CellLookup const&) {
        return states.off;
    });
    app.define_rule(states.ant_up_on, 
    [states = states] (Automate::CellLookup const&) {
        return states.off;
    });
    app.define_rule(states.ant_left_on, 
    [states = states] (Automate::CellLookup const&) {
        return states.off;
    });
    app.define_rule(states.ant_right_on, 
    [states = states] (Automate::CellLookup const&) {
        return states.off;
    });

    app.define_rule(states.ant_down_off, 
    [states = states] (Automate::CellLookup const&) {
        return states.on;
    });
    app.define_rule(states.ant_up_off, 
    [states = states] (Automate::CellLookup const&) {
        return states.on;
    });
    app.define_rule(states.ant_left_off, 
    [states = states] (Automate::CellLookup const&) {
        return states.on;
    });
    app.define_rule(states.ant_right_off, 
    [states = states] (Automate::CellLookup const&) {
        return states.on;
    });

    app.define_rule(states.on, 
    [states = states] (Automate::CellLookup const& cs) {
        auto around = cs.arounds();

        if (around.North == states.ant_right_on || around.North == states.ant_left_off)
            return states.ant_down_on;
        if (around.East == states.ant_down_on || around.East == states.ant_up_off)
            return states.ant_left_on;
        if (around.South == states.ant_left_on || around.South == states.ant_right_off)
            return states.ant_up_on;
        if (around.West == states.ant_up_on || around.West == states.ant_down_off)
            return states.ant_right_on;

        return states.on;
    });

    app.define_rule(states.off, 
    [states = states] (Automate::CellLookup const& cs) {
        auto around = cs.arounds();

        if (around.North == states.ant_right_on || around.North == states.ant_left_off)
            return states.ant_down_off;
        if (around.East == states.ant_down_on || around.East == states.ant_up_off)
            return states.ant_left_off;
        if (around.South == states.ant_left_on || around.South == states.ant_right_off)
            return states.ant_up_off;
        if (around.West == states.ant_up_on || around.West == states.ant_down_off)
            return states.ant_right_off;

        return states.off;
    });


    return { app, grid };
}

}*/