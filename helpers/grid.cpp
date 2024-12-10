#include <iostream>
#include <ranges>
#include "grid.h"

IntVec2D IntVec2D::operator+(const IntVec2D& other) const {
    return IntVec2D(x + other.x, y + other.y);
}

IntVec2D IntVec2D::operator-(const IntVec2D& other) const {
    return IntVec2D(x - other.x, y - other.y);
}
IntVec2D IntVec2D::operator*(const auto& scalar) const {
    return IntVec2D(x * scalar, y * scalar);
}

bool IntVec2D::operator==(const IntVec2D& other) const {
    return (x == other.x && y == other.y);
}

bool IntVec2D::operator<(const IntVec2D& other) const {
    return std::pair(x,y) < std::pair(other.x, other.y);
}

void IntVec2D::dbg_print() const{
    std::cout << "x: " << x << ", y: " << y << std::endl;
}

CharGrid2D::CharGrid2D(const std::string_view& input) {
    for(auto line : input | std::views::split('\n')) {
        if(line.empty()) continue;
        for(const char& c: line) {
            grid.push_back(c);
        }
        dims.y++;
    }
    if(dims.y > 0)
        dims.x = grid.size() / dims.y;
}

CharGrid2D::CharGrid2D(const IntVec2D& dims, const char& initializing_character) {
    this->dims = dims;
    this->grid = std::vector<char>(dims.x * dims.y, initializing_character);
}

bool CharGrid2D::operator==(const CharGrid2D& other) const {
    return dims==other.dims && grid==other.grid;
}

bool CharGrid2D::operator<(const CharGrid2D& other) const {
    if(dims!=other.dims) return dims < other.dims;
    else return grid < other.grid;
}

CharGrid2D CharGrid2D::from_input(const std::string_view& input) {
    CharGrid2D grid(input);
    return grid;
}

IntVec2D CharGrid2D::get_dims() const {
    return dims;
}

void CharGrid2D::iterate_over(const std::function<void(const IntVec2D& pos, const char& c)> callback) {
    for(long x = 0; x < dims.x; x++) {
        for(long y = 0; y < dims.y; y++) {
            auto pos = IntVec2D(x, y);
            callback(pos, at(pos));
        }
    }
}

char CharGrid2D::at(IntVec2D pos) const {
    if(pos.x < 0) return '\0';
    if(pos.y < 0) return '\0';
    if(pos.x >= dims.x) return '\0';
    if(pos.y >= dims.y) return '\0';
    return grid[pos.x + dims.x * pos.y];
}

char CharGrid2D::place(IntVec2D pos, char c) {
    if(pos.x < 0) return '\0';
    if(pos.y < 0) return '\0';
    if(pos.x >= dims.x) return '\0';
    if(pos.y >= dims.y) return '\0';
    char current_char = grid[pos.x + dims.x * pos.y];
    grid[pos.x + dims.x * pos.y] = c;
    return current_char;
}

void CharGrid2D::dbg_print(std::ostream& os) const {
    os << "Dimensions: x: " << dims.x << ", y: " << dims.y << std::endl;
    for(long y = 0; y < dims.y; y++) {
        for(long x = 0; x < dims.x; x++) {
            os << at(IntVec2D(x, y));
        }
        os << std::endl;
    }
}