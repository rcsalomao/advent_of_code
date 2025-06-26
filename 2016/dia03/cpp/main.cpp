// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
// #include <json.hpp>
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
    unsigned total_triangles{0};
    for (auto line : lines) {
        std::regex pattern{R"(\s+(\d+)\s+(\d+)\s+(\d+))"};
        std::smatch m;
        std::regex_match(line, m, pattern);
        int lado1 = std::stoi(m[1]);
        int lado2 = std::stoi(m[2]);
        int lado3 = std::stoi(m[3]);
        if (((lado1 + lado2) > lado3) and ((lado1 + lado3) > lado2) and
            ((lado2 + lado3) > lado1)) {
            total_triangles++;
        }
    }
    println("Resultado da parte 1: {}", total_triangles);
};

void parte_2(std::vector<std::string>& lines) {
    unsigned total_triangles{0};
    unsigned i{0};
    vector<int> triangle_TR1{};
    vector<int> triangle_TR2{};
    vector<int> triangle_TR3{};
    for (auto line : lines) {
        std::regex pattern{R"(\s+(\d+)\s+(\d+)\s+(\d+))"};
        std::smatch m;
        std::regex_match(line, m, pattern);
        triangle_TR1.emplace_back(std::stoi(m[1]));
        triangle_TR2.emplace_back(std::stoi(m[2]));
        triangle_TR3.emplace_back(std::stoi(m[3]));
        if (i % 3 == 2) {
            // process triangles
            for (auto& triangle : {triangle_TR1, triangle_TR2, triangle_TR3}) {
                int lado1 = triangle[0];
                int lado2 = triangle[1];
                int lado3 = triangle[2];
                if (((lado1 + lado2) > lado3) and ((lado1 + lado3) > lado2) and
                    ((lado2 + lado3) > lado1)) {
                    total_triangles++;
                }
            }
            // clear triangles
            triangle_TR1.clear();
            triangle_TR2.clear();
            triangle_TR3.clear();
        }
        i++;
    }
    println("Resultado da parte 2: {}", total_triangles);
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
