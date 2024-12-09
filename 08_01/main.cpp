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

        static CharGrid2D from_input(const std::string_view& input) {
            CharGrid2D grid(input);
            return grid;
        }

        IntVec2D& get_dims() {
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

        void dbg_print() {
            std::cout << "Dimensions: x: " << dims.x << ", y: " << dims.y << std::endl;
            for(long y = 0; y < dims.y; y++) {
                for(long x = 0; x < dims.x; x++) {
                    std::cout << at(IntVec2D(x, y));
                }
                std::cout << std::endl;
            }
        }
};

long parse(const std::string_view& input) {
    using std::string_view;
    using namespace std::views;

    auto antenna_grid = CharGrid2D::from_input(input);
    auto antinode_grid = CharGrid2D(antenna_grid.get_dims(), '.');

    std::vector<std::vector<IntVec2D>> frequency_register(256, std::vector<IntVec2D>());
    
    antenna_grid.iterate_over([&frequency_register](const IntVec2D& pos, const char& c) {
        if(c != '.' && c != '\0') {
            frequency_register[c].push_back(pos);
        }
    });

    //long cnt_antinodes = 0;
    for(const auto& antenna_positions: frequency_register) {
        auto view = std::views::all(antenna_positions) | drop(0);
        auto n = 0;
        while(!(view | drop(n)).empty()) {
            auto dview = view | drop(n);
            n++;
            auto a1 = *(dview | take(1)).begin();
            for(const auto& a2 : dview | drop(1)) {
                IntVec2D a = a1 - (a2 - a1);
                IntVec2D b = a2 + (a2 - a1);
                antinode_grid.place(a, '#');
                antinode_grid.place(b, '#');
                /*
                //TODO: still dont understand how this miscounts
                auto replaced1 = antinode_grid.place(a, '#');
                auto replaced2 = antinode_grid.place(b, '#');
                if(replaced1 == '.' || replaced2 == '.') cnt_antinodes++;*/
            }
        }
    }
    long cnt = 0;
    antinode_grid.iterate_over([&cnt] (const auto pos, const char c) {
        (void)pos;
        if(c == '#') cnt++;
    });

    return cnt;
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