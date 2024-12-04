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

    bool compare_pattern_with(const std::array<char, 3>& compare_chars, const access_pattern_t& pattern, const long x, const long y) const {
        const auto retrieved_pattern = retrieve_pattern(pattern, x, y);
        return retrieved_pattern[0] == compare_chars[0] && retrieved_pattern[1] == compare_chars[1] && retrieved_pattern[2] == compare_chars[2];
    }
};

const access_pattern_t upward_pattern{std::pair(0,-1), std::pair(0,-2), std::pair(0,-3)};
const access_pattern_t upleft_pattern{std::pair(-1,-1), std::pair(-2,-2), std::pair(-3,-3)};
const access_pattern_t leftward_pattern{std::pair(-1,0), std::pair(-2,0), std::pair(-3,0)};
const access_pattern_t downleft_pattern{std::pair(-1,1), std::pair(-2,2), std::pair(-3,3)};
const access_pattern_t downward_pattern{std::pair(0,1), std::pair(0,2), std::pair(0,3)};
const access_pattern_t downright_pattern{std::pair(1,1), std::pair(2,2), std::pair(3,3)};
const access_pattern_t rightward_pattern{std::pair(1,0), std::pair(2,0), std::pair(3,0)};
const access_pattern_t upright_pattern{std::pair(1,-1), std::pair(2,-2), std::pair(3,-3)};

long count_xmas_pattern_around_pos(const Field& field, const long x, const long y) {
    long count = 0;
    //std::cout << "Checking at: " << x << ", " << y << " " << field.try_at(x, y) << std::endl;
    if(field.try_at(x, y) == 'X') { //only if char at position fits pattern try the full matching
        std::array<char, 3> compare_chars{'M', 'A', 'S'};    
        if(field.compare_pattern_with(compare_chars, upward_pattern, x, y)) ++count;
        if(field.compare_pattern_with(compare_chars, upleft_pattern, x, y)) ++count;
        if(field.compare_pattern_with(compare_chars, leftward_pattern, x, y)) ++count;
        if(field.compare_pattern_with(compare_chars, downleft_pattern, x, y)) ++count;
        if(field.compare_pattern_with(compare_chars, downward_pattern, x, y)) ++count;
        if(field.compare_pattern_with(compare_chars, downright_pattern, x, y)) ++count;
        if(field.compare_pattern_with(compare_chars, rightward_pattern, x, y)) ++count;
        if(field.compare_pattern_with(compare_chars, upright_pattern, x, y)) ++count;
    }
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