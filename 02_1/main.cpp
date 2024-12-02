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

void calculate_derivatives(const std::string_view& raw_input, std::queue<std::vector<int>>& derivatives) {
    for (auto line : (raw_input | std::views::split('\n'))) {
        if (line.size() == 0) continue;
        std::vector<int> numbers;        
        for (auto part_str : (std::string_view{line} | std::views::split(' '))) {
            auto part_str_view = std::string_view{part_str};
            if (part_str_view.size() == 0) continue;
            int number;
            std::from_chars(part_str_view.data(), part_str_view.data() + part_str_view.size(), number);
            numbers.push_back(number);
        }
        std::vector<int> derivative(numbers.size() - 1);
        for (size_t i = 1; i < numbers.size(); ++i) {
            derivative[i - 1] = numbers[i] - numbers[i - 1];
        }
        derivatives.push(derivative);
    }
}

void parse_derivatives(std::queue<std::vector<int>>& derivatives, int& cnt_safe_reports) {
    while(!derivatives.empty()) {
        std::vector<int> derivative = derivatives.front();
        derivatives.pop();
        if(derivative.size() == 1) continue; // ignore single-element derivatives
        bool safe_report = true;
        for(uint i = 0; i < derivative.size(); i++) {
            if(derivative[i] > 3 || derivative[i] < -3) safe_report = false;
            if(i > 0)
                if(derivative[i] * derivative[i - 1] <= 0) safe_report = false; // compare sign
        }
        if(safe_report) cnt_safe_reports++;
    }
}

int main() { 
    std::string_view input = puzzle_input_literal;

    int cnt_safe_reports = 0;
    std::queue<std::vector<int>> derivatives;
    calculate_derivatives(input, derivatives);
    parse_derivatives(derivatives, cnt_safe_reports);

    std::cout << "Count of safe reports: " << cnt_safe_reports << std::endl;

    return 0; 
}
