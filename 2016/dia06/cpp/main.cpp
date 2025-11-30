// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
// #include <nlohmann/json.hpp>
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
using namespace std::string_literals;
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
    unsigned word_size = lines[0].size();
    vector<unordered_map<char, unsigned>> letters_count(word_size);
    for (auto line : lines) {
        for (auto [i, c] : vw::enumerate(line)) {
            letters_count[i][c]++;
        }
    }
    string word;
    std::fill_n(std::back_inserter(word), word_size, '_');
    for (auto [i, cn] : vw::enumerate(letters_count)) {
        unsigned max_count{0};
        for (auto& [c, n] : cn) {
            if (n > max_count) {
                word[i] = c;
                max_count = n;
            }
        }
    }
    println("Resultado da parte 1: {}", word);
};

void parte_2(std::vector<std::string>& lines) {
    unsigned word_size = lines[0].size();
    vector<unordered_map<char, unsigned>> letters_count(word_size);
    for (auto line : lines) {
        for (auto [i, c] : vw::enumerate(line)) {
            letters_count[i][c]++;
        }
    }
    string word;
    std::fill_n(std::back_inserter(word), word_size, '_');
    for (auto [i, cn] : vw::enumerate(letters_count)) {
        unsigned min_count{std::numeric_limits<unsigned>::max()};
        for (auto& [c, n] : cn) {
            if (n < min_count) {
                word[i] = c;
                min_count = n;
            }
        }
    }
    println("Resultado da parte 2: {}", word);
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
