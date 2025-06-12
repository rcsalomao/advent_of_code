// #include <Eigen/Dense>
#include <algorithm>
#include <boost/functional/hash.hpp>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
// #include <json.hpp>
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

void parte_1(std::vector<std::string>& lines) {
    size_t row, column;
    for (string line : lines) {
        std::smatch m;
        std::regex_match(line, m, std::regex{R"(.+ row (\d+), column (\d+).)"});
        row = std::stoi(m[1]);
        column = std::stoi(m[2]);
    }

    size_t diagonal{row + column - 1};
    size_t j{1};
    for (size_t i = 0; i < diagonal; i++) {
        j += i;
    }
    size_t k{j + column - 1};

    size_t number{20151125};
    for (size_t i = 1; i < k; i++) {
        number *= 252533;
        number %= 33554393;
    }

    println("Resultado da parte 1: {}", number);
};

void parte_2(std::vector<std::string>& lines) {
    println("Resultado da parte 2: {}", "");
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
    // parte_2(lines);

    return 0;
}
