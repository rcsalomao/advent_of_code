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

struct Node {
    string instruction;
    std::function<void(size_t&)> f;
};

void parte_1(std::vector<std::string>& lines) {
    unordered_map<string, long> registers{
        {"a", 7},
        {"b", 0},
        {"d", 0},
        {"c", 0},
    };
    vector<Node> instructions;
    regex cpy_num_pat{R"(^cpy (-?\d+) ([a-z]))"};
    regex cpy_reg_pat{R"(^cpy ([a-z]) ([a-z]))"};
    regex inc_pat{R"(^inc ([a-z]))"};
    regex dec_pat{R"(^dec ([a-z]))"};
    regex jnz_num_reg_pat{R"(^jnz (-?\d+) ([a-z]))"};
    regex jnz_reg_num_pat{R"(^jnz ([a-z]) (-?\d+))"};
    regex tgl_pat{R"(^tgl ([a-z]))"};
    for (auto line : lines) {
        smatch m;
        if (regex_match(line, m, cpy_num_pat)) {
            int val{stoi(m[1].str())};
            string reg{m[2]};
            instructions.emplace_back(m[0].str(),
                                      [&registers, reg, val](size_t& i) {
                                          registers[reg] = val;
                                          i++;
                                      });
        } else if (regex_match(line, m, cpy_reg_pat)) {
            string reg_src{m[1]};
            string reg_dest{m[2]};
            instructions.emplace_back(
                m[0].str(), [&registers, reg_dest, reg_src](size_t& i) {
                    registers[reg_dest] = registers[reg_src];
                    i++;
                });
        } else if (regex_match(line, m, inc_pat)) {
            string reg{m[1]};
            instructions.emplace_back(m[0].str(), [&registers, reg](size_t& i) {
                registers[reg]++;
                i++;
            });
        } else if (regex_match(line, m, dec_pat)) {
            string reg{m[1]};
            instructions.emplace_back(m[0].str(), [&registers, reg](size_t& i) {
                registers[reg]--;
                i++;
            });
        } else if (regex_match(line, m, jnz_reg_num_pat)) {
            string reg{m[1]};
            int val{stoi(m[2].str())};
            instructions.emplace_back(m[0].str(),
                                      [&registers, reg, val](size_t& i) {
                                          if (registers[reg] == 0) {
                                              i++;
                                          } else {
                                              i += val;
                                          }
                                      });
        } else if (regex_match(line, m, jnz_num_reg_pat)) {
            int val{stoi(m[1].str())};
            string reg{m[2]};
            instructions.emplace_back(m[0].str(),
                                      [&registers, val, reg](size_t& i) {
                                          if (val == 0) {
                                              i++;
                                          } else {
                                              i += registers[reg];
                                          }
                                      });
        } else if (regex_match(line, m, tgl_pat)) {
            string reg{m[1]};
            instructions.emplace_back(m[0].str(), [&registers, &instructions,
                                                   reg](size_t& i) {
                long v{registers[reg]};
                if (i + v >= instructions.size()) {
                    i++;
                    return;
                }
                regex inc_pat{R"(^inc ([a-z]))"};
                regex one_argument_pat{R"(^\w{3} ([a-z]))"};
                regex jnz_rd_pat{R"(^jnz ([a-z]) (-?\d+))"};
                regex jnz_rr_pat{R"(^jnz ([a-z]) ([a-z]))"};
                regex jnz_dr_pat{R"(^jnz (-?\d+) ([a-z]))"};
                regex cpy_rd_pat{R"(^cpy ([a-z]) (-?\d+))"};
                regex cpy_rr_pat{R"(^cpy ([a-z]) ([a-z]))"};
                regex cpy_dr_pat{R"(^cpy (-?\d+) ([a-z]))"};
                Node& n = instructions[i + v];
                smatch m;
                if (regex_match(n.instruction, m, inc_pat)) {
                    string instr{"dec "};
                    string reg_dec{m[1]};
                    instr += reg_dec;
                    instructions[i + v] =
                        std::move(Node{instr, [&registers, reg_dec](size_t& i) {
                                           registers[reg_dec]--;
                                           i++;
                                       }});
                } else if (regex_match(n.instruction, m, one_argument_pat)) {
                    string instr{"inc "};
                    string reg_inc{m[1]};
                    instr += reg_inc;
                    instructions[i + v] =
                        std::move(Node{instr, [&registers, reg_inc](size_t& i) {
                                           registers[reg_inc]++;
                                           i++;
                                       }});
                } else if (regex_match(n.instruction, m, jnz_rd_pat)) {
                    string instr{"cpy "};
                    instr += m[1].str();
                    instr += " ";
                    instr += m[2].str();
                    instructions[i + v] =
                        std::move(Node{instr, [](size_t& i) { i++; }});
                } else if (regex_match(n.instruction, m, jnz_dr_pat)) {
                    string instr{"cpy "};
                    int val_cpy{stoi(m[1].str())};
                    string reg_cpy{m[2]};
                    instr += m[1].str();
                    instr += " ";
                    instr += reg_cpy;
                    instructions[i + v] = std::move(
                        Node{instr, [&registers, reg_cpy, val_cpy](size_t& i) {
                                 registers[reg_cpy] = val_cpy;
                                 i++;
                             }});
                } else if (regex_match(n.instruction, m, jnz_rr_pat)) {
                    string instr{"cpy "};
                    string reg1_cpy{m[1]};
                    string reg2_cpy{m[2]};
                    instr += reg1_cpy;
                    instr += " ";
                    instr += reg2_cpy;
                    instructions[i + v] = std::move(Node{
                        instr, [&registers, reg1_cpy, reg2_cpy](size_t& i) {
                            registers[reg2_cpy] = registers[reg1_cpy];
                            i++;
                        }});
                } else if (regex_match(n.instruction, m, cpy_rd_pat)) {
                    string instr{"jnz "};
                    string reg_jnz{m[1]};
                    int val_jnz{stoi(m[2].str())};
                    instr += reg_jnz;
                    instr += " ";
                    instr += m[2].str();
                    instructions[i + v] = std::move(
                        Node{instr, [&registers, reg_jnz, val_jnz](size_t& i) {
                                 if (registers[reg_jnz] == 0) {
                                     i++;
                                 } else {
                                     i += val_jnz;
                                 }
                             }});
                } else if (regex_match(n.instruction, m, cpy_rr_pat)) {
                    string instr{"jnz "};
                    string reg1_jnz{m[1]};
                    string reg2_jnz{m[2]};
                    instr += reg1_jnz;
                    instr += " ";
                    instr += reg2_jnz;
                    instructions[i + v] = std::move(Node{
                        instr, [&registers, reg1_jnz, reg2_jnz](size_t& i) {
                            if (registers[reg1_jnz] == 0) {
                                i++;
                            } else {
                                i += registers[reg2_jnz];
                            }
                        }});
                } else if (regex_match(n.instruction, m, cpy_dr_pat)) {
                    string instr{"jnz "};
                    int val_jnz{stoi(m[1].str())};
                    string reg_jnz{m[2]};
                    instr += m[1].str();
                    instr += " ";
                    instr += reg_jnz;
                    instructions[i + v] = std::move(
                        Node{instr, [&registers, val_jnz, reg_jnz](size_t& i) {
                                 if (val_jnz == 0) {
                                     i++;
                                 } else {
                                     i += registers[reg_jnz];
                                 }
                             }});
                }
                i++;
                return;
            });
        }
    }

    size_t i{0};
    while (i < instructions.size()) {
        instructions[i].f(i);
    }

    println("Resultado da parte 1: {}", registers["a"]);
}

void parte_2(std::vector<std::string>& lines) {
    long n{12};
    vector<long> vl{from_range, vw::iota(1, n + 1)};
    long v{95 * 95 + std::reduce(vl.begin(), vl.end(), 1, std::multiplies<>{})};

    println("Resultado da parte 2: {}", v);
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
