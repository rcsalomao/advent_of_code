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

void parte_1(std::vector<std::string>& lines) {
    for (auto line : lines) {
        long total{0};
        for (size_t i = 0; i < line.size(); i++) {
            if (line[i] == line[(i + 1) % line.size()])
                total += stoi(string{line[i]});
        }
        println("Resultado da parte 1: {}", total);
    }
}

void parte_2(std::vector<std::string>& lines) {
    for (auto line : lines) {
        long total{0};
        for (size_t i = 0; i < line.size(); i++) {
            if (line[i] == line[(i + line.size() / 2) % line.size()])
                total += stoi(string{line[i]});
        }
        println("Resultado da parte 2: {}", total);
    }
}

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
