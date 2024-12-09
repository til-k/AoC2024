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
#include "input.h"

struct IntVec2D {
    long x = 0;
    long y = 0;
};

class CharGrid2D {
    private:
        std::vector<char> grid;
        IntVec2D dims;

        CharGrid2D(const std::string_view& input) {
            for(auto line : input | std::views::split('\n')) {
                if(line.empty()) continue;
                if(dims.x == 0) {
                    for(const char& c: line) {
                        dims.x++;
                        grid.push_back(c);
                    }
                }
                else {
                    for(const char& c: line) {
                        grid.push_back(c);
                    }
                }
                dims.y++;
            }
        }

    public:
        static CharGrid2D from_input(const std::string_view& input) {
            CharGrid2D grid(input);
            return grid;
        }

        char at(IntVec2D pos) const {
            if(pos.x < 0) return '\0';
            if(pos.y < 0) return '\0';
            if(pos.x >= dims.x) return '\0';
            if(pos.y >= dims.y) return '\0';
            return grid[pos.x + dims.x * pos.y];
        }
};

std::vector<std::string> to_svv(const std::string_view& input, const std::string_view& delim) {
    auto split_input = input | std::views::split(delim);
    std::vector<std::string> out;
    for(const auto& part : split_input) {
        out.push_back(std::string(part.data()));
    }
    return out;
}

long parse(const std::string_view& input) {
    using std::string_view;
    using namespace std::views;

    auto svv = to_svv(input, "\n");
    svv | std::views::transform([](std::string s) {
        return s + "s";
    });

    return input.size();
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