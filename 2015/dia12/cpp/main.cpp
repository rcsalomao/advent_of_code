#include <Eigen/Dense>
#include <algorithm>
#include <boost/functional/hash.hpp>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <limits>
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
using std::print;
using std::println;
using std::string;
using std::string_view;
using std::to_string;
using std::tuple;
using std::unordered_map;
using std::vector;

using nlohmann::json;

vector<string> split(const string& str, string_view delim) {
    return vector<string>{std::from_range,
                          str | vw::split(delim) | vw::transform([](auto a) {
                              return string{std::from_range, a};
                          })};
}

void f1(json& a, int& total) {
    if (a.is_number()) {
        total += a.template get<int>();
    }
    if (a.is_array()) {
        for (auto& v : a) {
            f1(v, total);
        }
    }
    if (a.is_object()) {
        for (auto& [k, v] : a.items()) {
            f1(a[k], total);
        }
    }
}

void f2(json& a, int& total) {
    if (a.is_number()) {
        total += a.template get<int>();
    }
    if (a.is_array()) {
        for (auto& v : a) {
            f2(v, total);
        }
    }
    if (a.is_object()) {
        for (auto& v : a) {
            if (v.is_string()) {
                if ("red" == v.template get<string>()) return;
            }
        }
        for (auto& [k, v] : a.items()) {
            f2(a[k], total);
        }
    }
}

void parte_1(std::vector<std::string>& lines) {
    string input{lines[0]};
    json data = json::parse(input);
    int total{0};
    f1(data, total);
    println("Total da parte 1: {}", total);
};

void parte_2(std::vector<std::string>& lines) {
    string input{lines[0]};
    json data = json::parse(input);
    int total{0};
    f2(data, total);
    println("Total da parte 2: {}", total);
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
