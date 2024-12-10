#include <functional>
#include <string_view>

struct IntVec2D {
    long x;
    long y;
    IntVec2D operator+(const IntVec2D& other) const;
    IntVec2D operator-(const IntVec2D& other) const;
    IntVec2D operator*(const auto& scalar) const;
    bool operator==(const IntVec2D& other) const;
    bool operator<(const IntVec2D& other) const;
    void dbg_print() const;
};


class CharGrid2D {
        std::vector<char> grid{};
        IntVec2D dims{};
        CharGrid2D(const std::string_view&);
    public:
        CharGrid2D(const IntVec2D&, const char&);
        bool operator==(const CharGrid2D&) const;
        bool operator<(const CharGrid2D&) const;
        static CharGrid2D from_input(const std::string_view&);
        IntVec2D get_dims() const;
        void iterate_over(const std::function<void(const IntVec2D& pos, const char& c)>);
        void iterate_over(const std::function<void(const IntVec2D& pos, const char& c)>) const;
        char at(IntVec2D) const;
        char place(IntVec2D, char);
        void dbg_print(std::ostream&) const;
};
