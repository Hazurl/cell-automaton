#include <cellz/World.hpp>
#include <cellz/Rules.hpp>

#include <SFML/Graphics.hpp>

#include <vector>

int main() {
    using namespace cellz;
    
    static unsigned constexpr window_height = 901;
    static unsigned constexpr window_width = 901;
    static int constexpr chunk_count = 9;
    static std::size_t constexpr chunk_size = 100;
    static unsigned constexpr chunk_padding = 1;
    int chunk_x = 0;
    int chunk_y = 0;



    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "SFML - Cellular Automata");
    window.setPosition({500, 50});
    //sf::View window_view = window.getView();
    //window_view.setCenter(0, 0);
    //window.setView(window_view);




    Option option{};
    option.finite_horizontal_count(chunk_count - 1);
    option.finite_vertical_count(chunk_count - 1);
    World world{ 0, chunk_size, option };
/*
    world.set({-1, 0}, 1);
    world.set({0, 0}, 1);
    world.set({1, 0}, 1);

    world.set({2, 3}, 1);
    world.set({3, 3}, 1);
    world.set({4, 3}, 1);
*/

    for(auto const& v : std::initializer_list<Position>{
        {1, 0},
        {2, 1},
        {2, 2},
        {1, 2},
        {0, 2},
    }) {
        world.set(v, 1);
        world.set({v.x + 10, v.y}, 1);
    }

/*
    world.set({0, 0}, 1);
    world.set({1, 0}, 1);
    world.set({0, 1}, 1);
    world.set({1, 1}, 1);
*/
/*
    world.set({0, 2}, 1);
    world.set({1, 2}, 1);
    world.set({0, -1}, 1);
    world.set({1, -1}, 1);
    world.set({-1, 0}, 1);
    world.set({-1, 1}, 1);
    world.set({2, 0}, 1);
    world.set({2, 1}, 1);
*/



    Rules rules{2};
    rules.on(0,  [] (Neighbors const& neighbors) -> state_t { 
        return neighbors.count(1) == 3 ? 1 : 0; 
    }).on(1, [] (Neighbors const& neighbors) -> state_t { 
        auto const count = neighbors.count(1); 
        return count == 2 || count == 3 ? 1 : 0; 
    });

    std::unordered_map<state_t, sf::Color> colors {
        {0, sf::Color{80, 80, 80}},
        {1, sf::Color{220, 220, 220}}
    };



    bool start = true;


    std::vector<sf::Texture> textures(chunk_count * chunk_count);
    std::vector<sf::Sprite> sprites;
    std::vector<Position> chunks_position {
        {-1, -1},
        { 0, -1},
        { 1, -1},
        {-1,  0},
        { 0,  0},
        { 1,  0},
        {-1,  1},
        { 0,  1},
        { 1,  1}
    };

    {
        auto texture_it = std::begin(textures);
        for(int y = 0; y < chunk_count; ++y) {
            for(int x = 0; x < chunk_count; ++x) {
                auto chunk_position = Position{ x + chunk_x - chunk_count / 2, y + chunk_y - chunk_count / 2};
                auto& texture = *(texture_it++);

                if (!texture.create(chunk_size, chunk_size)) {
                    std::cerr << "Couldn't create the texture\n";
                    return 1;
                }

                world.to_texture(chunk_position, texture, colors);

                auto& sprite = sprites.emplace_back(texture);
                
                auto const scale = ((static_cast<float>(window_width - chunk_padding) / static_cast<float>(chunk_count)) - static_cast<float>(chunk_padding)) / static_cast<float>(chunk_size);
                sprite.setScale(scale, scale);

                float const factor = static_cast<float>(window_width - chunk_padding) / static_cast<float>(chunk_count);
                sprite.setPosition(chunk_padding + x * factor, chunk_padding + y * factor);
            }
        }
    }

    auto update_textures = [&textures, &chunk_x, &chunk_y, &colors, &world] () {
        auto texture_it = std::begin(textures);
        for(int y = 0; y < chunk_count; ++y) {
            for(int x = 0; x < chunk_count; ++x) {
                auto chunk_position = Position{ x + chunk_x - chunk_count / 2, y + chunk_y - chunk_count / 2};
                auto& texture = *(texture_it++);

                world.to_texture(chunk_position, texture, colors);
            }
        }
    };
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Space) {
                if (!start && !event.key.shift) {
                    world.update(rules);
    
                    update_textures();
                }

                start = event.key.shift ? !start : false;
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Left) {
                chunk_x--;

                update_textures();
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Right) {
                chunk_x++;

                update_textures();
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Down) {
                chunk_y++;

                update_textures();
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Up) {
                chunk_y--;

                update_textures();
            }
        }

        if (start) {
            world.update(rules);
            world.update(rules);
            world.update(rules);
            world.update(rules);
            update_textures();
        }


        window.clear();
        for(auto const& sprite : sprites) {
            window.draw(sprite);
        }
        window.display();
    }
}