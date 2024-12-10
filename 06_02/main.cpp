#include <string_view>
#include <vector>
#include <iostream>
#include <fstream>
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
#include "../helpers/grid.h"
#include "../helpers/aux.h"

enum dir{UP, DOWN, LEFT, RIGHT};

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
                if(unique_looped_maps.find(grid_variation) == unique_looped_maps.end()) //only attempt if maps not in the set already
                    if(contains_loop(grid_variation, guard_start_position, UP)) 
                        unique_looped_maps.insert(grid_variation);
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

int main() 
{
    timed_execution(&parse, std::string_view((std::stringstream() << std::ifstream("sample.txt").rdbuf()).str()), "Example input");
    timed_execution(&parse, std::string_view((std::stringstream() << std::ifstream("puzzle.txt").rdbuf()).str()), "Puzzle input");
    return 0;
}  