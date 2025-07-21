// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <type_traits>
// #include <json.hpp>
#include <openssl/md5.h>

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

string calc_floor_line(string_view pf) {
    string r;
    for (size_t i : vw::iota(0ul, pf.size())) {
        if (i == 0) {
            if ((pf[i] == '^' and pf[i + 1] == '^') or
                (pf[i] == '.' and pf[i + 1] == '^')) {
                r += "^";
            } else {
                r += ".";
            }
        } else if (i == pf.size() - 1) {
            if ((pf[i - 1] == '^' and pf[i] == '^') or
                (pf[i - 1] == '^' and pf[i] == '.')) {
                r += "^";
            } else {
                r += ".";
            }
        } else {
            if ((pf[i - 1] == '^' and pf[i] == '^' and pf[i + 1] == '.') or
                (pf[i - 1] == '.' and pf[i] == '^' and pf[i + 1] == '^') or
                (pf[i - 1] == '^' and pf[i] == '.' and pf[i + 1] == '.') or
                (pf[i - 1] == '.' and pf[i] == '.' and pf[i + 1] == '^')) {
                r += "^";
            } else {
                r += ".";
            }
        }
    }
    return r;
}

void parte_1(std::vector<std::string>& lines) {
    vector<string> floor_lines{lines[0]};
    int num_lines{40};
    for (auto _ : vw::iota(0, num_lines - 1)) {
        floor_lines.emplace_back(calc_floor_line(floor_lines.back()));
    }
    unsigned total_safe_tiles{0};
    for (string_view floor : floor_lines) {
        total_safe_tiles += rg::count(floor, '.');
        // println("{}", floor);
    }
    println("Resultado da parte 1: {}", total_safe_tiles);
}

void parte_2(std::vector<std::string>& lines) {
    vector<string> floor_lines{lines[0]};
    int num_lines{400000};
    for (auto _ : vw::iota(0, num_lines - 1)) {
        floor_lines.emplace_back(calc_floor_line(floor_lines.back()));
    }
    unsigned total_safe_tiles{0};
    for (string_view floor : floor_lines) {
        total_safe_tiles += rg::count(floor, '.');
    }
    println("Resultado da parte 2: {}", total_safe_tiles);
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
