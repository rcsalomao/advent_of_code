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
#include <math.h>
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

void parte_1(std::vector<std::string>& lines) {
    vector<std::pair<long, long>> intervals;
    for (string line : lines) {
        auto d{split(line, "-")};
        intervals.emplace_back(std::stol(d[0]), std::stol(d[1]));
    }
    long i{0};
    bool done{false};
    while (not done) {
        bool keep_searching{false};
        auto it{intervals.begin()};
        for (; it < intervals.end();) {
            if (i >= it->first and i <= it->second) {
                i = it->second + 1;
                keep_searching = true;
                break;
            }
            it++;
        }
        if (keep_searching) {
            intervals.erase(it);
        } else {
            done = true;
        }
    }
    println("Resultado da parte 1: {}", i);
}

void parte_2(std::vector<std::string>& lines) {
    vector<std::pair<ulong, ulong>> intervals;
    for (string line : lines) {
        auto d{split(line, "-")};
        intervals.emplace_back(std::stol(d[0]), std::stol(d[1]));
    }
    rg::sort(intervals);
    ulong max{4294967295};
    ulong permitidos{0};
    ulong max_atual{0};
    for (auto [a, b] : intervals) {
        if (a > max_atual) {
            permitidos += a - max_atual;
        }
        if (b + 1 > max_atual) {
            max_atual = b + 1;
        }
    }
    if (max_atual <= max) {
        permitidos += max - max_atual + 1;
    }

    println("Resultado da parte 2: {}", permitidos);
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
