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
    vector<vector<string>> instructions;
    for (string line : lines) {
        std::smatch m;
        std::regex patter_1{R"((\w+)\s(\w))"};
        std::regex patter_2{R"((\w+)\s([+-]\d+))"};
        std::regex patter_3{R"((\w+)\s(\w),\s([+-]\d+))"};
        if (std::regex_match(line, m, patter_1)) {
            instructions.emplace_back(vector<string>{m[1], m[2]});
        }
        if (std::regex_match(line, m, patter_2)) {
            instructions.emplace_back(vector<string>{m[1], m[2]});
        }
        if (std::regex_match(line, m, patter_3)) {
            instructions.emplace_back(vector<string>{m[1], m[2], m[3]});
        }
    }
    unsigned a{0};
    unsigned b{0};
    size_t index{0};
    while (true) {
        if (index >= instructions.size()) break;
        vector<string> instruction{instructions[index]};
        string cmd{instruction[0]};
        if (cmd == "inc") {
            if (instruction[1] == "a") {
                a++;
            }
            if (instruction[1] == "b") {
                b++;
            }
            index++;
        }
        if (cmd == "jio") {
            unsigned val{0};
            if (instruction[1] == "a") {
                val = a;
            }
            if (instruction[1] == "b") {
                val = b;
            }
            if (val == 1) {
                index += std::stoi(instruction[2]);
            } else {
                index++;
            }
        }
        if (cmd == "jie") {
            unsigned val{0};
            if (instruction[1] == "a") {
                val = a;
            }
            if (instruction[1] == "b") {
                val = b;
            }
            if (val % 2 == 0) {
                index += std::stoi(instruction[2]);
            } else {
                index++;
            }
        }
        if (cmd == "tpl") {
            if (instruction[1] == "a") {
                a *= 3;
            }
            if (instruction[1] == "b") {
                b *= 3;
            }
            index++;
        }
        if (cmd == "hlf") {
            if (instruction[1] == "a") {
                a /= 2;
            }
            if (instruction[1] == "b") {
                b /= 2;
            }
            index++;
        }
        if (cmd == "jmp") {
            index += std::stoi(instruction[1]);
        }
    }

    println("Resultado da parte 1: {}", b);
};

void parte_2(std::vector<std::string>& lines) {
    vector<vector<string>> instructions;
    for (string line : lines) {
        std::smatch m;
        std::regex patter_1{R"((\w+)\s(\w))"};
        std::regex patter_2{R"((\w+)\s([+-]\d+))"};
        std::regex patter_3{R"((\w+)\s(\w),\s([+-]\d+))"};
        if (std::regex_match(line, m, patter_1)) {
            instructions.emplace_back(vector<string>{m[1], m[2]});
        }
        if (std::regex_match(line, m, patter_2)) {
            instructions.emplace_back(vector<string>{m[1], m[2]});
        }
        if (std::regex_match(line, m, patter_3)) {
            instructions.emplace_back(vector<string>{m[1], m[2], m[3]});
        }
    }
    unsigned a{1};
    unsigned b{0};
    size_t index{0};
    while (true) {
        if (index >= instructions.size()) break;
        vector<string> instruction{instructions[index]};
        string cmd{instruction[0]};
        if (cmd == "inc") {
            if (instruction[1] == "a") {
                a++;
            }
            if (instruction[1] == "b") {
                b++;
            }
            index++;
        }
        if (cmd == "jio") {
            unsigned val{0};
            if (instruction[1] == "a") {
                val = a;
            }
            if (instruction[1] == "b") {
                val = b;
            }
            if (val == 1) {
                index += std::stoi(instruction[2]);
            } else {
                index++;
            }
        }
        if (cmd == "jie") {
            unsigned val{0};
            if (instruction[1] == "a") {
                val = a;
            }
            if (instruction[1] == "b") {
                val = b;
            }
            if (val % 2 == 0) {
                index += std::stoi(instruction[2]);
            } else {
                index++;
            }
        }
        if (cmd == "tpl") {
            if (instruction[1] == "a") {
                a *= 3;
            }
            if (instruction[1] == "b") {
                b *= 3;
            }
            index++;
        }
        if (cmd == "hlf") {
            if (instruction[1] == "a") {
                a /= 2;
            }
            if (instruction[1] == "b") {
                b /= 2;
            }
            index++;
        }
        if (cmd == "jmp") {
            index += std::stoi(instruction[1]);
        }
    }

    println("Resultado da parte 2: {}", b);
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
