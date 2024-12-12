#include <string_view>
#include <vector>
#include <iostream>
#include <fstream>
#include <ranges>
#include <charconv>
#include <algorithm>
#include <numeric>
#include <tuple>
#include <future>
#include <thread>
#include <chrono>
#include <functional>
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
    if(auto nd = num_digits(stone); 0 == (nd % 2)) {
        auto f = (long)std::round(std::pow(10, nd / 2));
        return std::pair<long, long>{stone / f, stone % f};
    } 
    return std::pair<long, long>{-1, -1};
}

long rule_three(const long& stone) {
    return stone * 2024;
}   

std::array<std::array<long, 256>, 65536> lookup_table{};

template <long max_evolutions, long max_threads, long max_stones_before_split>
long stone_evolution(std::unique_ptr<std::vector<long>> stones, const long& current_evolution, const long& num_threads) {
    if(current_evolution == max_evolutions) {
        return stones->size();
    } else {
        long overall_cnt = 0;  
        for(const auto& stone: *stones) {
            bool lookup_successful = false;
            if(stone < 65536) {
                auto tree_depth = max_evolutions - current_evolution;
                if(lookup_table[stone][tree_depth] > 0) {
                    overall_cnt += lookup_table[stone][tree_depth];
                    lookup_successful = true;
                }
            }
            if(!lookup_successful) {            
                if(const auto& retval = rule_one(stone); retval != -1) {
                    std::vector<long> s{retval};
                    auto cnt = stone_evolution<max_evolutions, max_threads, max_stones_before_split>(std::make_unique<std::vector<long>>(s), current_evolution + 1, num_threads + 1);
                    if(stone < 65536) {
                        auto tree_depth = max_evolutions - current_evolution;
                        lookup_table[stone][tree_depth] = cnt;
                    }
                    overall_cnt += cnt;
                } else if(const auto& retval = rule_two(stone); retval.first != -1) {
                    std::vector<long> s{retval.first, retval.second};
                    auto cnt = stone_evolution<max_evolutions, max_threads, max_stones_before_split>(std::make_unique<std::vector<long>>(s), current_evolution + 1, num_threads + 1);
                    if(stone < 65536) {
                        auto tree_depth = max_evolutions - current_evolution;
                        lookup_table[stone][tree_depth] = cnt;
                    }
                    overall_cnt += cnt;
                } else {
                    std::vector<long> s{rule_three(stone)};
                    auto cnt = stone_evolution<max_evolutions, max_threads, max_stones_before_split>(std::make_unique<std::vector<long>>(s), current_evolution + 1, num_threads + 1);
                    if(stone < 65536) {
                        auto tree_depth = max_evolutions - current_evolution;
                        lookup_table[stone][tree_depth] = cnt;
                    }
                    overall_cnt += cnt;
                }
            }
        }
        return overall_cnt;
    }
}

long parse(const std::string_view& input) {
    using std::string_view;
    using namespace std::views;
    using namespace std::chrono;

    const auto& nums = to_svv(input, " ");
    auto stones = std::vector<long>();
    //initial filling
    for(const auto& nsv: nums) {
        auto& new_stone = stones.emplace_back();
        std::from_chars(nsv.cbegin(), nsv.end(), new_stone);
    }

    return stone_evolution<75, 8, 50000>(std::make_unique<std::vector<long>>(stones), 0, 1);
}

int main() 
{
    timed_execution(&parse, std::string_view((std::stringstream() << std::ifstream("sample.txt").rdbuf()).str()), "Example input");
    timed_execution(&parse, std::string_view((std::stringstream() << std::ifstream("puzzle.txt").rdbuf()).str()), "Puzzle input");
    return 0;
}  