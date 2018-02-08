#include <cellz/Automate/Grid.hpp>

namespace cellz {

Grid::Grid() {

}

void Grid::update(Automate const& amt) {
    unsigned width = amt.get_width();
    unsigned height = amt.get_height();
    if (texture.getSize() != sf::Vector2u(width, height)) {
        texture.create(width, height);
        object.setTexture(texture);
    }

    std::vector<sf::Uint8> pixels(width * height * 4);
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
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

void Grid::define_color(unsigned state, sf::Color color) {
    colors[state] = color;
}

}