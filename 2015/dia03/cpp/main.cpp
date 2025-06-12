#include <algorithm>
#include <boost/functional/hash.hpp>
#include <fstream>
#include <functional>
#include <numeric>
#include <print>
#include <ranges>
#include <unordered_set>
#include <vector>

namespace rg = std::ranges;
namespace vw = std::views;

void process_walk(char c, std::tuple<int, int>& position) {
    if (c == '^') std::get<1>(position)++;
    if (c == 'v') std::get<1>(position)--;
    if (c == '>') std::get<0>(position)++;
    if (c == '<') std::get<0>(position)--;
}

void parte_1(std::vector<std::string>& lines) {
    std::string instructions = lines[0];
    std::tuple<int, int> position = {0, 0};
    std::unordered_set<std::tuple<int, int>, boost::hash<std::tuple<int, int>>>
        visited_houses;
    visited_houses.insert(position);
    for (char c : instructions) {
        process_walk(c, position);
        visited_houses.insert(position);
    }
    std::print("Total parte 1: {}\n", visited_houses.size());
};

struct boost_hash_tuple {
    template <typename T>
    size_t operator()(const std::tuple<T, T>& x) const {
        return boost::hash_value(x);
    }
};

void parte_2(std::vector<std::string>& lines) {
    std::string instructions = lines[0];
    std::tuple<int, int> santa_position = {0, 0};
    std::tuple<int, int> robo_santa_position = {0, 0};
    std::unordered_set<std::tuple<int, int>, boost_hash_tuple> visited_houses;
    visited_houses.insert(santa_position);
    int flag{0};
    for (char c : instructions) {
        if (flag) {
            flag = 0;
            process_walk(c, santa_position);
            visited_houses.insert(santa_position);
        } else {
            flag = 1;
            process_walk(c, robo_santa_position);
            visited_houses.insert(robo_santa_position);
        }
    }
    std::print("Total parte 2: {}\n", visited_houses.size());
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
