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

void parte_1(std::vector<std::string>& lines) {
    unordered_map<string, long> registers{
        {"a", 0},
        {"b", 0},
        {"d", 0},
        {"c", 0},
    };
    vector<std::function<void(size_t&)>> instructions;
    regex cpy_num_pat{R"(^cpy (-?\d+) ([a-z]))"};
    regex cpy_reg_pat{R"(^cpy ([a-z]) ([a-z]))"};
    regex inc_pat{R"(^inc ([a-z]))"};
    regex dec_pat{R"(^dec ([a-z]))"};
    regex jnz_reg_pat{R"(^jnz ([a-z]) (-?\d+))"};
    regex jnz_num_pat{R"(^jnz (-?\d+) (-?\d+))"};
    for (auto line : lines) {
        smatch m;
        if (regex_match(line, m, cpy_num_pat)) {
            int val{stoi(m[1].str())};
            string reg{m[2]};
            instructions.emplace_back([&registers, reg, val](size_t& i) {
                registers[reg] = val;
                i++;
            });
        } else if (regex_match(line, m, cpy_reg_pat)) {
            string reg_src{m[1]};
            string reg_dest{m[2]};
            instructions.emplace_back(
                [&registers, reg_dest, reg_src](size_t& i) {
                    registers[reg_dest] = registers[reg_src];
                    i++;
                });
        } else if (regex_match(line, m, inc_pat)) {
            string reg{m[1]};
            instructions.emplace_back([&registers, reg](size_t& i) {
                registers[reg]++;
                i++;
            });
        } else if (regex_match(line, m, dec_pat)) {
            string reg{m[1]};
            instructions.emplace_back([&registers, reg](size_t& i) {
                registers[reg]--;
                i++;
            });
        } else if (regex_match(line, m, jnz_reg_pat)) {
            string reg{m[1]};
            int val{stoi(m[2].str())};
            instructions.emplace_back([&registers, reg, val](size_t& i) {
                if (registers[reg] == 0) {
                    i++;
                } else {
                    i += val;
                }
            });
        } else if (regex_match(line, m, jnz_num_pat)) {
            int val1{stoi(m[1].str())};
            int val2{stoi(m[2].str())};
            instructions.emplace_back([val1, val2](size_t& i) {
                if (val1 == 0) {
                    i++;
                } else {
                    i += val2;
                }
            });
        }
    }
    size_t i{0};
    while (i < instructions.size()) {
        instructions[i](i);
    }
    println("Resultado da parte 1: {}", registers["a"]);
};

void parte_2(std::vector<std::string>& lines) {
    unordered_map<string, long> registers{
        {"a", 0},
        {"b", 0},
        {"d", 0},
        {"c", 1},
    };
    vector<std::function<void(size_t&)>> instructions;
    regex cpy_num_pat{R"(^cpy (-?\d+) ([a-z]))"};
    regex cpy_reg_pat{R"(^cpy ([a-z]) ([a-z]))"};
    regex inc_pat{R"(^inc ([a-z]))"};
    regex dec_pat{R"(^dec ([a-z]))"};
    regex jnz_reg_pat{R"(^jnz ([a-z]) (-?\d+))"};
    regex jnz_num_pat{R"(^jnz (-?\d+) (-?\d+))"};
    for (auto line : lines) {
        smatch m;
        if (regex_match(line, m, cpy_num_pat)) {
            int val{stoi(m[1].str())};
            string reg{m[2]};
            instructions.emplace_back([&registers, reg, val](size_t& i) {
                registers[reg] = val;
                i++;
            });
        } else if (regex_match(line, m, cpy_reg_pat)) {
            string reg_src{m[1]};
            string reg_dest{m[2]};
            instructions.emplace_back(
                [&registers, reg_dest, reg_src](size_t& i) {
                    registers[reg_dest] = registers[reg_src];
                    i++;
                });
        } else if (regex_match(line, m, inc_pat)) {
            string reg{m[1]};
            instructions.emplace_back([&registers, reg](size_t& i) {
                registers[reg]++;
                i++;
            });
        } else if (regex_match(line, m, dec_pat)) {
            string reg{m[1]};
            instructions.emplace_back([&registers, reg](size_t& i) {
                registers[reg]--;
                i++;
            });
        } else if (regex_match(line, m, jnz_reg_pat)) {
            string reg{m[1]};
            int val{stoi(m[2].str())};
            instructions.emplace_back([&registers, reg, val](size_t& i) {
                if (registers[reg] == 0) {
                    i++;
                } else {
                    i += val;
                }
            });
        } else if (regex_match(line, m, jnz_num_pat)) {
            int val1{stoi(m[1].str())};
            int val2{stoi(m[2].str())};
            instructions.emplace_back([val1, val2](size_t& i) {
                if (val1 == 0) {
                    i++;
                } else {
                    i += val2;
                }
            });
        }
    }
    size_t i{0};
    while (i < instructions.size()) {
        instructions[i](i);
    }
    println("Resultado da parte 2: {}", registers["a"]);
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
