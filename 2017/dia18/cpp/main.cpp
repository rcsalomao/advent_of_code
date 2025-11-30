// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <deque>
#include <format>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <print>
#include <queue>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
// #include <nlohmann/json.hpp>
#include <math.h>
// #include <openssl/md5.h>

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
using std::shared_ptr;
using std::smatch;
using std::string;
using std::string_view;
using std::to_string;
using std::tuple;
using std::unique_ptr;
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

void parte_1(vector<string>& lines) {
    unordered_map<string, long long> registers{{"snd", 0}, {"rcv", 0}};
    vector<std::function<void(size_t&)>> instructions;
    regex set_rd_pat{R"(^set ([a-z]) (-?\d+))"};
    regex set_rr_pat{R"(^set ([a-z]) ([a-z]))"};
    regex mul_rd_pat{R"(^mul ([a-z]) (-?\d+))"};
    regex mul_rr_pat{R"(^mul ([a-z]) ([a-z]))"};
    regex jgz_rd_pat{R"(^jgz ([a-z]) (-?\d+))"};
    regex jgz_rr_pat{R"(^jgz ([a-z]) ([a-z]))"};
    regex add_rd_pat{R"(^add ([a-z]) (-?\d+))"};
    regex add_rr_pat{R"(^add ([a-z]) ([a-z]))"};
    regex mod_rd_pat{R"(^mod ([a-z]) (-?\d+))"};
    regex mod_rr_pat{R"(^mod ([a-z]) ([a-z]))"};
    regex snd_pat{R"(^snd ([a-z]))"};
    regex rcv_pat{R"(^rcv ([a-z]))"};
    for (auto line : lines) {
        smatch m;
        if (regex_match(line, m, set_rd_pat)) {
            string reg{m[1]};
            long long val{stol(m[2].str())};
            registers[reg] = 0;
            instructions.emplace_back([&registers, reg, val](size_t& i) {
                registers[reg] = val;
                i++;
            });
        } else if (regex_match(line, m, set_rr_pat)) {
            string reg1{m[1]};
            string reg2{m[2]};
            registers[reg1] = 0;
            registers[reg2] = 0;
            instructions.emplace_back([&registers, reg1, reg2](size_t& i) {
                registers[reg1] = registers[reg2];
                i++;
            });
        } else if (regex_match(line, m, mul_rd_pat)) {
            string reg{m[1]};
            long long val{stol(m[2].str())};
            registers[reg] = 0;
            instructions.emplace_back([&registers, reg, val](size_t& i) {
                registers[reg] *= val;
                i++;
            });
        } else if (regex_match(line, m, mul_rr_pat)) {
            string reg1{m[1]};
            string reg2{m[2]};
            registers[reg1] = 0;
            registers[reg2] = 0;
            instructions.emplace_back([&registers, reg1, reg2](size_t& i) {
                registers[reg1] *= registers[reg2];
                i++;
            });
        } else if (regex_match(line, m, jgz_rd_pat)) {
            string reg{m[1]};
            long long val{stol(m[2].str())};
            registers[reg] = 0;
            instructions.emplace_back([&registers, reg, val](size_t& i) {
                if (registers[reg] > 0) {
                    i += val;
                } else {
                    i++;
                }
            });
        } else if (regex_match(line, m, jgz_rr_pat)) {
            string reg1{m[1]};
            string reg2{m[2]};
            registers[reg1] = 0;
            registers[reg2] = 0;
            instructions.emplace_back([&registers, reg1, reg2](size_t& i) {
                if (registers[reg1] > 0) {
                    i += registers[reg2];
                } else {
                    i++;
                }
            });
        } else if (regex_match(line, m, add_rd_pat)) {
            string reg{m[1]};
            long long val{stol(m[2].str())};
            registers[reg] = 0;
            instructions.emplace_back([&registers, reg, val](size_t& i) {
                registers[reg] += val;
                i++;
            });
        } else if (regex_match(line, m, add_rr_pat)) {
            string reg1{m[1]};
            string reg2{m[2]};
            registers[reg1] = 0;
            registers[reg2] = 0;
            instructions.emplace_back([&registers, reg1, reg2](size_t& i) {
                registers[reg1] += registers[reg2];
                i++;
            });
        } else if (regex_match(line, m, mod_rd_pat)) {
            string reg{m[1]};
            long long val{stol(m[2].str())};
            registers[reg] = 0;
            instructions.emplace_back([&registers, reg, val](size_t& i) {
                registers[reg] %= val;
                i++;
            });
        } else if (regex_match(line, m, mod_rr_pat)) {
            string reg1{m[1]};
            string reg2{m[2]};
            registers[reg1] = 0;
            registers[reg2] = 0;
            instructions.emplace_back([&registers, reg1, reg2](size_t& i) {
                registers[reg1] %= registers[reg2];
                i++;
            });
        } else if (regex_match(line, m, snd_pat)) {
            string reg{m[1]};
            registers[reg] = 0;
            instructions.emplace_back([&registers, reg](size_t& i) {
                registers["snd"] = registers[reg];
                i++;
            });
        } else if (regex_match(line, m, rcv_pat)) {
            string reg{m[1]};
            registers[reg] = 0;
            instructions.emplace_back([&registers, reg](size_t& i) {
                if (registers[reg] != 0) {
                    registers["rcv"] = registers["snd"];
                }
                i++;
            });
        }
    }
    size_t i{0};
    while (i < instructions.size()) {
        instructions[i](i);
        if (registers["rcv"] != 0) break;
    }

    println("Resultado da parte 1: {}", registers["rcv"]);
}

struct Program {
    vector<string> instr;
    map<string, long long> reg;
    std::queue<long long> q;
    int ip = 0;
    long long send_count = 0;
    bool waiting = false;
};

long long get_val(const string& s, map<string, long long>& reg) {
    if (isalpha(s[0])) return reg[s];
    return stoll(s);
}

void parte_2(vector<string>& lines) {
    vector<Program> P(2);
    P[0].instr = lines;
    P[1].instr = lines;
    P[0].reg["p"] = 0;
    P[1].reg["p"] = 1;

    while (true) {
        bool progress = false;

        for (int i = 0; i < 2; i++) {
            if (P[i].ip < 0 || P[i].ip >= (int)P[i].instr.size()) {
                println("Instruction index out of bounds!");
                exit(1);
            }
            if (P[i].waiting && P[i].q.empty()) continue;

            P[i].waiting = false;
            std::istringstream ss(P[i].instr[P[i].ip]);
            string op, X, Y;
            ss >> op >> X >> Y;

            if (op == "snd") {
                long long v = get_val(X, P[i].reg);
                P[1 - i].q.push(v);
                P[i].send_count++;
                P[i].ip++;
            } else if (op == "set") {
                P[i].reg[X] = get_val(Y, P[i].reg);
                P[i].ip++;
            } else if (op == "add") {
                P[i].reg[X] += get_val(Y, P[i].reg);
                P[i].ip++;
            } else if (op == "mul") {
                P[i].reg[X] *= get_val(Y, P[i].reg);
                P[i].ip++;
            } else if (op == "mod") {
                P[i].reg[X] %= get_val(Y, P[i].reg);
                P[i].ip++;
            } else if (op == "rcv") {
                if (!P[i].q.empty()) {
                    P[i].reg[X] = P[i].q.front();
                    P[i].q.pop();
                    P[i].ip++;
                } else {
                    P[i].waiting = true;
                }
            } else if (op == "jgz") {
                if (get_val(X, P[i].reg) > 0)
                    P[i].ip += get_val(Y, P[i].reg);
                else
                    P[i].ip++;
            }
            if (!P[i].waiting) progress = true;
        }
        if (!progress) break;
    }

    println("Resultado da parte 2: {}", P[1].send_count);
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
