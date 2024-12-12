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
#include <list>
#include <cmath>
#include "../helpers/grid.h"
#include "../helpers/aux.h"

long num_digits(const long& in) {
    return (long)std::floor(std::log10(in) + 1);
}

long rule_one(const long& stone) {
    if(stone == 0) return 1;
    return -1;
}

std::pair<long, long> rule_two(const long& stone) {
    if(0 == (num_digits(stone) % 2)) {
        const auto s = std::to_string(stone);
        const auto sv = std::string_view{s};
        const auto sv_stones_1 = sv.substr(0, sv.size() / 2);
        const auto sv_stones_2 = sv.substr((sv.size() / 2), sv.size());
        auto return_pair = std::pair<long, long>{};
        std::from_chars(sv_stones_1.begin(), sv_stones_1.end(), return_pair.first);
        std::from_chars(sv_stones_2.begin(), sv_stones_2.end(), return_pair.second);
        return return_pair;
    } 
    return std::pair<long, long>{-1, -1};
}

long rule_three(const long& stone) {
    return stone * 2024;
}

long parse(const std::string_view& input) {
    using std::string_view;
    using namespace std::views;

    const auto& nums = to_svv(input, " ");
    std::vector<long> stones;

    //initial filling
    for(const auto& nsv: nums) {
        auto& new_stone = stones.emplace_back();
        std::from_chars(nsv.cbegin(), nsv.end(), new_stone);
    }

    //each loop iteration is a blink
    for(int i = 0; i < 25; i++) {
        std::vector<long> new_stones;
        for(auto stone : stones) {
            if(const auto& retval = rule_one(stone); retval != -1) 
                new_stones.push_back(retval);
            else if(const auto& retval = rule_two(stone); retval.first != -1) 
                new_stones.push_back(retval.first), new_stones.push_back(retval.second);
            else if(const auto& retval = rule_three(stone); retval != -1) 
                new_stones.push_back(retval);
        }
        stones = new_stones;
    }

    return stones.size();
}

int main() 
{
    timed_execution(&parse, std::string_view((std::stringstream() << std::ifstream("sample.txt").rdbuf()).str()), "Example input");
    timed_execution(&parse, std::string_view((std::stringstream() << std::ifstream("puzzle.txt").rdbuf()).str()), "Puzzle input");
    return 0;
}  