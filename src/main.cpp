#include <iostream>

#include <cellz/Automate/Automate.hpp>
#include <cellz/Automate/Grid.hpp>

#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <ctime>

int main() {
    std::srand(std::time(nullptr));
    using namespace cellz;

    constexpr unsigned state_alive = 1;
    constexpr unsigned state_dead = 0;

    constexpr unsigned width = 400;
    constexpr unsigned height = 400;
    constexpr unsigned scale = 2;

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

    app.set_all([] (int, int) { return std::rand() % 10 == 0 ? state_alive : state_dead; });

    Grid grid; 
    grid.define_color(state_alive, sf::Color::Black);
    grid.define_color(state_dead, sf::Color::White);

    grid.update(app);
    grid.set_scale({scale, scale});

    sf::RenderWindow window(sf::VideoMode(width * scale, height * scale), "GOL");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
                app.update_all();
                grid.update(app);
            }
        }

        window.clear();
        window.draw(grid);
        window.display();
    }
}