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
#include "input.h"

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

std::pair<long, long> calc_next_position(const std::pair<long, long>& guard_position, const dir d) {
    std::pair<long, long> next_position(guard_position);
    switch(d) {
        case UP:
            next_position.second--;
            break;
        case DOWN:
            next_position.second++;
            break;
        case LEFT:
            next_position.first--;
            break;
        case RIGHT:
            next_position.first++;
            break;
    }
    return next_position;
}

char look_at_next_step_on_map(const std::vector<std::vector<char>>& grid, const std::pair<long, long> guard_position, const dir d) {
    using std::pair;
    pair<long, long> next_position = calc_next_position(guard_position, d);
    if(next_position.first < 0) return 'X';
    if(next_position.second < 0) return 'Y';
    if((ulong)next_position.second >= grid.size()) return 'F';
    if((ulong)next_position.first >= grid[next_position.second].size()) return 'G';
    if(grid[next_position.second][next_position.first] == 'E') return 'E';
    if(grid[next_position.second][next_position.first] == '#') return '#';
    if(grid[next_position.second][next_position.first] == '.') return '.';
    return '?';
}

bool contains_loop(const std::vector<std::vector<char>>& grid, const std::pair<long, long>& guard_start_position, const dir d) {
    using std::vector;
    using std::tuple;
    using std::set;
    
    bool running = true;
    dir current_direction(d);
    std::pair<long, long> guard_current_position(guard_start_position);
    std::set<std::tuple<long, long, dir>> visited_positions_with_dir;
    while(running) {
        auto pdt = std::tuple(guard_current_position.first, guard_current_position.second, current_direction); //position and direction tuple
        if(visited_positions_with_dir.find(pdt) != visited_positions_with_dir.end()) return true; //guard was already at that position with direction, must be loop
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

    vector<vector<char>> grid(1000, vector<char>(1000, 'E')); // simply initiate a giant grid
    pair<long, long> guard_start_position{0, 0};

    pair<long, long> map_cursor{0, 0};
    for(const auto& line : input | split(string_view{"\n"})) {
        if(line.empty()) continue;
        for(char c : line) {
            if(c == '#') grid[map_cursor.second][map_cursor.first] = '#';
            if(c == '.') grid[map_cursor.second][map_cursor.first] = '.';
            if(c == '^') {
                guard_start_position.first = map_cursor.first;
                guard_start_position.second = map_cursor.second;
                grid[map_cursor.second][map_cursor.first] = '.';
            }
            map_cursor.first++;
        }
        map_cursor.second++;
        map_cursor.first = 0;
    }

    long cnt_looped_maps = 0;

    bool running = true;
    dir current_direction = UP;
    pair<long, long> guard_current_position(guard_start_position);
    while(running) { //Note: expecting that the base map does not loop
        auto next_step = look_at_next_step_on_map(grid, guard_current_position, current_direction);
        switch(next_step) {
            case '#':
                current_direction = turn_right(current_direction);
                break;
            case '.': {
                const auto next_position = calc_next_position(guard_current_position, current_direction);
                vector<vector<char>> grid_variation{grid};
                grid_variation[next_position.second][next_position.first] = '#';
                if(contains_loop(grid_variation, guard_start_position, UP)) cnt_looped_maps++;
                guard_current_position = next_position;
                break;
            }
            default:
                running = false;
                break;
        }
    }

    return cnt_looped_maps;
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