#include <Eigen/Dense>
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <print>
#include <ranges>
#include <regex>
#include <vector>

namespace rg = std::ranges;
namespace vw = std::views;

auto get_point(auto& match) {
    return std::vector<int>{
        std::from_range, match | vw::split(',') | vw::transform([](auto a) {
                             return std::stoi(std::string{std::from_range, a});
                         })};
}

void parte_1(std::vector<std::string>& lines) {
    std::map<int, int> mapping{{0, 1}, {1, 0}};
    constexpr int grid_size = 1000;
    Eigen::ArrayXXi grid(grid_size, grid_size);
    grid.setZero();
    for (auto line : lines) {
        std::regex point_pattern{R"(\d+,\d+)"};
        std::vector<std::string> matches;
        std::for_each(
            std::sregex_iterator(line.begin(), line.end(), point_pattern),
            std::sregex_iterator(),
            [&matches](auto i) { matches.push_back(std::smatch{i}.str()); });
        auto point_1 = get_point(matches[0]);
        auto point_2 = get_point(matches[1]);
        int x0 = point_1[0];
        int y0 = point_1[1];
        int x1 = point_2[0];
        int y1 = point_2[1];
        auto dx = x1 - x0;
        auto dy = y1 - y0;
        if (line.contains("turn on")) {
            grid.block(y0, x0, dy + 1, dx + 1) = 1;
        };
        if (line.contains("turn off")) {
            grid.block(y0, x0, dy + 1, dx + 1) = 0;
        };
        if (line.contains("toggle")) {
            for (int i : vw::iota(y0, y1 + 1)) {
                for (int j : vw::iota(x0, x1 + 1)) {
                    grid(i, j) = mapping[grid(i, j)];
                }
            }
        };
    }
    std::println("Resultado da parte 1: {}", grid.sum());
};

void parte_2(std::vector<std::string>& lines) {
    constexpr int grid_size = 1000;
    Eigen::ArrayXXi grid(grid_size, grid_size);
    grid.setZero();
    for (auto line : lines) {
        std::regex point_pattern{R"(\d+,\d+)"};
        std::vector<std::string> matches;
        std::for_each(
            std::sregex_iterator(line.begin(), line.end(), point_pattern),
            std::sregex_iterator(),
            [&matches](auto i) { matches.push_back(std::smatch{i}.str()); });
        auto point_1 = get_point(matches[0]);
        auto point_2 = get_point(matches[1]);
        int x0 = point_1[0];
        int y0 = point_1[1];
        int x1 = point_2[0];
        int y1 = point_2[1];
        auto dx = x1 - x0;
        auto dy = y1 - y0;
        if (line.contains("turn on")) {
            grid.block(y0, x0, dy + 1, dx + 1) += 1;
        };
        if (line.contains("turn off")) {
            for (int i : vw::iota(y0, y1 + 1)) {
                for (int j : vw::iota(x0, x1 + 1)) {
                    grid(i, j) = std::max(0, grid(i, j) - 1);
                }
            }
        };
        if (line.contains("toggle")) {
            grid.block(y0, x0, dy + 1, dx + 1) += 2;
        };
    }
    std::println("Resultado da parte 2: {}", grid.sum());
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
