// #include <Eigen/Dense>
#include <algorithm>
// #include <boost/functional/hash.hpp>
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
    unordered_map<string, int> tape{
        {"children", 3}, {"cats", 7},     {"samoyeds", 2}, {"pomeranians", 3},
        {"akitas", 0},   {"vizslas", 0},  {"goldfish", 5}, {"trees", 3},
        {"cars", 2},     {"perfumes", 1},
    };
    vector<unordered_map<string, int>> sue_maps;
    string sue_number{""};  // It is a string! >:c
    for (auto line : lines) {
        std::smatch m;
        std::regex_match(line, m, std::regex{R"(Sue (\d+): (.*))"});
        string s{m[2]};
        vector<string> vs = split(s, ", ");
        bool identified{true};
        for (string& s : vs) {
            auto a = split(s, ": ");
            if (tape[a[0]] != std::stoi(a[1])) {
                identified = false;
                break;
            };
        }
        if (identified) sue_number = m[1];
    }
    println("Resultado da parte 1: {}", sue_number);
};

void parte_2(std::vector<std::string>& lines) {
    unordered_map<string, int> tape{
        {"children", 3}, {"cats", 7},     {"samoyeds", 2}, {"pomeranians", 3},
        {"akitas", 0},   {"vizslas", 0},  {"goldfish", 5}, {"trees", 3},
        {"cars", 2},     {"perfumes", 1},
    };
    vector<unordered_map<string, int>> sue_maps;
    string sue_number{""};  // It is a string! >:c
    for (auto line : lines) {
        std::smatch m;
        std::regex_match(line, m, std::regex{R"(Sue (\d+): (.*))"});
        string s{m[2]};
        vector<string> vs = split(s, ", ");
        bool identified{true};
        for (string& s : vs) {
            auto a = split(s, ": ");
            if (a[0] == "cats" or a[0] == "trees") {
                if (tape[a[0]] > std::stoi(a[1])) {
                    identified = false;
                    break;
                };
            } else if (a[0] == "pomeranians" or a[0] == "goldfish") {
                if (tape[a[0]] < std::stoi(a[1])) {
                    identified = false;
                    break;
                };
            } else {
                if (tape[a[0]] != std::stoi(a[1])) {
                    identified = false;
                    break;
                };
            }
        }
        if (identified) sue_number = m[1];
    }
    println("Resultado da parte 2: {}", sue_number);
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
