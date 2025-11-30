#include <Eigen/Dense>
#include <algorithm>
#include <boost/functional/hash.hpp>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
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

using nlohmann::json;

vector<string> split(const string& str, string_view delim) {
    return vector<string>{std::from_range,
                          str | vw::split(delim) | vw::transform([](auto a) {
                              return string{std::from_range, a};
                          })};
}

void parte_1(std::vector<std::string>& lines) {
    unordered_map<string, unordered_map<string, int>> happiness_map;
    std::regex pattern{R"((\w+) would (\w+) (\d+) .* (\w+)\.)"};
    for (string line : lines) {
        std::smatch matches;
        std::regex_match(line, matches, pattern);
        string name1{matches[1]};
        int signal{matches[2].str() == "gain" ? 1 : -1};
        int number{signal * std::stoi(matches[3])};
        string name2{matches[4]};
        happiness_map[name1][name2] = number;
    }
    std::vector<string> an{
        std::from_range,
        happiness_map | vw::transform([](auto& a) { return a.first; })};
    rg::sort(an);
    std::vector<vector<string>> permutations;
    permutations.emplace_back(an);
    while (std::next_permutation(an.begin(), an.end())) {
        permutations.emplace_back(an);
    }
    size_t n_persons = an.size();
    int max_happiness{0};
    for (auto& permutation : permutations) {
        int permutation_happiness{0};
        for (size_t i : vw::iota(0u, permutation.size())) {
            permutation_happiness +=
                happiness_map[permutation[i]]
                             [permutation[(i + 1) % n_persons]] +
                happiness_map[permutation[(i + 1) % n_persons]][permutation[i]];
        }
        max_happiness = std::max(max_happiness, permutation_happiness);
    }
    println("Resultado da parte 1: {}", max_happiness);
};

void parte_2(std::vector<std::string>& lines) {
    map<string, map<string, int>> happiness_map;
    std::regex pattern{R"((\w+) would (\w+) (\d+) .* (\w+)\.)"};
    for (string line : lines) {
        std::smatch matches;
        std::regex_match(line, matches, pattern);
        string name1{matches[1]};
        int signal{matches[2].str() == "gain" ? 1 : -1};
        int number{signal * std::stoi(matches[3])};
        string name2{matches[4]};
        happiness_map[name1][name2] = number;
        happiness_map[name1]["Me"] = happiness_map["Me"][name1] = 0;
    }

    std::vector<string> an{
        std::from_range,
        happiness_map | vw::transform([](auto& a) { return a.first; })};
    std::vector<vector<string>> permutations;
    permutations.emplace_back(an);
    while (std::next_permutation(an.begin(), an.end())) {
        permutations.emplace_back(an);
    }
    size_t n_persons = an.size();
    int max_happiness{0};
    for (auto& permutation : permutations) {
        int permutation_happiness{0};
        for (size_t i : vw::iota(0u, permutation.size())) {
            permutation_happiness +=
                happiness_map[permutation[i]]
                             [permutation[(i + 1) % n_persons]] +
                happiness_map[permutation[(i + 1) % n_persons]][permutation[i]];
        }
        max_happiness = std::max(max_happiness, permutation_happiness);
    }
    println("Resultado da parte 2: {}", max_happiness);
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
