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

    std::vector<uint32_t> blocks;

    ulong current_file = 0;
    bool is_file = true;
    for(const char& c: input) {
        if(c == '\n') continue;
        //assume all other input is just numbers and valid
        ulong number = c - '0';
        if(is_file) {
            for(ulong i = 0; i < number; i++) blocks.push_back(current_file);
            current_file++; 
        }
        else{
            for(ulong i = 0; i < number; i++) blocks.push_back(UINT32_MAX);
        }
        is_file = !is_file;
    }

    ulong forward_cursor = 0;
    ulong backward_cursor = blocks.size() - 1;
    while(forward_cursor < backward_cursor) {
        if(blocks[forward_cursor] == UINT32_MAX) {
            while(blocks[backward_cursor] == UINT32_MAX) backward_cursor--;
            blocks[forward_cursor] = blocks[backward_cursor];
            blocks[backward_cursor] = UINT32_MAX;
            backward_cursor--;
        }
        forward_cursor++;
    }

    long sum = 0;
    for(ulong i = 0; i < blocks.size(); i++) {
        if(blocks[i] != UINT32_MAX) {
            sum += i * blocks[i];
        }
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