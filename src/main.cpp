#include <iostream>

#include <cellz/Automate.hpp>
#include <cellz/Grid.hpp>
#include <cellz/example/gol.hpp>
#include <cellz/example/ant.hpp>

#include <cellz/World.hpp>
#include <cellz/Rules.hpp>

#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <ctime>

int main() {
    std::srand(std::time(nullptr));
    using namespace cellz;

    example::LangtonAnt ant;
    auto&&[app, grid] = ant.make();
/*
    example::GameOfLife::Patterns::block(app, {10, 10}, gol.states.alive);
    example::GameOfLife::Patterns::beehive(app, {20, 10}, gol.states.alive);
    example::GameOfLife::Patterns::loaf(app, {30, 10}, gol.states.alive);
    example::GameOfLife::Patterns::boat(app, {40, 10}, gol.states.alive);
    example::GameOfLife::Patterns::tub(app, {50, 10}, gol.states.alive);

    example::GameOfLife::Patterns::blinker(app, {10, 20}, gol.states.alive);
    example::GameOfLife::Patterns::toad(app, {20, 20}, gol.states.alive);
    example::GameOfLife::Patterns::beacon(app, {30, 20}, gol.states.alive);
    example::GameOfLife::Patterns::pulsar(app, {40, 20}, gol.states.alive);
    example::GameOfLife::Patterns::pentadecathlon(app, {60, 20}, gol.states.alive);

    example::GameOfLife::Patterns::glider(app, {10, 25}, gol.states.alive);
    example::GameOfLife::Patterns::l_spaceship(app, {50, 50}, gol.states.alive);
*/
    app.define_wrap_mode(Automate::WrapMode::Both);

    sf::RenderWindow window(sf::VideoMode(ant.size.width * ant.size.scale, ant.size.heigth * ant.size.scale), "SFML - Cellular Automata");

    sf::Clock clock;
    float dt = 0;

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

        dt += clock.restart().asSeconds();
        while(dt > 0.002) {
            app.update_all();
            dt -= 0.002;
        }
        grid.update(app);

        window.clear();
        window.draw(grid);
        window.display();
    }
}