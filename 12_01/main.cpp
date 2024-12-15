
#include <string_view>
#include <iostream>
#include <fstream>
#include <ranges>
#include <vector>
#include <set>
#include <tuple>
#include <chrono>
#include <optional>
#include "../helpers/grid.h"
#include "../helpers/aux.h"

struct Fruit {
    IntVec2D pos;
    char type;
};

std::array<Fruit, 4> get_neighbors(const CharGrid2D& grid, const IntVec2D& around_pos) {
    std::array<Fruit, 4> neighbors;
    neighbors[0] = Fruit(IntVec2D(around_pos.x - 1, around_pos.y), grid.at(IntVec2D(around_pos.x - 1, around_pos.y)));
    neighbors[1] = Fruit(IntVec2D(around_pos.x + 1, around_pos.y), grid.at(IntVec2D(around_pos.x + 1, around_pos.y)));
    neighbors[2] = Fruit(IntVec2D(around_pos.x, around_pos.y - 1), grid.at(IntVec2D(around_pos.x, around_pos.y - 1)));
    neighbors[3] = Fruit(IntVec2D(around_pos.x, around_pos.y + 1), grid.at(IntVec2D(around_pos.x, around_pos.y + 1)));
    return neighbors;
}

std::pair<long, long> recursively_count_field(CharGrid2D& garden, const Fruit& fruit, std::set<IntVec2D>& field_positions) {
    auto sum_area = 1;
    auto sum_perimeter = 0;
    //garden.place(fruit.pos, '.');
    field_positions.insert(fruit.pos);
    for(const auto& n : get_neighbors(garden, fruit.pos)) {
        if(field_positions.find(n.pos) == field_positions.end()) {
            if(n.type == fruit.type) {
                auto [area, perimeter] = recursively_count_field(garden, n, field_positions);
                sum_area += area;
                sum_perimeter += perimeter;
            } else {
                sum_perimeter++;
            }
        }
    }
    return std::pair(sum_area, sum_perimeter);
}

std::optional<Fruit> find_any_fruit(const CharGrid2D& grid) {
    std::optional<Fruit> optf;
    grid.iterate_over([&optf](auto pos, auto c){
        if(c != '.') optf = Fruit(pos, c);
    });
    return optf;
}

long parse(const std::string_view& input) {
    using std::string_view;
    using namespace std::views;
    using namespace std::chrono;

    CharGrid2D garden(input);
    
    long sum = 0;
    while(true) {
        if(auto optf = find_any_fruit(garden)){
            std::set<IntVec2D> field_positions;
            auto [area, perimeter] = recursively_count_field(garden, optf.value(), field_positions); 
            sum+=area*perimeter;
            for(const auto& pos : field_positions) garden.place(pos, '.');
        }
        else break;
    }
    
    return sum;
}

int main() 
{
    timed_execution(&parse, std::string_view((std::stringstream() << std::ifstream("sample.txt").rdbuf()).str()), "Example input");
    timed_execution(&parse, std::string_view((std::stringstream() << std::ifstream("puzzle.txt").rdbuf()).str()), "Puzzle input");
    return 0;
}  