// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
// #include <nlohmann/json.hpp>
#include <format>
#include <limits>
#include <map>
#include <numeric>
#include <print>
#include <queue>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace rg = std::ranges;
namespace vw = std::views;
using namespace std::string_view_literals;
using std::map;
using std::print;
using std::println;
using std::string;
using std::string_view;
using std::to_string;
using std::tuple;
using std::unordered_map;
using std::vector;

// using nlohmann::json;

vector<string> split(const string& str, string_view delim) {
    return vector<string>{std::from_range,
                          str | vw::split(delim) | vw::transform([](auto a) {
                              return string{std::from_range, a};
                          })};
}

template <typename T>
class Grid {
   public:
    Grid() = default;

    void set_value(int i, int j, T value) {
        string key = std::vformat("{},{}", std::make_format_args(i, j));
        grid_map[key] = value;
    }
    void set_value(string key, T value) { grid_map[key] = value; }

    T& get_value(string key) { return grid_map[key]; }
    T& get_value(int i, int j) {
        string key = std::vformat("{},{}", std::make_format_args(i, j));
        return grid_map[key];
    }

    bool check_key(string key) { return grid_map.contains(key); }
    bool check_key(int i, int j) {
        string key = std::vformat("{},{}", std::make_format_args(i, j));
        return grid_map.contains(key);
    }

    auto& get_map() { return grid_map; }

   private:
    unordered_map<string, T> grid_map;
};

void change_position_parte_1(char c, vector<int>& position) {
    switch (c) {
        case 'D':
            position[1] += 1;
            break;
        case 'U':
            position[1] -= 1;
            break;
        case 'R':
            position[0] += 1;
            break;
        case 'L':
            position[0] -= 1;
            break;
    }
    for (auto& p : position) {
        p = std::clamp(p, 0, 2);
    }
}

void parte_1(std::vector<std::string>& lines) {
    Grid<string> keypad{};
    keypad.set_value(0, 0, "1");
    keypad.set_value(1, 0, "2");
    keypad.set_value(2, 0, "3");
    keypad.set_value(0, 1, "4");
    keypad.set_value(1, 1, "5");
    keypad.set_value(2, 1, "6");
    keypad.set_value(0, 2, "7");
    keypad.set_value(1, 2, "8");
    keypad.set_value(2, 2, "9");

    std::stringstream out{};
    vector<int> position{1, 1};
    for (string line : lines) {
        for (char c : line) {
            change_position_parte_1(c, position);
        }
        out << keypad.get_value(position[0], position[1]);
    }

    println("Resultado da parte 1: {}", out.str());
};

void change_position_parte_2(char c, vector<int>& position) {
    vector<int> new_position{position};
    switch (c) {
        case 'D':
            new_position[1] -= 1;
            break;
        case 'U':
            new_position[1] += 1;
            break;
        case 'R':
            new_position[0] += 1;
            break;
        case 'L':
            new_position[0] -= 1;
            break;
    }
    if ((abs(new_position[0]) + abs(new_position[1])) < 3) {
        position = new_position;
    }
}

void parte_2(std::vector<std::string>& lines) {
    Grid<string> keypad{};
    keypad.set_value(0, 2, "1");
    keypad.set_value(0, 1, "3");
    keypad.set_value(0, 0, "7");
    keypad.set_value(0, -1, "B");
    keypad.set_value(0, -2, "D");

    keypad.set_value(1, 1, "4");
    keypad.set_value(1, 0, "8");
    keypad.set_value(1, -1, "C");

    keypad.set_value(-1, 1, "2");
    keypad.set_value(-1, 0, "6");
    keypad.set_value(-1, -1, "A");

    keypad.set_value(2, 0, "9");

    keypad.set_value(-2, 0, "5");

    std::stringstream out{};
    vector<int> position{-2, 0};
    for (string line : lines) {
        for (char c : line) {
            change_position_parte_2(c, position);
        }
        out << keypad.get_value(position[0], position[1]);
    }

    println("Resultado da parte 2: {}", out.str());
};

int main(int, char** argv) {
    std::print("\n");

    std::ifstream input;
    input.open(argv[1]);

    std::string line;
    std::vector<std::string> lines;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    input.close();

    parte_1(lines);
    parte_2(lines);

    return 0;
}
