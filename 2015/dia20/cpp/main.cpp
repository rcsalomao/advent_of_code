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

void parte_1(std::vector<std::string>& lines) {
    int input = std::stoi(lines[0]);

    int N{input / 10};
    std::vector<int> houses(N, 0);

    int house;
    for (int i : vw::iota(0, N)) {
        int j = i;
        while (j < N) {
            houses[j] += (i + 1) * 10;
            j += (i + 1);
        }
        if (houses[i] > input) {
            house = i + 1;
            break;
        }
    }

    println("Resultado da parte 1: {}", house);
};

void parte_2(std::vector<std::string>& lines) {
    int input = std::stoi(lines[0]);

    int N{input / 10};
    std::vector<int> houses(N, 0);

    int house;
    for (int i : vw::iota(0, N)) {
        int j = i;
        int count{50};
        while (j < N and count) {
            houses[j] += (i + 1) * 11;
            j += (i + 1);
            count--;
        }
        if (houses[i] > input) {
            house = i + 1;
            break;
        }
    }

    println("Resultado da parte 2: {}", house);
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
