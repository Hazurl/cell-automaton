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

    constexpr unsigned width = 100;
    constexpr unsigned height = 100;
    constexpr unsigned scale = 8;

    Automate::LangtonAnt ant;
    //Automate app = Automate::langton_ant(width, height, ant);
    Automate app = Automate::game_of_life(width, height);
    app.define_wrap_mode(Automate::WrapMode::Both);

    Grid grid; 
    app.set(width / 2, height / 2, 1);
    app.set(width / 2 + 1, height / 2, 1);
    app.set(width / 2 - 1, height / 2, 1);
    grid.define_color(state_alive, sf::Color::Black);
    grid.define_color(state_dead, sf::Color::White);

    grid.update(app);
    grid.set_scale({scale, scale});

    sf::RenderWindow window(sf::VideoMode(width * scale, height * scale), "SFML - GOL");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        /*app.update_selected({{ant.x, ant.y}});
        app.update_selected({{ant.x, ant.y}});
        app.update_selected({{ant.x, ant.y}});
        app.update_selected({{ant.x, ant.y}});
        app.update_selected({{ant.x, ant.y}});
        grid.update(app, ant.x - 5, ant.y - 5, 11, 11);*/
        app.update_all();

        grid.update(app);

        window.clear();
        window.draw(grid);
        window.display();
    }
}