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
    vector<long> mask{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
                      1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    size_t clock_signal_size{mask.size()};
    vector<long> clock_signal;
    unordered_map<string, long> registers{
        {"a", 0},
        {"b", 0},
        {"d", 0},
        {"c", 0},
    };
    vector<std::function<void(size_t&)>> instructions;
    regex cpy_dr_pat{R"(^cpy (-?\d+) ([a-z]))"};
    regex cpy_rr_pat{R"(^cpy ([a-z]) ([a-z]))"};
    regex inc_pat{R"(^inc ([a-z]))"};
    regex dec_pat{R"(^dec ([a-z]))"};
    regex jnz_rd_pat{R"(^jnz ([a-z]) (-?\d+))"};
    regex jnz_dd_pat{R"(^jnz (-?\d+) (-?\d+))"};
    regex out_pat{R"(^out ([a-z]))"};
    for (auto line : lines) {
        smatch m;
        if (regex_match(line, m, cpy_dr_pat)) {
            int val{stoi(m[1].str())};
            string reg{m[2]};
            instructions.emplace_back([&registers, reg, val](size_t& i) {
                registers[reg] = val;
                i++;
            });
        } else if (regex_match(line, m, cpy_rr_pat)) {
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
        } else if (regex_match(line, m, jnz_rd_pat)) {
            string reg{m[1]};
            int val{stoi(m[2].str())};
            instructions.emplace_back([&registers, reg, val](size_t& i) {
                if (registers[reg] == 0) {
                    i++;
                } else {
                    i += val;
                }
            });
        } else if (regex_match(line, m, jnz_dd_pat)) {
            int val1{stoi(m[1].str())};
            int val2{stoi(m[2].str())};
            instructions.emplace_back([val1, val2](size_t& i) {
                if (val1 == 0) {
                    i++;
                } else {
                    i += val2;
                }
            });
        } else if (regex_match(line, m, out_pat)) {
            string reg{m[1]};
            instructions.emplace_back(
                [&registers, reg, &clock_signal, clock_signal_size](size_t& i) {
                    if (clock_signal.size() < clock_signal_size) {
                        clock_signal.push_back(registers[reg]);
                        i++;
                    } else {
                        i = std::numeric_limits<size_t>::max() - 1;
                    }
                });
        }
    }

    long first_a{0};
    for (long initial_a : vw::iota(0l, 1000l)) {
        registers["a"] = initial_a;
        registers["b"] = 0;
        registers["c"] = 0;
        registers["d"] = 0;
        clock_signal.clear();

        size_t i{0};
        while (i < instructions.size()) {
            instructions[i](i);
        }

        bool found{true};
        for (size_t i : vw::iota(0u, mask.size())) {
            if (mask[i] * clock_signal[i] != mask[i]) {
                found = false;
                break;
            }
        }
        if (found) {
            // println("{}", clock_signal);
            first_a = initial_a;
            break;
        }
    }

    println("Resultado da parte 1: {}", first_a);
}

void parte_2(std::vector<std::string>& lines) {
    println("Resultado da parte 2: {}", "");
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
