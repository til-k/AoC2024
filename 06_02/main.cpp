#include <string_view>
#include <vector>
#include <iostream>
#include <ranges>
#include <charconv>
#include <algorithm>
#include <numeric>
#include <tuple>
#include <queue>
#include <thread>
#include <chrono>
#include <set>
#include <functional>
#include "input.h"

enum dir{UP, DOWN, LEFT, RIGHT};

struct IntVec2D {
    long x = 0;
    long y = 0;

    IntVec2D operator+(const IntVec2D& other) const {
        return IntVec2D(x + other.x, y + other.y);
    }

    IntVec2D operator-(const IntVec2D& other) const {
        return IntVec2D(x - other.x, y - other.y);
    }

    IntVec2D operator*(const auto& scalar) const {
        return IntVec2D(x * scalar, y * scalar);
    }

    bool operator==(const IntVec2D& other) const {
        return (x == other.x && y == other.y);
    }

    bool operator<(const IntVec2D& other) const {
        return (x < other.x && y <= other.y);
    }

    void dbg_print() const{
        std::cout << "x: " << x << ", y: " << y << std::endl;
    }
};

class CharGrid2D {
    private:
        std::vector<char> grid;
        IntVec2D dims;

        CharGrid2D(const std::string_view& input) {
            for(auto line : input | std::views::split('\n')) {
                if(line.empty()) continue;
                for(const char& c: line) {
                    grid.push_back(c);
                }
                dims.y++;
            }
            if(dims.y > 0)
                dims.x = grid.size() / dims.y;
        }

    public:
        CharGrid2D(const IntVec2D& dims, const char& initializing_character) {
            this->dims = dims;
            this->grid = std::vector<char>(dims.x * dims.y, initializing_character);
        }

        bool operator==(const CharGrid2D& other) const {
            return dims==other.dims && grid==other.grid;
        }

        bool operator<(const CharGrid2D& other) const {
            if(dims!=other.dims) return dims < other.dims;
            else return grid < other.grid;
        }

        static CharGrid2D from_input(const std::string_view& input) {
            CharGrid2D grid(input);
            return grid;
        }

        IntVec2D get_dims() const {
            return dims;
        }

        void iterate_over(const std::function<void(const IntVec2D& pos, const char& c)> callback) {
            for(long x = 0; x < dims.x; x++) {
                for(long y = 0; y < dims.y; y++) {
                    auto pos = IntVec2D(x, y);
                    callback(pos, at(pos));
                }
            }
        }

        char at(IntVec2D pos) const {
            if(pos.x < 0) return '\0';
            if(pos.y < 0) return '\0';
            if(pos.x >= dims.x) return '\0';
            if(pos.y >= dims.y) return '\0';
            return grid[pos.x + dims.x * pos.y];
        }

        char place(IntVec2D pos, char c) {
            if(pos.x < 0) return '\0';
            if(pos.y < 0) return '\0';
            if(pos.x >= dims.x) return '\0';
            if(pos.y >= dims.y) return '\0';
            char current_char = grid[pos.x + dims.x * pos.y];
            grid[pos.x + dims.x * pos.y] = c;
            return current_char;
        }

        void dbg_print() const {
            std::cout << "Dimensions: x: " << dims.x << ", y: " << dims.y << std::endl;
            for(long y = 0; y < dims.y; y++) {
                for(long x = 0; x < dims.x; x++) {
                    std::cout << at(IntVec2D(x, y));
                }
                std::cout << std::endl;
            }
        }
};

dir turn_right(dir direction) {
    switch(direction) {
        case UP:
            return RIGHT;
        case RIGHT:
            return DOWN;
        case DOWN:
            return LEFT;
        case LEFT:
            return UP;
    }
    return direction;
}

IntVec2D calc_next_position(const IntVec2D& guard_position, const dir d) {
    IntVec2D next_position(guard_position);
    switch(d) {
        case UP:
            next_position.y--;
            break;
        case DOWN:
            next_position.y++;
            break;
        case LEFT:
            next_position.x--;
            break;
        case RIGHT:
            next_position.x++;
            break;
    }
    return next_position;
}

char look_at_next_step_on_map(const CharGrid2D& grid, const IntVec2D guard_position, const dir d) {
    IntVec2D next_position = calc_next_position(guard_position, d);
    return grid.at(next_position);
}

bool contains_loop(const CharGrid2D& grid, const IntVec2D& guard_start_position, const dir d) {
    using std::vector;
    using std::tuple;
    using std::set;
    
    bool running = true;
    dir current_direction(d);
    IntVec2D guard_current_position(guard_start_position);
    std::set<std::tuple<long, long, dir>> visited_positions_with_dir;
    while(running) {
        auto pdt = std::tuple(guard_current_position.x, guard_current_position.y, current_direction); //position and direction tuple
        if(visited_positions_with_dir.find(pdt) != visited_positions_with_dir.end()) { //guard was already at that position with direction, must be loop
            return true;
        }
        visited_positions_with_dir.insert(pdt);
        auto next_step = look_at_next_step_on_map(grid, guard_current_position, current_direction);
        switch(next_step) {
            case '#':
                current_direction = turn_right(current_direction);
                break;
            case '.':
                guard_current_position = calc_next_position(guard_current_position, current_direction);
                break;
            default:
                running = false;
                break;
        }
    }
    return false;
}

long parse(const std::string_view& input) {
    using std::string_view;
    using namespace std::views;
    using std::vector;
    using std::pair;

    CharGrid2D grid = CharGrid2D::from_input(input); // simply initiate a giant grid
    IntVec2D guard_start_position;
    grid.iterate_over([&guard_start_position](const IntVec2D& pos, const char& c) {
        if(c == '^') {
            guard_start_position = pos;
        }
    });
    grid.place(guard_start_position, '.');

    bool running = true;
    dir current_direction = UP;
    IntVec2D guard_current_position(guard_start_position);
    std::set<CharGrid2D> unique_looped_maps;
    while(running) { //Note: expecting that the base map does not loop
        auto next_step = look_at_next_step_on_map(grid, guard_current_position, current_direction);
        switch(next_step) {
            case '#':
                current_direction = turn_right(current_direction);
                break;
            case '.': {
                const auto next_position = calc_next_position(guard_current_position, current_direction);
                CharGrid2D grid_variation(grid);
                grid_variation.place(next_position, '#');
                if(contains_loop(grid_variation, guard_start_position, UP)) unique_looped_maps.insert(grid_variation);
                guard_current_position = next_position;
                break;
            }
            default:
                running = false;
                break;
        }
    }

    return unique_looped_maps.size();
}

void timed_execution(const std::string_view& input, const std::string_view& title) {
    auto start = std::chrono::system_clock::now();
    int result = parse(input);
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << title << std::endl;
    std::cout << "Result: " << result << std::endl;
    std::cout << "Time needed: " << elapsed.count() << std::endl;
}

int main() 
{
    timed_execution(example_input, "Example input");
    std::cout << "-------------------------------------------" << std::endl;
    timed_execution(puzzle_input, "Puzzle input");

    return 0;
}  