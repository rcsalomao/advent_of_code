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
    unordered_map<string, int> registers;
    unordered_map<string, std::function<void(string, int)>> op_map{
        {"inc", [&registers](string s, int i) { registers[s] += i; }},
        {"dec", [&registers](string s, int i) { registers[s] -= i; }},
    };
    unordered_map<string, std::function<bool(string, int)>> comp_map{
        {">", [&registers](string s, int i) { return registers[s] > i; }},
        {"<", [&registers](string s, int i) { return registers[s] < i; }},
        {">=", [&registers](string s, int i) { return registers[s] >= i; }},
        {"<=", [&registers](string s, int i) { return registers[s] <= i; }},
        {"==", [&registers](string s, int i) { return registers[s] == i; }},
        {"!=", [&registers](string s, int i) { return registers[s] != i; }},
    };
    vector<std::function<void()>> instructions;
    regex pat{R"((\w+) (\w+) (-?\d+) if (\w+) ([><=!]+) (-?\d+))"};
    for (auto line : lines) {
        smatch m;
        regex_match(line, m, pat);
        string reg{m[1]};
        string op{m[2]};
        int num{stoi(m[3])};
        string reg_test{m[4]};
        string comp{m[5]};
        int num_test{stoi(m[6])};
        registers[reg] = 0;
        registers[reg_test] = 0;
        instructions.push_back(
            [reg, op, num, reg_test, comp, num_test, &op_map, &comp_map]() {
                if (comp_map[comp](reg_test, num_test)) op_map[op](reg, num);
            });
    }
    for (auto& ins : instructions) {
        ins();
    }
    int max = std::numeric_limits<int>::min();
    for (auto [k, v] : registers) {
        max = std::max(max, v);
    }
    println("Resultado da parte 1: {}", max);
}

void parte_2(vector<string>& lines) {
    unordered_map<string, int> registers;
    unordered_map<string, std::function<void(string, int)>> op_map{
        {"inc", [&registers](string s, int i) { registers[s] += i; }},
        {"dec", [&registers](string s, int i) { registers[s] -= i; }},
    };
    unordered_map<string, std::function<bool(string, int)>> comp_map{
        {">", [&registers](string s, int i) { return registers[s] > i; }},
        {"<", [&registers](string s, int i) { return registers[s] < i; }},
        {">=", [&registers](string s, int i) { return registers[s] >= i; }},
        {"<=", [&registers](string s, int i) { return registers[s] <= i; }},
        {"==", [&registers](string s, int i) { return registers[s] == i; }},
        {"!=", [&registers](string s, int i) { return registers[s] != i; }},
    };
    vector<std::function<void()>> instructions;
    regex pat{R"((\w+) (\w+) (-?\d+) if (\w+) ([><=!]+) (-?\d+))"};
    for (auto line : lines) {
        smatch m;
        regex_match(line, m, pat);
        string reg{m[1]};
        string op{m[2]};
        int num{stoi(m[3])};
        string reg_test{m[4]};
        string comp{m[5]};
        int num_test{stoi(m[6])};
        registers[reg] = 0;
        registers[reg_test] = 0;
        instructions.push_back(
            [reg, op, num, reg_test, comp, num_test, &op_map, &comp_map]() {
                if (comp_map[comp](reg_test, num_test)) op_map[op](reg, num);
            });
    }
    int max = std::numeric_limits<int>::min();
    for (auto& ins : instructions) {
        ins();
        for (auto [k, v] : registers) {
            max = std::max(max, v);
        }
    }
    println("Resultado da parte 2: {}", max);
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
