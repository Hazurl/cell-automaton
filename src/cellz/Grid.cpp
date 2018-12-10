#include <cellz/Grid.hpp>
#include <iostream>

namespace cellz {

void Grid::update(Automate const& amt) {
    unsigned width = amt.get_width();
    unsigned height = amt.get_height();
    if (texture.getSize() != sf::Vector2u(width, height)) {
        texture.create(width, height);
        object.setTexture(texture);
    }

    std::vector<sf::Uint8> pixels(width * height * 4);
    for(unsigned x = 0; x < width; x++) {
        for(unsigned y = 0; y < height; y++) {
            unsigned pos = 4 * (x + y * amt.get_width());
            sf::Color color = colors[amt.get(x, y)];
            pixels[pos    ] = color.r; // R
            pixels[pos + 1] = color.g; // G
            pixels[pos + 2] = color.b; // B
            pixels[pos + 3] = color.a; // A
        }
    } 
    texture.update(pixels.data());
}

void Grid::update(Automate const& amt, int start_x, int start_y, unsigned rect_width, unsigned rect_height) {
    unsigned width = amt.get_width();
    unsigned height = amt.get_height();
    if (texture.getSize() != sf::Vector2u(width, height)) {
        texture.create(width, height);
        object.setTexture(texture);
    }

    if (rect_width == 0 || start_x + rect_width <= 0 || static_cast<unsigned>(start_x) > width)
        return;
    if (rect_height == 0 || start_y + rect_height <= 0 || static_cast<unsigned>(start_y) > height)
        return;

    if (start_x < 0) {
        rect_width += start_x;
        start_x = 0;
    } else if (start_x + rect_width >= width) {
        rect_width = width - start_x - 1;
    }

    if (start_y < 0) {
        rect_height += start_y;
        start_y = 0;
    } else if (start_y + rect_height >= height) {
        rect_height = height - start_y - 1;
    }

    std::vector<sf::Uint8> pixels(rect_width * rect_height * 4);
    for(unsigned x = 0; x < rect_width; x++) {
        for(unsigned y = 0; y < rect_height; y++) {
            unsigned pos = 4 * (x + y * rect_width);
            sf::Color color = colors[amt.get(x + start_x, y + start_y)];
            pixels[pos    ] = color.r; // R
            pixels[pos + 1] = color.g; // G
            pixels[pos + 2] = color.b; // B
            pixels[pos + 3] = color.a; // A
        }
    } 
    texture.update(pixels.data(), rect_width, rect_height, start_x, start_y);
}

void Grid::set_position(sf::Vector2f const& position) {
    object.setPosition(position);
}

void Grid::set_scale(sf::Vector2f const& scale) {
    object.setScale(scale);
}

void Grid::set_rotation(float rotation) {
    object.setRotation(rotation);
}

sf::Vector2f Grid::get_position() const {
    return object.getPosition();
}

sf::Vector2f Grid::get_scale() const {
    return object.getScale();
}

float Grid::get_rotation() const {
    return object.getRotation();
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(object, states);
}

void Grid::define_color(unsigned state, sf::Color const& color) {
    colors[state] = color;
}

}