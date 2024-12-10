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
#include <functional>
#include <set>
#include "../helpers/grid.h"
#include "../helpers/aux.h"

using map_point_t = std::pair<IntVec2D, char>;
using neighbor_t = std::array<std::pair<IntVec2D, char>, 4>;

map_point_t extract_at(const CharGrid2D& grid, const IntVec2D& pos) {
    return map_point_t(pos, grid.at(pos));
}

neighbor_t get_neighbors(const CharGrid2D& grid, const IntVec2D& around_pos) {
    neighbor_t neighbors;
    neighbors[0] = extract_at(grid, around_pos - IntVec2D(-1,  0));
    neighbors[1] = extract_at(grid, around_pos - IntVec2D(+1,  0));
    neighbors[2] = extract_at(grid, around_pos - IntVec2D( 0, -1));
    neighbors[3] = extract_at(grid, around_pos - IntVec2D( 0, +1));
    return neighbors;
}

ulong find_ends(const CharGrid2D& map, const IntVec2D& current_pos, const ulong& current_height) {
    ulong cnt_ends = 0;
    const auto& neighbors = get_neighbors(map, current_pos);
    for(const auto& n : neighbors) {
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

    const CharGrid2D map = CharGrid2D::from_input(input);
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
    timed_execution(&parse, std::string_view((std::stringstream() << std::ifstream("sample.txt").rdbuf()).str()), "Example input");
    timed_execution(&parse, std::string_view((std::stringstream() << std::ifstream("puzzle.txt").rdbuf()).str()), "Puzzle input");
    return 0;
}  