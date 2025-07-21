// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <type_traits>
// #include <json.hpp>
#include <math.h>
#include <openssl/md5.h>

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
using std::from_range;
using std::map;
using std::print;
using std::println;
using std::regex;
using std::regex_match;
using std::regex_search;
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

string dec_to_binary(unsigned n) {
    string r = "";
    while (n > 0) {
        unsigned bit = n & 1;
        r += to_string(bit);
        n = n >> 1;
    }
    std::reverse(r.begin(), r.end());
    return r;
}

unsigned binary_to_dec(string s) {
    unsigned r{0};
    for (auto [i, c] : vw::enumerate(s | vw::reverse)) {
        r += (c == '0' ? 0 : 1) * std::pow(2, i);
    }
    return r;
}

void parte_1(std::vector<std::string>& lines) {
    int num_elves{stoi(lines[0])};
    string bin{dec_to_binary(num_elves)};
    bin = bin.substr(1, bin.size() - 1) + "1";
    auto res = binary_to_dec(bin);

    println("Resultado da parte 1: {}", res);
}

void parte_2(std::vector<std::string>& lines) {
    auto simulate_n = [](uint n_results) {
        for (int num_elves : vw::iota(1u, n_results + 1)) {
            vector<int> elves{from_range, vw::iota(1, num_elves + 1)};
            size_t idx{0};
            while (elves.size() != 1) {
                auto elv_size = elves.size();
                size_t center_elv_idx{elv_size / 2};
                auto a = (center_elv_idx + idx) % elv_size;
                elves.erase(elves.begin() + a);
                if (idx > a) idx--;
                idx = (idx + 1) % elves.size();
            }
            println("{}: {}", num_elves, elves[0]);
        }
    };
    // simulate_n(30);
    auto calc_n = [](uint n_results) {
        for (uint num_elves : vw::iota(1u, n_results + 1)) {
            uint b{1};
            while (b < num_elves) b *= 3;
            uint a{b / 3};
            uint n{0};
            if (num_elves < b - a) {
                n = num_elves - a;
            } else {
                n = 2 * num_elves - b;
            }
            println("{}: {}", num_elves, n);
        }
    };
    // calc_n(30);

    int num_elves{stoi(lines[0])};
    int b{1};
    while (b < num_elves) b *= 3;
    int a{b / 3};
    int n{0};
    if (num_elves < b - a) {
        n = num_elves - a;
    } else {
        n = 2 * num_elves - b;
    }
    println("Resultado da parte 2: {}", n);
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
