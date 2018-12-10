#include <cellz/World.hpp>
#include <cellz/Rules.hpp>

#include <SFML/Graphics.hpp>

#include <vector>

int main() {
    using namespace cellz;
    
    static unsigned constexpr window_height = 800;
    static unsigned constexpr window_width = 800;
    static std::size_t constexpr chunk_count = 3;
    static std::size_t constexpr chunk_size = 100;
    static unsigned constexpr chunk_padding = 1;



    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "SFML - Cellular Automata");
    sf::View window_view = window.getView();
    window_view.setCenter(0, 0);
    window.setView(window_view);




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


    std::vector<sf::Texture> textures(9);
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

    auto chunk_position_iter = std::cbegin(chunks_position);
    for(auto& texture : textures) {
        if (!texture.create(chunk_size, chunk_size)) {
            std::cerr << "Couldn't create a the texture\n";
            return 1;
        }

        auto const chunk_position = *(chunk_position_iter++);
        world.to_texture(chunk_position, texture, colors);

        auto& sprite = sprites.emplace_back(texture);

        sprite.setOrigin(chunk_size / 2, chunk_size / 2);
        
        auto const scale = ((static_cast<float>(window_width - chunk_padding) / static_cast<float>(chunk_count)) - static_cast<float>(chunk_padding)) / static_cast<float>(chunk_size);
        sprite.setScale(scale, scale);

        auto const factor = static_cast<float>(scale * chunk_size + chunk_padding);
        sprite.setPosition(chunk_position.x * factor, chunk_position.y * factor);
    }
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Space) {
                if (!start && !event.key.shift) {
                    world.update(rules);
    
                    auto chunk_position_iter = std::cbegin(chunks_position);
                    for(auto& texture : textures) {
                        auto const chunk_position = *(chunk_position_iter++);
                        world.to_texture(chunk_position, texture, colors);
                    }
                }

                start = event.key.shift ? !start : false;
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Left) {
                for(auto& p : chunks_position) {
                    p.x--;
                }

                auto chunk_position_iter = std::cbegin(chunks_position);
                for(auto& texture : textures) {
                    auto const chunk_position = *(chunk_position_iter++);
                    world.to_texture(chunk_position, texture, colors);
                }
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Right) {
                for(auto& p : chunks_position) {
                    p.x++;
                }

                auto chunk_position_iter = std::cbegin(chunks_position);
                for(auto& texture : textures) {
                    auto const chunk_position = *(chunk_position_iter++);
                    world.to_texture(chunk_position, texture, colors);
                }
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Down) {
                for(auto& p : chunks_position) {
                    p.y++;
                }

                auto chunk_position_iter = std::cbegin(chunks_position);
                for(auto& texture : textures) {
                    auto const chunk_position = *(chunk_position_iter++);
                    world.to_texture(chunk_position, texture, colors);
                }
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Up) {
                for(auto& p : chunks_position) {
                    p.y--;
                }

                auto chunk_position_iter = std::cbegin(chunks_position);
                for(auto& texture : textures) {
                    auto const chunk_position = *(chunk_position_iter++);
                    world.to_texture(chunk_position, texture, colors);
                }
            }
        }

        if (start) {
            world.update(rules);

            auto chunk_position_iter = std::cbegin(chunks_position);
            for(auto& texture : textures) {
                auto const chunk_position = *(chunk_position_iter++);
                world.to_texture(chunk_position, texture, colors);
            }
        }


        window.clear();
        for(auto const& sprite : sprites) {
            window.draw(sprite);
        }
        window.display();
    }
}