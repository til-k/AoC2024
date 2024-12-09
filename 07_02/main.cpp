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
#include <cmath>
#include "input.h"

long calc(const std::vector<long>& terms, const std::vector<char>& operands) {
    long sum = terms[0];
    for(ulong i = 0; i < operands.size(); i++) {
        if(operands[i] == '+') sum += terms[i + 1];
        else if(operands[i] == '|') sum = sum * std::pow(10, (long)std::floor(std::log10(terms[i + 1]) + 1)) + terms[i + 1];
        else if(operands[i] == '*') sum = sum * terms[i + 1];
    }
    return sum;
}

bool done(std::vector<char>& operands) {
    return std::all_of(operands.begin(), operands.end(), [](const char c) {return c == '*';});
}

std::vector<char> permutate(std::vector<char>& operands) {
    std::vector<char> permutated_operands(operands);
    bool carry = true;
    for(long i = permutated_operands.size() - 1; i >= 0; i--) {
        if(carry) {
            if(permutated_operands[i] == '+') {
                permutated_operands[i] = '|';
                carry = false;
            }
            else if(permutated_operands[i] == '|') {
                permutated_operands[i] = '*';
                carry = false;
            }
            else if(permutated_operands[i] == '*') {
                permutated_operands[i] = '+';
                carry = true;
            }
        }
    }
    return permutated_operands;
}

bool is_possible(const long test_value, const std::vector<long>& terms) {
    std::vector<char> operands(terms.size()-1, '+');
    while(true) {
        if(test_value == calc(terms, operands)) return true;
        operands = permutate(operands);
        if(done(operands)) {
            if(test_value == calc(terms, operands)) return true;
            else break;
        }
    }
    return false;
}

long parse(const std::string_view& input) {
    using sv = std::string_view;
    using namespace std::views;
    using namespace std::ranges;
    using std::vector;

    long sum = 0;
    auto splits = input | split(sv("\n")) | filter([](const auto& line) { return !line.empty(); });
    for_each(splits, [&sum](const auto& line) {
        auto parts = line | std::views::split(sv(":"));
        auto test_str = parts | std::views::take(1);
        long test_value;
        std::from_chars(sv(*test_str.begin()).begin(), sv(*test_str.begin()).end(), test_value);
        vector<long> terms;
        auto term_strs = sv(*(parts | std::views::drop(1)).begin()) | std::views::split(sv{" "}) | std::views::filter([](const auto& term_str) { return !term_str.empty(); });
        std::ranges::for_each(term_strs, [&terms](const auto& t_str) {
            long term = 0;
            std::from_chars(sv(t_str).begin(), sv(t_str).end(), term);
            terms.push_back(term);
        });
        if(is_possible(test_value, terms)) {
            sum += test_value;
        }
    });
    return sum;
}

void timed_execution(const std::string_view& input, const std::string_view& title) {
    auto start = std::chrono::system_clock::now();
    long result = parse(input);
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