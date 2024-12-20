#include <string_view>
#include <functional>
#include <chrono>
#include <iostream>

void timed_execution(std::function<long(const std::string_view&)>, const std::string_view&, const std::string_view& title);
std::vector<std::string_view> to_svv(const std::string_view& input, const std::string_view& delim);
