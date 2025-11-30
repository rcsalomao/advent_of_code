// #include <Eigen/Dense>
#include <algorithm>
// #include <boost/functional/hash.hpp>
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

void parte_1(std::vector<std::string>& lines) {
    vector<int> containers;
    for (auto line : lines) {
        containers.emplace_back(std::stoi(line));
    }
    // println("containers: {}", containers);
    unsigned m{0};
    for (size_t i{1}; i < containers.size(); i++) {
        auto combis{make_combinations(containers, i)};
        // println("combis: {}", combis);
        for (auto& combi : combis) {
            // if (std::reduce(combi.begin(), combi.end(), 0) == 25) m++;
            if (std::reduce(combi.begin(), combi.end(), 0) == 150) m++;
            // println("combi: {} {}", combi, m);
        }
    }
    println("Resultado da parte 1: {}", m);
};

void parte_2(std::vector<std::string>& lines) {
    vector<int> containers;
    for (auto line : lines) {
        containers.emplace_back(std::stoi(line));
    }
    // println("containers: {}", containers);
    unordered_map<int, int> out_map{
        std::from_range,
        vw::iota(1u, containers.size() + 1) |
            vw::transform([](auto a) { return tuple<int, int>{a, 0}; })};
    for (size_t i{1}; i < containers.size(); i++) {
        auto combis{make_combinations(containers, i)};
        // println("combis: {}", combis);
        for (auto& combi : combis) {
            // if (std::reduce(combi.begin(), combi.end(), 0) == 25) {
            if (std::reduce(combi.begin(), combi.end(), 0) == 150) {
                out_map[combi.size()]++;
            };
        }
    }
    int res{0};
    int min_containers{std::numeric_limits<int>::max()};
    for (auto [k, v] : out_map) {
        if (k < min_containers and v > 0) {
            min_containers = k;
            res = v;
        }
    }
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
