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

long parse(const std::string_view& input) {
    using std::string_view;
    using namespace std::views;
    using std::vector;
    using std::pair;

    vector<pair<long, long>> rules;
    vector<vector<long>> reorderings;

    {
        auto parts = input | split(string_view{"\n\n"});
        auto it = parts.begin();
        auto rule_listings = *it | split(string_view{"\n"});
        for(const auto& rule_listing : rule_listings) {
            if(rule_listing.empty()) continue;  // ignore empty lines
            pair<long, long> rule;
            long i = 0;
            for(const auto r : (rule_listing | split(string_view{"|"}))) {
                if(i == 0) 
                    if(std::from_chars(r.begin(), r.end(), rule.first).ec != std::errc()) return -2; 
                if(i == 1) 
                    if(std::from_chars(r.begin(), r.end(), rule.second).ec != std::errc()) return -3; 
                if(i > 1) 
                    return -4;
                i++;
            }
            rules.push_back(rule);
        }
        auto page_listings = *(++it) | split(string_view{"\n"});
        for(const auto& page_listing : page_listings) {
            if(page_listing.empty()) continue;  // ignore empty lines
            vector<long> page;
            for(const auto& page_string : page_listing | split(string_view{","})) {
                auto ps = string_view{page_string};
                long page_num;
                if(std::from_chars(ps.begin(), ps.end(), page_num).ec != std::errc()) return -1; 
                page.push_back(page_num);
            }
            reorderings.push_back(page);
        }
    }

    long sum = 0;
    for(const auto& pages : reorderings) {
        bool is_valid = true;
        for(const auto& rule : rules) {
            for(ulong i = 0; i < (pages.size() - 1); i++) {
                for(ulong j = i + 1; j < pages.size(); j++) {
                    if(pages[i] == rule.second && pages[j] == rule.first) { // rule not applied
                        is_valid = false;
                    }
                }
            }
        }
        if(is_valid) {
            sum += pages[(pages.size()) / 2];
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