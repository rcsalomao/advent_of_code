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

ulong decompress(string s, bool parte_2) {
    if (!s.contains('(')) {
        return s.size();
    } else {
        std::regex pat{R"(\((\d+)x(\d+)\))", std::regex::optimize};
        std::smatch m;
        std::regex_search(s, m, pat);
        long stt{m.position()};
        long len{m.length()};
        long pad{std::stoi(m.str(1))};
        long times{std::stoi(m.str(2))};
        return stt +
               (parte_2 ? decompress(s.substr(stt + len, pad), parte_2) : pad) *
                   times +
               decompress(s.substr(stt + len + pad), parte_2);
    }
};

void parte_1(std::vector<std::string>& lines) {
    string input = lines[0];
    auto res = decompress(input, false);
    println("Resultado da parte 1: {}", res);
};

void parte_2(std::vector<std::string>& lines) {
    string input = lines[0];
    auto res = decompress(input, true);
    println("Resultado da parte 2: {}", res);
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
