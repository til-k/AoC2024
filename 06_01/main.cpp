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
    //std::cout << "Looking at next step: (" << next_position.first << ", " << next_position.second << ") " << grid[next_position.second][next_position.first] << std::endl;
    //std::cout << std::string_view{grid[next_position.second]} << std::endl;
    if(grid[next_position.second][next_position.first] == 'E') return 'E';
    if(grid[next_position.second][next_position.first] == '#') return '#';
    if(grid[next_position.second][next_position.first] == '.') return '.';
    return '?';
}

long parse(const std::string_view& input) {
    using std::string_view;
    using namespace std::views;
    using std::vector;
    using std::pair;

    vector<vector<char>> grid(1000, vector<char>(1000, 'E')); // simply initiate a giant grid
    pair<long, long> guard_position{0, 0};

    pair<long, long> map_cursor{0, 0};
    for(const auto& line : input | split(string_view{"\n"})) {
        if(line.empty()) continue;
        for(char c : line) {
            if(c == '#') grid[map_cursor.second][map_cursor.first] = '#';
            if(c == '.') grid[map_cursor.second][map_cursor.first] = '.';
            if(c == '^') {
                guard_position.first = map_cursor.first;
                guard_position.second = map_cursor.second;
                grid[map_cursor.second][map_cursor.first] = '.';
            }
            map_cursor.first++;
        }
        map_cursor.second++;
        map_cursor.first = 0;
    }

    bool running = true;
    dir current_direction = UP;
    std::set<pair<long, long>> visited_positions;
    while(running) {
        visited_positions.insert(guard_position);
        auto next_step = look_at_next_step_on_map(grid, guard_position, current_direction);
        //std::cout << "Guard position: (" << guard_position.first << ", " << guard_position.second << ")" << std::endl;
        //std::cout << "Next step: " << next_step << std::endl;
        //std::cout << "Current map position: " << grid[guard_position.second][guard_position.first] << std::endl;
        //std::cout << "-------------------------------------------" << std::endl;
        switch(next_step) {
            case '#':
                current_direction = turn_right(current_direction);
                break;
            case '.':
                guard_position = calc_next_position(guard_position, current_direction);
                break;
            default:
                running = false;
                break;
        }
    }

    return visited_positions.size();
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