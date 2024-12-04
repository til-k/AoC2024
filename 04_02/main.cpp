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

using access_pattern_t = std::array<std::pair<long, long>, 3>;

class Field {
public:
    const std::vector<std::vector<char>>& data;
    Field(const std::vector<std::vector<char>>& data) : data(data) {}

    char try_at(const long x, const long y) const {
        return 
        (size_t)y < data.size() && (size_t)x < data[y].size() &&
        y >= 0 && x >= 0
        ? data[y][x] : '\0';
    }

    std::array<char, 3> retrieve_pattern(const access_pattern_t& pattern, const long x, const long y) const {
        std::array<char, 3> result;
        for(size_t i = 0; i < pattern.size(); ++i) {
            result[i] = try_at(x + pattern[i].first, y + pattern[i].second);
        }
        return result;
    }

    bool compare_pattern(const std::string& pattern, const long x, const long y, const long offset_x, const long offset_y) const {
        long at_x = x + offset_x;
        long at_y = y + offset_y;
        for(auto pattern_char : pattern) {
            switch (pattern_char) {
                case '.': {
                    at_x++;
                    break;
                }
                case '\n': {
                    at_y++; 
                    at_x = x + offset_x;
                    break;
                }
                default: {
                    if(try_at(at_x, at_y) != pattern_char) {
                        return false;
                    }
                    at_x++;
                    break;
                }
            }
        }
        return true;
    }
};

constexpr std::string pattern_1 = {
R"(M.S
.A.
M.S)"
};

constexpr std::string pattern_2 = {
R"(M.M
.A.
S.S)"
};

constexpr std::string pattern_3 = {
R"(S.M
.A.
S.M)"
};

constexpr std::string pattern_4 = {
R"(S.S
.A.
M.M)"
};

long count_xmas_pattern_around_pos(const Field& field, const long x, const long y) {
    long count = 0;
    if(field.compare_pattern(pattern_1, x, y, -1, -1)) count++;
    if(field.compare_pattern(pattern_2, x, y, -1, -1)) count++;
    if(field.compare_pattern(pattern_3, x, y, -1, -1)) count++;
    if(field.compare_pattern(pattern_4, x, y, -1, -1)) count++;
    return count;
}

long parse(const std::string_view& input) {
    using std::string_view;
    using namespace std::views;
    std::vector<std::vector<char>> field_data;
    for(auto line : input | split('\n')) {
        if(line.empty()) continue;  // ignore empty lines
        field_data.push_back(std::vector<char>(line.begin(), line.end()));
    }
    long sum = 0;
    Field field(field_data);
    for(size_t y = 0; y < field.data.size(); ++y) {
        for(size_t x = 0; x < field.data[y].size(); ++x) {
            sum += count_xmas_pattern_around_pos(field, x, y);
        }
    }

    return sum;
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