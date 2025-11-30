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

void parte_1(vector<string>& lines) {
    vector<int> lengths = split(lines[0], ",") |
                          vw::transform([](auto s) { return stoi(s); }) |
                          rg::to<vector<int>>();
    uint cursor{0};
    uint skip{0};
    vector<int> seq{from_range, vw::iota(0, 256)};
    size_t seq_size{seq.size()};
    for (int l : lengths) {
        if ((size_t)l > seq_size) continue;
        rg::rotate(seq, std::begin(seq) + cursor);
        rg::reverse(seq.begin(), seq.begin() + l);
        rg::rotate(seq, std::end(seq) - cursor);
        cursor = (cursor + l + skip) % seq_size;
        skip++;
    }
    println("Resultado da parte 1: {}", seq[0] * seq[1]);
}

void parte_2(vector<string>& lines) {
    vector<int> lengths =
        lines[0] | vw::transform([](char c) { return static_cast<int>(c); }) |
        rg::to<vector<int>>();
    lengths.insert(lengths.end(), {17, 31, 73, 47, 23});

    uint cursor{0};
    uint skip{0};
    vector<int> seq{from_range, vw::iota(0, 256)};
    size_t seq_size{seq.size()};
    uint iters{64};
    while (iters--) {
        for (int l : lengths) {
            if ((size_t)l > seq_size) continue;
            rg::rotate(seq, std::begin(seq) + cursor);
            rg::reverse(seq.begin(), seq.begin() + l);
            rg::rotate(seq, std::end(seq) - cursor);
            cursor = (cursor + l + skip) % seq_size;
            skip++;
        }
    }
    vector<int> dh;
    for (uint i{0}; i < 16; i++) {
        int v{0};
        for (int j :
             rg::subrange(seq.begin() + i * 16, seq.begin() + i * 16 + 16)) {
            v ^= j;
        }
        dh.push_back(v);
    }
    string hash;
    for (int i : dh) {
        hash += std::format("{:02x}", i);
    }
    println("Resultado da parte 2: {}", hash);
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
