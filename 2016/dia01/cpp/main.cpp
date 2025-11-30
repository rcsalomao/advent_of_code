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
#include <unordered_set>
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

int turn(string mov, int dir) {
    if (mov == "R") {
        return (dir + 1) % 4;
    } else {
        if (dir) {
            return dir - 1;
        } else {
            return 3;
        }
    }
}

void walk(vector<int>& position, int orientation, int steps) {
    switch (orientation) {
        case 0:
            position[1] += steps;
            break;
        case 1:
            position[0] += steps;
            break;
        case 2:
            position[1] -= steps;
            break;
        case 3:
            position[0] -= steps;
            break;
    }
}

void parte_1(std::vector<std::string>& lines) {
    string input{lines[0]};
    auto instructions = split(input, ", ");
    vector<int> position{0, 0};
    std::regex pattern{R"(([RL])(\d+))"};
    int orientation{0};
    for (string instruction : instructions) {
        std::smatch m;
        std::regex_match(instruction, m, pattern);
        orientation = turn(m[1].str(), orientation);
        walk(position, orientation, std::stoi(m[2]));
    }

    println("Resultado da parte 1: {}",
            std::abs(position[0]) + std::abs(position[1]));
};

void parte_2(std::vector<std::string>& lines) {
    string input{lines[0]};
    auto instructions = split(input, ", ");
    std::unordered_set<tuple<int, int>, boost::hash<tuple<int, int>>>
        visited_locations;
    vector<int> position{0, 0};
    visited_locations.insert(tuple<int, int>{0, 0});
    std::regex pattern{R"(([RL])(\d+))"};
    int orientation{0};
    for (string instruction : instructions) {
        std::smatch m;
        std::regex_match(instruction, m, pattern);
        orientation = turn(m[1].str(), orientation);
        int steps = std::stoi(m[2]);
        bool found_dup{false};
        switch (orientation) {
            case 0:
                for (int i{1}; i < steps + 1; i++) {
                    position[1] += 1;
                    if (visited_locations.contains(
                            tuple<int, int>{position[0], position[1]})) {
                        found_dup = true;
                        break;
                    } else {
                        visited_locations.insert(
                            tuple<int, int>{position[0], position[1]});
                    }
                }
                break;
            case 1:
                for (int i{1}; i < steps + 1; i++) {
                    position[0] += 1;
                    if (visited_locations.contains(
                            tuple<int, int>{position[0], position[1]})) {
                        found_dup = true;
                        break;
                    } else {
                        visited_locations.insert(
                            tuple<int, int>{position[0], position[1]});
                    }
                }
                break;
            case 2:
                for (int i{1}; i < steps + 1; i++) {
                    position[1] -= 1;
                    if (visited_locations.contains(
                            tuple<int, int>{position[0], position[1]})) {
                        found_dup = true;
                        break;
                    } else {
                        visited_locations.insert(
                            tuple<int, int>{position[0], position[1]});
                    }
                }
                break;
            case 3:
                for (int i{1}; i < steps + 1; i++) {
                    position[0] -= 1;
                    if (visited_locations.contains(
                            tuple<int, int>{position[0], position[1]})) {
                        found_dup = true;
                        break;
                    } else {
                        visited_locations.insert(
                            tuple<int, int>{position[0], position[1]});
                    }
                }
                break;
        }
        if (found_dup) break;
    }

    println("Resultado da parte 2: {}",
            std::abs(position[0]) + std::abs(position[1]));
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
