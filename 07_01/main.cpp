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

long calc(const std::vector<long>& terms, std::vector<char>& operands) {
    long sum = terms[0];
    for(ulong i = 0; i < operands.size(); i++) {
        if(operands[i] == '+') sum += terms[i + 1];
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
    using std::string_view;
    using namespace std::views;
    using std::vector;

    long sum = 0;
    for(const auto& line : input | split(string_view{"\n"})) {
        if(line.empty()) continue;
        vector<string_view> parts;
        for(const auto& part: string_view(line) | split(string_view{":"})) {
            parts.push_back(string_view(part));
        }
        if(parts.size() != 2) return -1; //should never happen, error out
        long test_value;
        if(std::from_chars(parts[0].begin(), parts[0].end(), test_value).ec != std::errc()) return -2;
        vector<long> terms;
        for(const auto& t_str: parts[1] | split(string_view{" "})) {
            if(t_str.empty()) continue;
            const auto t_sv = string_view(t_str);
            long term;
            if(std::from_chars(t_sv.begin(), t_sv.end(), term).ec != std::errc()) return -3;
            terms.push_back(term);
        }
        if(terms.size() < 2) return -4; //should never happen, error out
        if(is_possible(test_value, terms)) sum += test_value;
    }

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