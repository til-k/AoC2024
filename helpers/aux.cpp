#include "aux.h"
#include <ranges>

void timed_execution(std::function<long(const std::string_view&)> parse_fun, const std::string_view& input, const std::string_view& title) {
    auto start = std::chrono::system_clock::now();
    long result = parse_fun(input);
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << title << std::endl;
    std::cout << "Result: " << result << std::endl;
    std::cout << "Time needed: " << elapsed.count() << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
}

std::vector<std::string_view> to_svv(const std::string_view& input, const std::string_view& delim) {
    auto split_input = input | std::views::split(delim);
    std::vector<std::string_view> out;
    for(const auto& part : split_input) {
        out.push_back(std::string_view(part.data()));
    }
    return out;
}