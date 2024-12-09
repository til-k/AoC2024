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
    using fd = std::pair<uint32_t, uint8_t>; // fd = file descriptor

    //first is number of file, second is section size in number of blocks
    std::vector<fd> blocks;
    /*auto dbg_print = [&blocks]() {
        if(!blocks.empty()) {
            for(auto i: blocks) std::cout << (char)(i.first+'0');
            std::cout << std::endl;
        }
    };
    dbg_print();*/

    ulong current_file = 0;
    bool is_file = true;
    for(const char& c: input) {
        if(c == '\n') continue;
        //assume all other input is just numbers and valid
        ulong number = c - '0';
        std::vector<uint32_t> section;
        if(is_file) {
            for(ulong i = 0; i < number; i++) section.push_back(current_file);
            current_file++; 
        }
        else{
            for(ulong i = 0; i < number; i++) section.push_back(UINT32_MAX);
        }
        for(ulong i = 0; i < section.size(); i++) blocks.push_back(fd(section[i], section.size()));
        is_file = !is_file;
    }

    ulong backward_cursor = blocks.size() - 1;
    while(backward_cursor > 0) {
        if(blocks[backward_cursor].first != UINT32_MAX) {
            ulong free_sec_start = UINT32_MAX;
            for(ulong fc = 0; fc < backward_cursor; fc++) {
                if(blocks[fc].first == UINT32_MAX && (blocks[fc].second >= blocks[backward_cursor].second)) {
                    free_sec_start = fc;
                    break;
                }
            }
            if(free_sec_start != UINT32_MAX) {
                const ulong free_sec_end = free_sec_start + blocks[backward_cursor].second - 1;
                const ulong copied_section_size = blocks[backward_cursor].second;
                // bcc = backward copy curser
                for(ulong bcc = 0; bcc < blocks[backward_cursor].second; bcc++) {
                    blocks[free_sec_end - bcc] = blocks[backward_cursor - bcc];
                    blocks[backward_cursor - bcc].first = UINT32_MAX;
                    blocks[backward_cursor - bcc].second = copied_section_size;
                }
                if(blocks[free_sec_end + 1].first == UINT32_MAX) {
                    auto remaining_section_size = blocks[free_sec_end + 1].second - copied_section_size;
                    // pfsac = previously free section adjustment cursor
                    for(ulong pfsac = free_sec_end + 1; pfsac < (free_sec_end + 1 + remaining_section_size); pfsac++) {
                        blocks[pfsac].second = remaining_section_size;
                    }
                }
            }
        }
        backward_cursor--;
    }

    long sum = 0;
    for(ulong i = 0; i < blocks.size(); i++) {
        if(blocks[i].first != UINT32_MAX) {
            sum += i * blocks[i].first;
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