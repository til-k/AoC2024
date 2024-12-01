#include <string_view>
#include <vector>
#include <iostream>
#include <ranges>
#include <charconv>
#include <algorithm>
#include <numeric>
#include <tuple>
#include "input.h"

int main() { 
    std::string_view input = puzzle_input_literal;

    std::vector<int> numbers_left;
    std::vector<int> numbers_right;

    for (auto line : (input | std::views::split('\n'))) {
        if (line.size() == 0) continue;
        int part_cnt = 0;
        for (auto part_str : (std::string_view{line} | std::views::split(' '))) {
            auto part_str_view = std::string_view{part_str};
            if (part_str_view.size() == 0) continue;
            int number;
            std::from_chars(part_str_view.data(), part_str_view.data() + part_str_view.size(), number);
            if(part_cnt == 0) numbers_left.push_back(number);
            else if(part_cnt == 1) numbers_right.push_back(number);
            else {return -1;} // invalid input, more than two parts per line
            part_cnt++;
        }
    }

    std::cout << numbers_left.size() << " numbers in the left list" << std::endl;
    std::cout << numbers_right.size() << " numbers in the right list" << std::endl;

    //std sort numbers_left and numbers_right
    std::sort(numbers_left.begin(), numbers_left.end());
    std::sort(numbers_right.begin(), numbers_right.end());

    int dist_sum = 0;
    // find the differences between numbers in both lists
    for (size_t i = 0; i < std::min(numbers_left.size(), numbers_right.size()); ++i) {
        dist_sum += std::abs(numbers_left[i] - numbers_right[i]);
    }

    std::cout << "Sum of differences: " << dist_sum << std::endl;

    return 0; 
}
