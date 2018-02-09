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

    constexpr unsigned width = 200;
    constexpr unsigned height = 200;
    constexpr unsigned scale = 4;

    Automate::LangtonAnt ant;
    Automate app = Automate::langton_ant(width, height, ant);
    app.define_wrap_mode(Automate::WrapMode::Both);

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
        }
        app.update_selected({{ant.x, ant.y}});
        app.update_selected({{ant.x, ant.y}});
        app.update_selected({{ant.x, ant.y}});
        app.update_selected({{ant.x, ant.y}});
        app.update_selected({{ant.x, ant.y}});
        grid.update(app, ant.x - 5, ant.y - 5, 11, 11);

        window.clear();
        window.draw(grid);
        window.display();
    }
}