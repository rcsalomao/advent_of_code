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

void parte_1(std::vector<std::string>& lines) {
    int total{0};
    for (auto line : lines) {
        auto numbers = split(line, "\t") |
                       vw::transform([](auto& a) { return stoi(a); }) |
                       rg::to<vector<int>>();
        auto [min, max] = rg::minmax(numbers);
        total += max - min;
    }
    println("Resultado da parte 1: {}", total);
}

template <typename F>
void calc_combinations_index(size_t n, size_t k, F&& out) {
    if (k > n) return;
    std::vector<size_t> a(k);
    for (size_t i = 0; i < k; ++i) a[i] = i;
    while (true) {
        out(a);
        int i = int(k) - 1;
        while (i >= 0 && a[i] >= n - 1 - (k - 1 - i)) --i;
        if (i < 0) break;
        for (size_t j = a[i] + 1; i < (long)k; ++j, ++i) a[i] = j;
    }
}

template <class T>
vector<vector<T>> make_combinations(vector<T>& vi, size_t n) {
    vector<vector<T>> vo;
    calc_combinations_index(vi.size(), n, [&vo, &vi](auto& a) {
        vector<T> v;
        for (auto& i : a) {
            v.push_back(vi[i]);
        }
        vo.emplace_back(v);
    });
    return vo;
}

void parte_2(std::vector<std::string>& lines) {
    int total{0};
    for (auto line : lines) {
        auto numbers = split(line, "\t") |
                       vw::transform([](auto& a) { return stoi(a); }) |
                       rg::to<vector<int>>();
        auto res = make_combinations(numbers, 2);
        for (auto vi : res) {
            int a{vi[0]};
            int b{vi[1]};
            if (a % b == 0) {
                total += a / b;
                break;
            }
            if (b % a == 0) {
                total += b / a;
                break;
            }
        }
    }

    println("Resultado da parte 2: {}", total);
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
