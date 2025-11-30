// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <type_traits>
// #include <nlohmann/json.hpp>
#include <math.h>
#include <openssl/md5.h>

#include <deque>
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
using std::deque;
using std::from_range;
using std::map;
using std::print;
using std::println;
using std::regex;
using std::regex_match;
using std::regex_search;
using std::set;
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
        string key = std::vformat("{};{}", std::make_format_args(i, j));
        grid_map[key] = value;
    }
    void set_value(string key, T value) { grid_map[key] = value; }

    T& get_value(string key) { return grid_map[key]; }
    T& get_value(int i, int j) {
        string key = std::vformat("{};{}", std::make_format_args(i, j));
        return grid_map[key];
    }

    bool check_key(string key) { return grid_map.contains(key); }
    bool check_key(int i, int j) {
        string key = std::vformat("{};{}", std::make_format_args(i, j));
        return grid_map.contains(key);
    }

    auto& get_map() { return grid_map; }

   private:
    unordered_map<string, T> grid_map;
};

void print_maze(long max_i, long max_j, auto& maze) {
    for (long j = 0; j < max_j + 1; j++) {
        for (long i = 0; i < max_i + 1; i++) {
            print("{}", maze.get_value(i, j));
        }
        std::cout << std::endl;
    }
}

string to_string(tuple<int, int>& node) {
    auto [i, j] = node;
    return std::vformat("{};{}", std::make_format_args(i, j));
}

tuple<int, int> sum(tuple<int, int>& coord_1, tuple<int, int>& coord_2) {
    return {std::get<0>(coord_1) + std::get<0>(coord_2),
            std::get<1>(coord_1) + std::get<1>(coord_2)};
}

void parte_1(std::vector<std::string>& lines) {
    Grid<char> maze;
    long max_i{0};
    long max_j{0};
    string numbers{"0123456789"};
    unordered_map<int, tuple<tuple<int, int>, char>> places;
    for (auto [j, line] : vw::enumerate(lines)) {
        max_j = std::max(max_j, j);
        for (auto [i, c] : vw::enumerate(line)) {
            max_i = std::max(max_i, i);
            maze.set_value(i, j, c);
            if (numbers.contains(c)) places[stoi(string{c})] = {tuple{i, j}, c};
        }
    }

    vector<tuple<int, int>> dirs{{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    unordered_map<string, uint> distances;
    for (size_t m = 0; m < places.size(); m++) {
        for (size_t n = {m + 1}; n < places.size(); n++) {
            auto [coords_m, place_m] = places[m];
            auto [coords_n, place_n] = places[n];
            string end_key{to_string(coords_n)};
            deque<tuple<tuple<int, int>, int>> queue{{coords_m, 0}};
            std::unordered_set<string> visited{};
            visited.insert(to_string(coords_m));
            bool done{false};
            int n_steps{0};
            while (not done) {
                auto [coords, steps] = queue.front();
                queue.pop_front();
                if (to_string(coords) == end_key) {
                    done = true;
                    n_steps = steps;
                    continue;
                }
                for (auto dir : dirs) {
                    auto try_coords{sum(coords, dir)};
                    string try_key{to_string(try_coords)};
                    if (visited.contains(try_key)) continue;
                    if (maze.get_value(try_key) == '#') continue;
                    queue.push_back({try_coords, steps + 1});
                    visited.insert(try_key);
                }
                if (queue.empty()) done = true;
            }
            distances[string{place_m, place_n}] = n_steps;
            distances[string{place_n, place_m}] = n_steps;
        }
    }

    std::ostringstream oss;
    for (auto [k, v] : places) {
        oss << k;
    }
    string try_path{oss.str()};
    rg::sort(try_path);
    int min_dist{std::numeric_limits<int>::max()};
    while (std::next_permutation(try_path.begin() + 1, try_path.end())) {
        int dist{0};
        for (auto i : vw::iota(0u, try_path.size() - 1)) {
            dist += distances[try_path.substr(i, 2)];
        }
        if (dist < min_dist) min_dist = dist;
    }

    println("Resultado da parte 1: {}", min_dist);
}

void parte_2(std::vector<std::string>& lines) {
    Grid<char> maze;
    long max_i{0};
    long max_j{0};
    string numbers{"0123456789"};
    unordered_map<int, tuple<tuple<int, int>, char>> places;
    for (auto [j, line] : vw::enumerate(lines)) {
        max_j = std::max(max_j, j);
        for (auto [i, c] : vw::enumerate(line)) {
            max_i = std::max(max_i, i);
            maze.set_value(i, j, c);
            if (numbers.contains(c)) places[stoi(string{c})] = {tuple{i, j}, c};
        }
    }

    vector<tuple<int, int>> dirs{{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    unordered_map<string, uint> distances;
    for (size_t m = 0; m < places.size(); m++) {
        for (size_t n = {m + 1}; n < places.size(); n++) {
            auto [coords_m, place_m] = places[m];
            auto [coords_n, place_n] = places[n];
            string end_key{to_string(coords_n)};
            deque<tuple<tuple<int, int>, int>> queue{{coords_m, 0}};
            std::unordered_set<string> visited{};
            visited.insert(to_string(coords_m));
            bool done{false};
            int n_steps{0};
            while (not done) {
                auto [coords, steps] = queue.front();
                queue.pop_front();
                if (to_string(coords) == end_key) {
                    done = true;
                    n_steps = steps;
                    continue;
                }
                for (auto dir : dirs) {
                    auto try_coords{sum(coords, dir)};
                    string try_key{to_string(try_coords)};
                    if (visited.contains(try_key)) continue;
                    if (maze.get_value(try_key) == '#') continue;
                    queue.push_back({try_coords, steps + 1});
                    visited.insert(try_key);
                }
                if (queue.empty()) done = true;
            }
            distances[string{place_m, place_n}] = n_steps;
            distances[string{place_n, place_m}] = n_steps;
        }
    }

    std::ostringstream oss;
    for (auto [k, v] : places) {
        oss << k;
    }
    string try_path{oss.str()};
    rg::sort(try_path);
    try_path += "0";
    int min_dist{std::numeric_limits<int>::max()};
    while (std::next_permutation(try_path.begin() + 1, try_path.end() - 1)) {
        int dist{0};
        for (auto i : vw::iota(0u, try_path.size() - 1)) {
            dist += distances[try_path.substr(i, 2)];
        }
        if (dist < min_dist) min_dist = dist;
    }

    println("Resultado da parte 2: {}", min_dist);
}

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
