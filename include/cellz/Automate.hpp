#pragma once

#include <functional>
#include <vector>
#include <map>
#include <optional>

#include <SFML/Graphics.hpp>

namespace cellz {

class Automate {
public:

    class CellLookup {
    public:
        struct Around {
            unsigned North, NorthEast, East, SouthEast, South, SouthWest, West, NorthWest;

            void for_each(std::function<void(unsigned)> const& f) const;

        };
        struct Neighbour {
            unsigned North, East, South, West;

            void for_each(std::function<void(unsigned)> const& f) const;
        };

        CellLookup(Automate const& app, unsigned id);

        unsigned current() const;
        Around arounds() const;
        Neighbour neighbours() const;
    
    private:
        Automate const& app;
        unsigned id;
        sf::Vector2i coords;
    };

    enum class WrapMode {
        None, Horizontal, Vertical, Both
    };


    Automate(unsigned cells_width_count, unsigned cells_height_count, unsigned starting_state);

    struct LangtonAnt {
        int x, y;
        int dir; // 0 North; 1 East; 2 South; 3 West
    };
    static Automate langton_ant(unsigned width, unsigned height, LangtonAnt& ant);
    static Automate game_of_life(unsigned width, unsigned height);

    void update_all();
    void update_selected(std::vector<sf::Vector2i> const& selected);

    void define_rule(unsigned state, std::function<unsigned(CellLookup const&)> rule);
    void define_wrap_mode(WrapMode mode, unsigned default_state = 0);

    unsigned get(int x, int y) const;
    void set(int x, int y, unsigned state);
    void set_all(unsigned state);
    void set_all(std::function<unsigned(int, int)> const& generator);

    unsigned get_width() const;
    unsigned get_height() const;

private:

    std::optional<sf::Vector2i> to_coords(unsigned id) const;
    std::optional<unsigned> from_coords(int x, int y) const;

    unsigned default_state;
    WrapMode mode;

    unsigned width, height;
    std::vector<unsigned> cells;

    std::map<unsigned, std::function<unsigned(CellLookup const&)>> rules;

};

}