// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <type_traits>
// #include <nlohmann/json.hpp>
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

vector<string> find_all(string& line, regex& pattern) {
    vector<string> matches;
    // std::for_each(std::sregex_iterator(line.begin(), line.end(), pattern),
    //               std::sregex_iterator(),
    //               [&matches](smatch i) { matches.push_back(i.str()); });
    for (std::sregex_iterator it{line.begin(), line.end(), pattern};
         it != std::sregex_iterator{}; it++) {
        matches.push_back(it->str());
    }
    return matches;
}

struct Generator {
    ulong seed;
    ulong factor;
    ulong sample() {
        seed = (seed * factor) % 2147483647;
        return seed;
    }
};

string dec_to_binary(ulong n) {
    string res = "";
    while (n > 0) {
        int bit = n & 1;
        res.push_back('0' + bit);
        n = n >> 1;
    }
    size_t s{32 - res.size()};
    while (s--) {
        res.push_back('0');
    }
    rg::reverse(res);
    return res;
}

bool compare_binaries(string_view b1, string_view b2) {
    return b1.substr(16, 16) == b2.substr(16, 16);
}

void parte_1(vector<string>& lines) {
    vector<Generator> generators;
    regex pat{R"(Generator (\w+) starts with (\d+))"};
    for (auto line : lines) {
        smatch m;
        regex_match(line, m, pat);
        if (m[1] == "A") generators.emplace_back(stoul(m[2].str()), 16807);
        if (m[1] == "B") generators.emplace_back(stoul(m[2].str()), 48271);
    }

    int total{0};
    for (size_t i = 0; i < 40000000; i++) {
        ulong sample_a = generators[0].sample();
        ulong sample_b = generators[1].sample();
        // string bin_a = dec_to_binary(sample_a);
        // string bin_b = dec_to_binary(sample_b);
        // if (compare_binaries(bin_a, bin_b)) total++;
        std::bitset<16> bin_a{sample_a};
        std::bitset<16> bin_b{sample_b};
        if (bin_a == bin_b) total++;
    }

    println("Resultado da parte 1: {}", total);
}

struct Generator2 {
    ulong seed;
    ulong factor;
    ulong mod;
    ulong sample() {
        seed = (seed * factor) % 2147483647;
        while (seed % mod) {
            seed = (seed * factor) % 2147483647;
        }
        return seed;
    }
};

void parte_2(vector<string>& lines) {
    vector<Generator2> generators;
    regex pat{R"(Generator (\w+) starts with (\d+))"};
    for (auto line : lines) {
        smatch m;
        regex_match(line, m, pat);
        if (m[1] == "A") generators.emplace_back(stoul(m[2].str()), 16807, 4);
        if (m[1] == "B") generators.emplace_back(stoul(m[2].str()), 48271, 8);
    }

    int total{0};
    for (size_t i = 0; i < 5000000; i++) {
        ulong sample_a = generators[0].sample();
        ulong sample_b = generators[1].sample();
        // string bin_a = dec_to_binary(sample_a);
        // string bin_b = dec_to_binary(sample_b);
        // if (compare_binaries(bin_a, bin_b)) total++;
        std::bitset<16> bin_a{sample_a};
        std::bitset<16> bin_b{sample_b};
        if (bin_a == bin_b) total++;
    }

    println("Resultado da parte 2: {}", total);
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
