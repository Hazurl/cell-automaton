#pragma once

#include <cellz/Automate/Automate.hpp>

#include <SFML/Graphics.hpp>

namespace cellz {

class Grid : public sf::Drawable {
public:

    Grid() = default;

    void update(Automate const& automate);
    void update(Automate const& automate, int start_x, int start_y, unsigned rect_width, unsigned rect_height);

    void set_position(sf::Vector2f const& position);
    void set_scale(sf::Vector2f const& scale);
    void set_rotation(float rotation);

    sf::Vector2f get_position() const;
    sf::Vector2f get_scale() const;
    float get_rotation() const;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void define_color(unsigned state, sf::Color const& color);

private:

    sf::Sprite object;
    sf::Texture texture;

    std::map<unsigned, sf::Color> colors;

};

}