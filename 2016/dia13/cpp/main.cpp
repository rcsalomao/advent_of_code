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
using std::from_range;
using std::map;
using std::print;
using std::println;
using std::regex;
using std::regex_match;
using std::regex_search;
using std::smatch;
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

std::vector<std::string> find_all(std::string& line, std::regex& pattern) {
    std::vector<std::string> matches;
    // std::for_each(std::sregex_iterator(line.begin(), line.end(), pattern),
    //               std::sregex_iterator(),
    //               [&matches](std::smatch i) { matches.push_back(i.str()); });
    for (std::sregex_iterator it{line.begin(), line.end(), pattern};
         it != std::sregex_iterator{}; it++) {
        matches.push_back(it->str());
    }
    return matches;
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

template <typename T>
void print_grid(Grid<T>& grid, int w, int h) {
    assert(w > 0);
    assert(h > 0);
    for (int j : vw::iota(0, h)) {
        for (int i : vw::iota(0, w)) {
            std::cout << grid.get_value(i, j);
        }
        std::cout << std::endl;
    }
}

struct GridPoint {
    int i;
    int j;
    ulong n_steps{0};
};

bool operator==(const GridPoint& gp1, const GridPoint& gp2) {
    return (gp1.i == gp2.i) and (gp1.j == gp2.j);
}

struct HashFunctor {
    size_t operator()(const GridPoint& grid_point) const {
        string s = to_string(grid_point.i) + "," + to_string(grid_point.j);
        return std::hash<string>{}(s);
    }
};

string dec_to_binary(int n) {
    string res = "";

    while (n > 0) {
        int bit = n & 1;
        res.push_back('0' + bit);
        n = n >> 1;
    }

    std::reverse(res.begin(), res.end());
    return res;
}

void parte_1(std::vector<std::string>& lines) {
    string input = lines[0];
    smatch m;
    regex_match(input, m, regex{R"(.* (\d+))"});
    int favorite_number{stoi(m[1].str())};

    auto test_function = [favorite_number](int x, int y) {
        return x * x + 3 * x + 2 * x * y + y + y * y + favorite_number;
    };

    auto space_type = [test_function](int i, int j) {
        int num{test_function(i, j)};
        string bin_num{dec_to_binary(num)};
        int n_1s = rg::count(bin_num, '1');
        if (n_1s % 2) {
            return "#";
        } else {
            return ".";
        }
    };

    // int max_w{10};
    // int max_h{10};
    // Grid<string> grid{};
    ulong res{0};

    std::queue<GridPoint> q;
    std::unordered_set<GridPoint, HashFunctor> seen;
    q.push(GridPoint{1, 1});
    seen.insert(GridPoint{1, 1});
    // grid.set_value(0, 0, "s");
    vector<tuple<int, int>> dirs{{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
    while (!q.empty()) {
        GridPoint gp{q.front()};
        q.pop();
        bool done{false};
        for (auto [i, j] : dirs) {
            GridPoint new_gp{gp.i + i, gp.j + j, gp.n_steps};
            new_gp.n_steps++;
            // if (new_gp.i < 0 or new_gp.i > max_w - 1) continue;
            // if (new_gp.j < 0 or new_gp.j > max_h - 1) continue;
            if (new_gp.i < 0) continue;
            if (new_gp.j < 0) continue;
            if (new_gp.i == 31 and new_gp.j == 39) {
                done = true;
                res = new_gp.n_steps;
                break;
            }
            // if (!seen.contains(new_gp)) {
            if (!seen.contains(new_gp) and
                space_type(new_gp.i, new_gp.j) == "."s) {
                q.push(new_gp);
                seen.insert(new_gp);
                // grid.set_value(new_gp.i, new_gp.j,
                //                space_type(new_gp.i, new_gp.j));
            }
        }
        if (done) break;
    }

    // print_grid(grid, max_w, max_h);
    // println("{}", vector{from_range, seen | vw::transform([](auto& s) {
    //                                      return to_string(s.i) + "," +
    //                                             to_string(s.j);
    //                                  })});

    println("Resultado da parte 1: {}", res);
};

void parte_2(std::vector<std::string>& lines) {
    string input = lines[0];
    smatch m;
    regex_match(input, m, regex{R"(.* (\d+))"});
    int favorite_number{stoi(m[1].str())};

    auto test_function = [favorite_number](int x, int y) {
        return x * x + 3 * x + 2 * x * y + y + y * y + favorite_number;
    };

    auto space_type = [test_function](int i, int j) {
        int num{test_function(i, j)};
        string bin_num{dec_to_binary(num)};
        int n_1s = rg::count(bin_num, '1');
        if (n_1s % 2) {
            return "#";
        } else {
            return ".";
        }
    };

    // int max_w{10};
    // int max_h{10};
    // Grid<string> grid{};

    std::queue<GridPoint> q;
    std::unordered_set<GridPoint, HashFunctor> seen;
    q.push(GridPoint{1, 1});
    seen.insert(GridPoint{1, 1});
    // grid.set_value(0, 0, "s");
    vector<tuple<int, int>> dirs{{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
    while (!q.empty()) {
        GridPoint gp{q.front()};
        q.pop();
        bool done{false};
        for (auto [i, j] : dirs) {
            GridPoint new_gp{gp.i + i, gp.j + j, gp.n_steps};
            new_gp.n_steps++;
            // if (new_gp.i < 0 or new_gp.i > max_w - 1) continue;
            // if (new_gp.j < 0 or new_gp.j > max_h - 1) continue;
            if (new_gp.i < 0) continue;
            if (new_gp.j < 0) continue;
            if (new_gp.i == 31 and new_gp.j == 39) {
                done = true;
                break;
            }
            // if (!seen.contains(new_gp)) {
            if (!seen.contains(new_gp) and
                space_type(new_gp.i, new_gp.j) == "."s) {
                q.push(new_gp);
                seen.insert(new_gp);
                // grid.set_value(new_gp.i, new_gp.j,
                //                space_type(new_gp.i, new_gp.j));
            }
        }
        if (done) break;
    }

    // print_grid(grid, max_w, max_h);

    size_t res = vector{
        from_range,
        seen | vw::filter([](auto& s) {
            return s.n_steps <= 50;
        })}.size();

    println("Resultado da parte 2: {}", res);
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
