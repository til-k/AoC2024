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
#include "input.h"

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
        return std::pair(x,y) < std::pair(other.x, other.y);
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

std::array<std::pair<IntVec2D, char>, 4> get_neighbors(const CharGrid2D& grid, const IntVec2D& around_pos) {
    std::array<std::pair<IntVec2D, char>, 4> neighbors;
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

void timed_execution(const std::string_view& input, const std::string_view& title) {
    auto start = std::chrono::system_clock::now();
    long result = parse(input);
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