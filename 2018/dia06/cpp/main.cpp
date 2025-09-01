// #include <Eigen/Dense>
// #include <json.hpp>
// #include <openssl/md5.h>
#include <bits/stdc++.h>

namespace rg = std::ranges;
namespace vw = std::views;
using namespace std::string_literals;
using namespace std::string_view_literals;
using std::deque;
using std::from_range;
using std::map;
using std::print;
using std::println;
using std::queue;
using std::regex;
using std::regex_match;
using std::regex_search;
using std::set;
using std::shared_ptr;
using std::smatch;
using std::string;
using std::string_view;
using std::to_string;
using std::tuple;
using std::unique_ptr;
using std::unordered_map;
using std::unordered_set;
using std::vector;

// using nlohmann::json;

vector<string> split(const string& str, string_view delim) {
    return vector<string>{std::from_range,
                          str | vw::split(delim) | vw::transform([](auto a) {
                              return string{std::from_range, a};
                          })};
}

vector<string> find_all(string& line, regex& pattern) {
    vector<string> matches;
    // std::for_each(std::sregex_iterator(line.begin(), line.end(), pattern),
    //               std::sregex_iterator(),
    //               [&matches](smatch i) { matches.push_back(i.str()); });
    for (std::sregex_iterator it{line.begin(), line.end(), pattern};
         it != std::sregex_iterator{}; it++) {
        matches.push_back(it->str());
    }
    return matches;
}

struct Coord {
    int row;
    int col;
};

string to_string(Coord& coord) {
    return "(" + to_string(coord.row) + "," + to_string(coord.col) + ")";
}

auto parte_1(vector<string>& lines) {
    vector<Coord> coords;
    for (auto line : lines) {
        auto a = split(line, ", ");
        coords.emplace_back(stoi(a[0]), stoi(a[1]));
    }

    int min_row = rg::min(coords, {}, &Coord::row).row;
    int min_col = rg::min(coords, {}, &Coord::col).col;
    int max_row = rg::max(coords, {}, &Coord::row).row;
    int max_col = rg::max(coords, {}, &Coord::col).col;

    unordered_set<int> idxs_to_exclude;
    for (int row : vw::iota(min_row, max_row + 1)) {
        for (int col : vw::iota(min_col, max_col + 1)) {
            if (!(row == min_row or row == max_row) and
                !(col == min_col or col == max_col))
                continue;
            int min_dist = std::numeric_limits<int>::max();
            int idx{-1};
            for (auto [i, coord] : vw::enumerate(coords)) {
                int dist = abs(coord.row - row) + abs(coord.col - col);
                if (dist < min_dist) {
                    min_dist = dist;
                    idx = i;
                }
            }
            idxs_to_exclude.insert(idx);
        }
    }

    unordered_map<int, int> regions_count;
    for (int row : vw::iota(min_row, max_row + 1)) {
        for (int col : vw::iota(min_col, max_col + 1)) {
            if (!(row > min_row and row < max_row and col > min_col and
                  col < max_col))
                continue;
            int min_dist = std::numeric_limits<int>::max();
            int idx{-1};
            for (auto [i, coord] : vw::enumerate(coords)) {
                int dist = abs(coord.row - row) + abs(coord.col - col);
                if (dist < min_dist) {
                    min_dist = dist;
                    idx = i;
                }
            }
            regions_count[idx]++;
        }
    }

    int r{-1};
    for (auto [k, v] : regions_count) {
        if (idxs_to_exclude.contains(k)) continue;
        r = std::max(r, v);
    }

    return r;
}

auto parte_2(vector<string>& lines) {
    vector<Coord> coords;
    for (auto line : lines) {
        auto a = split(line, ", ");
        coords.emplace_back(stoi(a[0]), stoi(a[1]));
    }

    int min_row = rg::min(coords, {}, &Coord::row).row;
    int min_col = rg::min(coords, {}, &Coord::col).col;
    int max_row = rg::max(coords, {}, &Coord::row).row;
    int max_col = rg::max(coords, {}, &Coord::col).col;

    int r{0};
    for (int row : vw::iota(min_row, max_row + 1)) {
        for (int col : vw::iota(min_col, max_col + 1)) {
            int sum{0};
            for (auto [i, coord] : vw::enumerate(coords)) {
                int dist = abs(coord.row - row) + abs(coord.col - col);
                sum += dist;
            }
            if (sum < 10000) r++;
        }
    }

    return r;
}

int main(int, char** argv) {
    std::ifstream input;
    input.open(argv[1]);

    string line;
    vector<string> lines;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    input.close();

    std::print("\n");
    println("Resultado da parte 1: {}", parte_1(lines));
    println("Resultado da parte 2: {}", parte_2(lines));

    return 0;
}
