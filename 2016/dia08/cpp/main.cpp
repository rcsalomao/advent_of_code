// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
// #include <json.hpp>
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
using namespace std::string_literals;
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

class Display {
    Grid<string> grid;
    int w;
    int h;

   public:
    Display(int width, int height) : w(width), h(height) {
        for (int i : vw::iota(0, w)) {
            for (int j : vw::iota(0, h)) {
                grid.set_value(i, j, ".");
            }
        }
    }

    void rect(int a, int b) {
        for (int i : vw::iota(0, a)) {
            for (int j : vw::iota(0, b)) {
                grid.set_value(i, j, "#");
            }
        }
    }

    void rotate_row(int a, int b) {
        vector<string> row{std::from_range,
                           vw::iota(0, w) | vw::transform([=, this](int i) {
                               return grid.get_value(i, a);
                           })};
        for (auto [i, s] : vw::enumerate(row)) {
            grid.set_value((i + b) % w, a, s);
        }
    }

    void rotate_column(int a, int b) {
        vector<string> column{std::from_range,
                              vw::iota(0, h) | vw::transform([=, this](int i) {
                                  return grid.get_value(a, i);
                              })};
        for (auto [i, s] : vw::enumerate(column)) {
            grid.set_value(a, (i + b) % h, s);
        }
    }

    int calc_total_lit_pixels() {
        int total{0};
        rg::for_each(grid.get_map(), [&total](auto& el) {
            if (el.second == "#") {
                total++;
            }
        });
        return total;
    }

    void show() {
        for (int j : vw::iota(0, h)) {
            for (int i : vw::iota(0, w)) {
                std::cout << grid.get_value(i, j);
            }
            std::cout << std::endl;
        }
    }
};

void parte_1(std::vector<std::string>& lines) {
    // Display display{8, 3};
    Display display{50, 6};
    std::regex pattern_1{R"(rect (\d+)x(\d+))"};
    std::regex pattern_2{R"(rotate row y=(\d+) by (\d+))"};
    std::regex pattern_3{R"(rotate column x=(\d+) by (\d+))"};
    for (auto line : lines) {
        std::smatch m;
        if (std::regex_match(line, m, pattern_1)) {
            display.rect(std::stoi(m[1]), std::stoi(m[2]));
            continue;
        }
        if (std::regex_match(line, m, pattern_2)) {
            display.rotate_row(std::stoi(m[1]), std::stoi(m[2]));
            continue;
        }
        if (std::regex_match(line, m, pattern_3)) {
            display.rotate_column(std::stoi(m[1]), std::stoi(m[2]));
            continue;
        }
    }
    println("Resultado da parte 1: {}", display.calc_total_lit_pixels());
};

void parte_2(std::vector<std::string>& lines) {
    // Display display{8, 3};
    Display display{50, 6};
    std::regex pattern_1{R"(rect (\d+)x(\d+))"};
    std::regex pattern_2{R"(rotate row y=(\d+) by (\d+))"};
    std::regex pattern_3{R"(rotate column x=(\d+) by (\d+))"};
    for (auto line : lines) {
        std::smatch m;
        if (std::regex_match(line, m, pattern_1)) {
            display.rect(std::stoi(m[1]), std::stoi(m[2]));
            continue;
        }
        if (std::regex_match(line, m, pattern_2)) {
            display.rotate_row(std::stoi(m[1]), std::stoi(m[2]));
            continue;
        }
        if (std::regex_match(line, m, pattern_3)) {
            display.rotate_column(std::stoi(m[1]), std::stoi(m[2]));
            continue;
        }
    }
    println("Resultado da parte 2: {}", "");
    display.show();
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
