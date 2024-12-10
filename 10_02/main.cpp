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
#include <functional>
#include <set>
#include "../helpers/grid.h"
#include "../helpers/aux.h"
#include "input.h"

using map_point_t = std::pair<IntVec2D, char>;
using neighbor_t = std::array<std::pair<IntVec2D, char>, 4>;
neighbor_t get_neighbors(const CharGrid2D& grid, const IntVec2D& around_pos) {
    neighbor_t neighbors;
    neighbors[0] = std::pair(IntVec2D(around_pos.x - 1, around_pos.y), grid.at(IntVec2D(around_pos.x - 1, around_pos.y)));
    neighbors[1] = std::pair(IntVec2D(around_pos.x + 1, around_pos.y), grid.at(IntVec2D(around_pos.x + 1, around_pos.y)));
    neighbors[2] = std::pair(IntVec2D(around_pos.x, around_pos.y - 1), grid.at(IntVec2D(around_pos.x, around_pos.y - 1)));
    neighbors[3] = std::pair(IntVec2D(around_pos.x, around_pos.y + 1), grid.at(IntVec2D(around_pos.x, around_pos.y + 1)));
    return neighbors;
}

ulong find_ends(const CharGrid2D& map, const IntVec2D& current_pos, const ulong& current_height) {
    ulong cnt_ends = 0;
    auto neighbors = get_neighbors(map, current_pos);
    for(auto n : neighbors) {
        if(n.second != '\0') {
            ulong n_h = n.second - '0';
            if(current_height == 8 && n_h == 9) cnt_ends++;
            else if((current_height + 1) == n_h) cnt_ends += find_ends(map, n.first, n_h);
        }
    }
    return cnt_ends;
}

long parse(const std::string_view& input) {
    using std::string_view;
    using namespace std::views;

    CharGrid2D map = CharGrid2D::from_input(input);
    ulong trailhead_score = 0;
    map.iterate_over([map, &trailhead_score](const IntVec2D& pos, const char& c){
        ulong height = c - '0';
        if(height == 0) {
            trailhead_score += find_ends(map, pos, height);
        } 
    });

    return trailhead_score;
}

int main() 
{
    timed_execution(&parse, example_input, "Example input");
    timed_execution(&parse, puzzle_input, "Puzzle input");
    return 0;
}  