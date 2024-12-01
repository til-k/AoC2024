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

    std::sort(numbers_left.begin(), numbers_left.end());
    std::sort(numbers_right.begin(), numbers_right.end());

    int similarity_score = 0;
    for (auto num : numbers_left)
    {
        for (auto num2 : numbers_right) {
            if (num == num2) {
                similarity_score += num;
            }
        }
    }

    std::cout << "Similarity score: " << similarity_score << std::endl;

    return 0; 
}
