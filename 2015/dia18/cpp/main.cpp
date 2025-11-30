// #include <Eigen/Dense>
#include <algorithm>
#include <boost/functional/hash.hpp>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
// #include <nlohmann/json.hpp>
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

class Grid {
    using GridType =
        unordered_map<tuple<int, int>, string, boost::hash<tuple<int, int>>>;
    long _w;
    long _h;
    string _s;
    GridType _grid;

   public:
    Grid(long w, long h, string s) : _w{w}, _h{h}, _s{s} {
        assert(w > 0);
        assert(h > 0);
        for (long i{0}; i < _w; i++) {
            for (long j{0}; j < _h; j++) {
                _grid[{i, j}] = _s;
            }
        }
    }

    void print() {
        for (long j{0}; j < _h; j++) {
            for (long i{0}; i < _w; i++) {
                std::cout << _grid[{i, j}];
            }
            std::cout << std::endl;
        }
    }
    void update_position(long x, long y, string s) {
        if (0 <= x and 0 <= y and x < _w and y < _h) _grid[{x, y}] = s;
    }
    string get_position(long x, long y,
                        std::optional<string> out_of_bounds = std::nullopt) {
        if (0 <= x and 0 <= y and x < _w and y < _h) {
            return _grid[{x, y}];
        } else {
            if (out_of_bounds) {
                return *out_of_bounds;
            } else {
                return _s;
            }
        }
    }
    GridType& get_grid() { return _grid; }
};

vector<string> get_neighbours(Grid& grid, long x, long y) {
    vector<string> vo;
    for (long i = -1; i < 2; i++) {
        for (long j = -1; j < 2; j++) {
            if (i == 0 and j == 0) continue;
            vo.push_back(grid.get_position(x + i, y + j));
        }
    }
    return vo;
}

void parte_1(std::vector<std::string>& lines) {
    long grid_size{100};
    Grid g{grid_size, grid_size, "."};
    for (auto [y, line] : vw::enumerate(lines)) {
        for (auto [x, c] : vw::enumerate(line)) {
            if (c == '#') g.update_position(x, y, "#");
        }
    }

    int steps{100};
    while (steps) {
        Grid copy_grid{g};
        for (long j = 0; j < grid_size; j++) {
            for (long i = 0; i < grid_size; i++) {
                auto neighbours = get_neighbours(copy_grid, i, j);
                auto c{rg::count(neighbours, "#")};
                if (copy_grid.get_position(i, j) == ".") {
                    if (c == 3) {
                        g.update_position(i, j, "#");
                    }
                } else {
                    if (c != 2 and c != 3) {
                        g.update_position(i, j, ".");
                    }
                };
            }
        }
        steps--;
    }

    // g.print();
    long n_lights{0};
    for (auto [k, v] : g.get_grid()) {
        if (v == "#") n_lights++;
    }

    println("Resultado da parte 1: {}", n_lights);
};

void parte_2(std::vector<std::string>& lines) {
    long grid_size{100};
    Grid g{grid_size, grid_size, "."};
    for (auto [y, line] : vw::enumerate(lines)) {
        for (auto [x, c] : vw::enumerate(line)) {
            if (c == '#') g.update_position(x, y, "#");
        }
    }

    int steps{100};
    while (steps) {
        Grid copy_grid{g};
        for (long j = 0; j < grid_size; j++) {
            for (long i = 0; i < grid_size; i++) {
                if (i == 0 and j == 0) continue;
                if (i == 99 and j == 0) continue;
                if (i == 0 and j == 99) continue;
                if (i == 99 and j == 99) continue;
                auto neighbours = get_neighbours(copy_grid, i, j);
                auto c{rg::count(neighbours, "#")};
                if (copy_grid.get_position(i, j) == ".") {
                    if (c == 3) {
                        g.update_position(i, j, "#");
                    }
                } else {
                    if (c != 2 and c != 3) {
                        g.update_position(i, j, ".");
                    }
                };
            }
        }
        steps--;
    }

    // g.print();
    long n_lights{0};
    for (auto [k, v] : g.get_grid()) {
        if (v == "#") n_lights++;
    }

    println("Resultado da parte 2: {}", n_lights);
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
