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
#include "input.h"

long parse(const std::string_view input) {
    using std::string_view;
    using namespace std::views;
    std::vector<std::pair<long, long>> pairs;        
    bool is_enabled = true;
    for(auto line_subr : (input | split(string_view{"\n"}))) {
        if(line_subr.size() == 0) continue;
        string_view line = string_view{line_subr};
        for(uint i = 0; i < line.size(); i++) {
            if(std::string("do()").compare(line.substr(i, 4)) == 0) is_enabled = true;
            if(std::string("don't()").compare(line.substr(i, 7)) == 0) is_enabled = false;
            if(!is_enabled) continue;
            if(std::string("mul(").compare(line.substr(i, 4)) != 0) continue;
            std::pair<long, long> pair;
            auto substr_without_mul = line.substr(i+4);
            auto pos_comma = substr_without_mul.find(",");
            if(pos_comma == std::string::npos) continue;
            auto first_num_str = substr_without_mul.substr(0, pos_comma);
            auto [ptr1, ec1] = std::from_chars(first_num_str.begin(), first_num_str.end(), pair.first);
            (void)ptr1;
            if(ec1!= std::errc()) continue;
            if(ptr1 != first_num_str.end()) continue; //if not everything was parsed as number, skip
            auto substr_after_comma = substr_without_mul.substr(pos_comma+1);
            auto pos_right_bracket = substr_after_comma.find(")");
            if(pos_right_bracket == std::string::npos) continue;
            auto second_num_str = substr_after_comma.substr(0, pos_right_bracket);
            auto [ptr2, ec2] = std::from_chars(second_num_str.begin(), second_num_str.end(), pair.second);
            (void)ptr2;
            if(ec2!= std::errc()) continue;
            if(ptr2 != second_num_str.end()) continue; //if not everything was parsed as number, skip
            pairs.push_back(pair);
        }
    }
    int sum = 0;
    for(auto pair : pairs) {
        sum += pair.first * pair.second;
    }
    return sum;
}

int main() 
{
    int example_total = parse(example_input);
    std::cout << "example input total: " << example_total << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    int puzzle_total = parse(puzzle_input);
    std::cout << "puzzle input total: " << puzzle_total << std::endl;
    return 0;
}  