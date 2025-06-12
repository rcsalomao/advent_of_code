#include <Eigen/Dense>
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <print>
#include <queue>
#include <ranges>
#include <regex>
#include <unordered_map>
#include <vector>

namespace rg = std::ranges;
namespace vw = std::views;

int process_wire(std::string wire,
                 std::unordered_map<std::string, std::string>& commands_map,
                 std::unordered_map<std::string, int>& cache) {
    if (cache.contains(wire)) {
        return cache[wire];
    }
    std::regex and_pattern{R"((\w+)\s+AND\s+(\w+))"};
    std::regex or_pattern{R"((\w+)\s+OR\s+(\w+))"};
    std::regex lshift_pattern{R"((\w+)\s+LSHIFT\s+(\w+))"};
    std::regex rshift_pattern{R"((\w+)\s+RSHIFT\s+(\w+))"};
    std::regex not_pattern{R"(NOT\s+(\w+))"};
    std::regex attr_pattern{R"((\w+))"};
    std::regex num_pattern{R"(\d+)"};
    if (std::regex_match(wire, num_pattern)) {
        return std::stoi(wire);
    } else {
        std::smatch matches;
        if (std::regex_match(commands_map[wire], matches, and_pattern)) {
            int r1 = process_wire(matches[1].str(), commands_map, cache);
            int r2 = process_wire(matches[2].str(), commands_map, cache);
            int res = r1 & r2;
            cache[wire] = res;
            return res;
        }
        if (std::regex_match(commands_map[wire], matches, or_pattern)) {
            int r1 = process_wire(matches[1].str(), commands_map, cache);
            int r2 = process_wire(matches[2].str(), commands_map, cache);
            int res = r1 | r2;
            cache[wire] = res;
            return res;
        }
        if (std::regex_match(commands_map[wire], matches, rshift_pattern)) {
            int r1 = process_wire(matches[1].str(), commands_map, cache);
            int r2 = process_wire(matches[2].str(), commands_map, cache);
            int res = r1 >> r2;
            cache[wire] = res;
            return res;
        }
        if (std::regex_match(commands_map[wire], matches, lshift_pattern)) {
            int r1 = process_wire(matches[1].str(), commands_map, cache);
            int r2 = process_wire(matches[2].str(), commands_map, cache);
            int res = r1 << r2;
            cache[wire] = res;
            return res;
        }
        if (std::regex_match(commands_map[wire], matches, not_pattern)) {
            int r1 = process_wire(matches[1].str(), commands_map, cache);
            int res = ~r1;
            cache[wire] = res;
            return res;
        }
        if (std::regex_match(commands_map[wire], matches, attr_pattern)) {
            int res = process_wire(matches[1].str(), commands_map, cache);
            cache[wire] = res;
            return res;
        }
    }
};

void parte_1(std::vector<std::string>& lines) {
    std::unordered_map<std::string, std::string> commands_map;
    for (auto line : lines) {
        std::smatch matches;
        std::regex pattern{R"((.+) -> (\w+))"};
        if (std::regex_match(line, matches, pattern)) {
            commands_map[matches[2].str()] = matches[1].str();
        }
    }

    std::unordered_map<std::string, int> cache;

    int res = process_wire("a", commands_map, cache);
    std::println("Resultado da parte 1: {}", res);
};

void parte_2(std::vector<std::string>& lines) {
    std::unordered_map<std::string, std::string> commands_map;
    for (auto line : lines) {
        std::smatch matches;
        std::regex pattern{R"((.+) -> (\w+))"};
        if (std::regex_match(line, matches, pattern)) {
            commands_map[matches[2].str()] = matches[1].str();
        }
    }

    std::unordered_map<std::string, int> cache;

    int wire_a = process_wire("a", commands_map, cache);
    commands_map["b"] = std::to_string(wire_a);
    cache.clear();
    int res = process_wire("a", commands_map, cache);
    std::println("Resultado da parte 2: {}", res);
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
