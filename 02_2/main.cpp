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

void to_numbers(const std::string_view& line, std::vector<int>& numbers) {
    if (line.size() == 0) return;
    for (auto part_str : (std::string_view{line} | std::views::split(' '))) {
        auto part_str_view = std::string_view{part_str};
        if (part_str_view.size() == 0) continue;
        int number;
        std::from_chars(part_str_view.data(), part_str_view.data() + part_str_view.size(), number);
        numbers.push_back(number);
    }
}

void calculate_derivatives(const std::vector<std::vector<int>>& variations, std::vector<std::vector<int>>& derivatives) {
    for(const auto& numbers : variations) {
        if(numbers.size() < 2) return;
        std::vector<int> derivative(numbers.size() - 1);
        for (size_t i = 1; i < numbers.size(); ++i) {
            derivative[i - 1] = numbers[i] - numbers[i - 1];
        }
        derivatives.push_back(derivative);
    }
}

bool is_report_safe(const std::vector<int>& derivative) {
    for(uint i = 0; i < derivative.size(); i++) {
        if(derivative[i] > 3 || derivative[i] < -3) return false;
        if(i > 0)
            if(derivative[i] * derivative[i - 1] <= 0) return false; // compare sign
    }
    return true;
}

void create_variations(const std::vector<int>& input, std::vector<std::vector<int>>& variations) {
    std::vector<int> first_variation = input;
    variations.push_back(first_variation);
    for(uint i = 0; i < input.size(); i++) {
        std::vector<int> new_variation = input;
        new_variation.erase(new_variation.begin() + i);
        variations.push_back(new_variation);
    }
}

void eval_derivatives(std::vector<std::vector<int>>& variations, bool& is_safe_overall) {
    is_safe_overall = false;
    for(const auto& variation : variations) {
        is_safe_overall |= is_report_safe(variation);
    }
}

int main() { 
    std::string_view input = puzzle_input_literal;

    int cnt_safe_reports = 0;
    for (auto line : (input | std::views::split('\n'))) {
        std::vector<int> numbers;
        to_numbers(std::string_view{line}, numbers);
        if(numbers.size() == 0) continue; // invalid input, no numbers found
        std::vector<std::vector<int>> variations;
        create_variations(numbers, variations);
        std::vector<std::vector<int>> derivatives;
        calculate_derivatives(variations, derivatives);
        bool is_safe = false;
        eval_derivatives(derivatives, is_safe);
        if(is_safe) cnt_safe_reports++;
    }

    std::cout << "Count of safe reports: " << cnt_safe_reports << std::endl;

    return 0; 
}
