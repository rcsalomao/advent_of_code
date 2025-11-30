// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <type_traits>
// #include <nlohmann/json.hpp>
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

string expand_bitset(string a) {
    string reversed_a{string{from_range, a | vw::reverse}};
    auto flip = [](string_view s) {
        return string{from_range, s | vw::transform([](char sv) {
                                      return sv == '0' ? '1' : '0';
                                  })};
    };
    return {a + "0" + flip(reversed_a)};
}

string calc_checksum(string_view a) {
    auto reduce = [](string_view sv) {
        string r;
        for (size_t i{0}; i < sv.size() / 2; i++) {
            r += sv[2 * i] == sv[2 * i + 1] ? "1" : "0";
        }
        return r;
    };
    string o = reduce(a);
    while (o.size() % 2 == 0) {
        o = reduce(o);
    }
    return o;
}

void parte_1(std::vector<std::string>& lines) {
    size_t disk_size{272};
    string bs{lines[0]};
    while (bs.size() < disk_size) {
        bs = expand_bitset(bs);
    }
    string final_bs = bs.substr(0, disk_size);
    println("Resultado da parte 1: {}", calc_checksum(final_bs));
}

void parte_2(std::vector<std::string>& lines) {
    size_t disk_size{35651584};
    string bs{lines[0]};
    while (bs.size() < disk_size) {
        bs = expand_bitset(bs);
    }
    string final_bs = bs.substr(0, disk_size);
    println("Resultado da parte 2: {}", calc_checksum(final_bs));
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
