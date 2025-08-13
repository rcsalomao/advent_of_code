// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <type_traits>
// #include <json.hpp>
#include <math.h>
#include <openssl/md5.h>

#include <deque>
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
using std::deque;
using std::from_range;
using std::map;
using std::print;
using std::println;
using std::regex;
using std::regex_match;
using std::regex_search;
using std::set;
using std::smatch;
using std::string;
using std::string_view;
using std::to_string;
using std::tuple;
using std::unordered_map;
using std::unordered_set;
using std::vector;

// using nlohmann::json;

vector<string> split(const string& str, string_view delim) {
    return vector<string>{std::from_range,
                          str | vw::split(delim) | vw::transform([](auto a) {
                              return string{std::from_range, a};
                          })};
}

std::vector<std::string> find_all(std::string& line, std::regex& pattern) {
    std::vector<std::string> matches;
    // std::for_each(std::sregex_iterator(line.begin(), line.end(), pattern),
    //               std::sregex_iterator(),
    //               [&matches](std::smatch i) { matches.push_back(i.str()); });
    for (std::sregex_iterator it{line.begin(), line.end(), pattern};
         it != std::sregex_iterator{}; it++) {
        matches.push_back(it->str());
    }
    return matches;
}

struct State {
    int depth{0};
    bool garbage{false};
    bool ignore_next{false};
};

int calc_score(string s) {
    State state;
    vector<int> depths;
    for (char c : s) {
        if (state.ignore_next) {
            state.ignore_next = false;
        } else if (c == '!') {
            state.ignore_next = true;
        } else if (c == '<') {
            state.garbage = true;
        } else if (c == '>') {
            state.garbage = false;
        } else if (c == '{' and !state.garbage) {
            state.depth += 1;
            depths.push_back(state.depth);
        } else if (c == '}' and !state.garbage) {
            state.depth -= 1;
        }
    }
    return std::reduce(depths.begin(), depths.end(), 0);
}

void parte_1(vector<string>& lines) {
    int score{-1};
    for (auto line : lines) {
        score = calc_score(line);
    }
    println("Resultado da parte 1: {}", score);
}

int calc_garbage(string s) {
    State state;
    int garbage{0};
    for (char c : s) {
        if (state.garbage and c == '<' and !state.ignore_next) garbage++;
        if (state.garbage and c != '<' and c != '>' and c != '!' and
            !state.ignore_next) {
            garbage++;
        }
        if (state.ignore_next) {
            state.ignore_next = false;
        } else if (c == '!') {
            state.ignore_next = true;
        } else if (c == '<') {
            state.garbage = true;
        } else if (c == '>') {
            state.garbage = false;
        } else if (c == '{' and !state.garbage) {
            state.depth += 1;
        } else if (c == '}' and !state.garbage) {
            state.depth -= 1;
        }
    }
    return garbage;
}

void parte_2(vector<string>& lines) {
    int garbage{-1};
    for (auto line : lines) {
        garbage = calc_garbage(line);
    }
    println("Resultado da parte 2: {}", garbage);
}

int main(int, char** argv) {
    std::print("\n");

    std::ifstream input;
    input.open(argv[1]);

    string line;
    vector<string> lines;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    input.close();

    parte_1(lines);
    parte_2(lines);

    return 0;
}
