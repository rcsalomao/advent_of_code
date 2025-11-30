// #include <Eigen/Dense>
// #include <nlohmann/json.hpp>
// #include <openssl/md5.h>
#include <bits/stdc++.h>

namespace rg = std::ranges;
namespace vw = std::views;
using namespace std::string_literals;
using namespace std::string_view_literals;
using std::bitset;
using std::deque;
using std::from_range;
using std::map;
using std::pair;
using std::print;
using std::println;
using std::priority_queue;
using std::queue;
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

void pr(auto&& val, auto&&... vals) {
    std::stringstream ss;
    ss << std::format("{}", val);
    (..., (ss << " " << std::format("{}", vals)));
    println("{}", ss.str());
}

vector<string> split(string const& str, string_view delim) {
    return vector<string>{std::from_range,
                          str | vw::split(delim) | vw::transform([](auto a) {
                              return string{std::from_range, a};
                          })};
}

unordered_map<string,
              std::function<void(vector<long>& registers, long, long, long)>>
    ops{
        {"addr", [](vector<long>& registers, long A, long B,
                    long C) { registers[C] = registers[A] + registers[B]; }},
        {"addi", [](vector<long>& registers, long A, long B,
                    long C) { registers[C] = registers[A] + B; }},
        {"mulr", [](vector<long>& registers, long A, long B,
                    long C) { registers[C] = registers[A] * registers[B]; }},
        {"muli", [](vector<long>& registers, long A, long B,
                    long C) { registers[C] = registers[A] * B; }},
        {"banr", [](vector<long>& registers, long A, long B,
                    long C) { registers[C] = registers[A] & registers[B]; }},
        {"bani", [](vector<long>& registers, long A, long B,
                    long C) { registers[C] = registers[A] & B; }},
        {"borr", [](vector<long>& registers, long A, long B,
                    long C) { registers[C] = registers[A] | registers[B]; }},
        {"bori", [](vector<long>& registers, long A, long B,
                    long C) { registers[C] = registers[A] | B; }},
        {"setr", [](vector<long>& registers, long A, long,
                    long C) { registers[C] = registers[A]; }},
        {"seti", [](vector<long>& registers, long A, long,
                    long C) { registers[C] = A; }},
        {"gtir",
         [](vector<long>& registers, long A, long B, long C) {
             A > registers[B] ? registers[C] = 1 : registers[C] = 0;
         }},
        {"gtri",
         [](vector<long>& registers, long A, long B, long C) {
             registers[A] > B ? registers[C] = 1 : registers[C] = 0;
         }},
        {"gtrr",
         [](vector<long>& registers, long A, long B, long C) {
             registers[A] > registers[B] ? registers[C] = 1 : registers[C] = 0;
         }},
        {"eqir",
         [](vector<long>& registers, long A, long B, long C) {
             A == registers[B] ? registers[C] = 1 : registers[C] = 0;
         }},
        {"eqri",
         [](vector<long>& registers, long A, long B, long C) {
             registers[A] == B ? registers[C] = 1 : registers[C] = 0;
         }},
        {"eqrr",
         [](vector<long>& registers, long A, long B, long C) {
             registers[A] == registers[B] ? registers[C] = 1 : registers[C] = 0;
         }},
    };

bool is_equal(vector<long> const& r1, vector<long> const& r2) {
    for (auto [i, j] : vw::zip(r1, r2)) {
        if (i != j) return false;
    }
    return true;
}

auto parte_1(vector<string>& lines) {
    int j{0};
    vector<string> strings_vector{};
    unordered_map<int, vector<string>> operation_tests;
    for (auto line : lines) {
        if (line.length()) {
            strings_vector.push_back(line);
        } else {
            if (strings_vector.empty()) continue;
            operation_tests[++j] = strings_vector;
            strings_vector.clear();
        }
    }
    vector<string> operations{std::move(strings_vector)};

    int k{0};
    for (auto tests : operation_tests | vw::values) {
        string before_string = tests[0];
        string operation_string = tests[1];
        string after_string = tests[2];

        before_string.replace(0, 9, "").pop_back();
        after_string.replace(0, 9, "").pop_back();

        vector<long> before =
            split(before_string, " ") |
            vw::transform([](string sv) { return std::stol(sv); }) |
            rg::to<vector<long>>();
        vector<long> operation =
            split(operation_string, " ") |
            vw::transform([](string sv) { return std::stol(sv); }) |
            rg::to<vector<long>>();
        vector<long> after =
            split(after_string, " ") |
            vw::transform([](string sv) { return std::stol(sv); }) |
            rg::to<vector<long>>();

        int i{0};
        for (auto [op_label, op] : ops) {
            auto befor{before};
            op(befor, operation[1], operation[2], operation[3]);
            if (is_equal(befor, after)) {
                i++;
            }
        }
        if (i >= 3) {
            k++;
        }
    }
    return k;
}

auto parte_2(vector<string>& lines) {
    int j{0};
    vector<string> strings_vector{};
    unordered_map<int, vector<string>> operation_tests;
    for (auto line : lines) {
        if (line.length()) {
            strings_vector.push_back(line);
        } else {
            if (strings_vector.empty()) continue;
            operation_tests[++j] = strings_vector;
            strings_vector.clear();
        }
    }
    vector<string> operations{std::move(strings_vector)};

    // pr(operation_tests);
    // pr(operations);

    unordered_map<int, vector<vector<string>>> possible_operations;
    for (auto tests : operation_tests | vw::values) {
        string before_string = tests[0];
        string operation_string = tests[1];
        string after_string = tests[2];

        before_string.replace(0, 9, "").pop_back();
        after_string.replace(0, 9, "").pop_back();

        vector<long> before =
            split(before_string, " ") |
            vw::transform([](string sv) { return std::stol(sv); }) |
            rg::to<vector<long>>();
        vector<long> operation =
            split(operation_string, " ") |
            vw::transform([](string sv) { return std::stol(sv); }) |
            rg::to<vector<long>>();
        vector<long> after =
            split(after_string, " ") |
            vw::transform([](string sv) { return std::stol(sv); }) |
            rg::to<vector<long>>();

        long op_code{operation[0]};
        long A{operation[1]};
        long B{operation[2]};
        long C{operation[3]};
        vector<string> op_labels{};
        for (auto [op_label, op] : ops) {
            auto befor{before};
            op(befor, A, B, C);
            if (is_equal(befor, after)) {
                op_labels.push_back(op_label);
            }
        }
        if (possible_operations.contains(op_code)) {
            possible_operations[op_code].push_back(op_labels);
        } else {
            possible_operations[op_code] = {op_labels};
        }
    }

    // pr(possible_operations);

    unordered_map<int, set<string>> opcode_oplabels_map;
    for (auto [opcode, possible_oplabels] : possible_operations) {
        set<string> oplabels{std::from_range, possible_oplabels[0]};
        for (auto try_oplabels : possible_oplabels | vw::drop(1)) {
            vector<string> to_remove{};
            for (string oplabel : oplabels) {
                if (rg::find(try_oplabels, oplabel) == try_oplabels.end()) {
                    to_remove.push_back(oplabel);
                }
            }
            for (auto r : to_remove) {
                oplabels.erase(r);
            }
        }
        opcode_oplabels_map[opcode] = oplabels;
    }

    // pr(opcode_oplabels_map);

    unordered_map<long, string> opcode_oplabel_map;
    bool done{false};
    while (not done) {
        done = true;
        string oplabel;
        long opcode;
        for (auto& [k, v] : opcode_oplabels_map) {
            if (v.size() == 1 and !opcode_oplabel_map.contains(k)) {
                done = false;
                oplabel = *v.begin();
                opcode = k;
                done = false;
                break;
            }
        }
        if (done) break;
        opcode_oplabel_map[opcode] = oplabel;
        for (auto& [k, v] : opcode_oplabels_map) {
            if (k == opcode) continue;
            if (v.contains(oplabel)) {
                v.erase(oplabel);
            };
        }
    }

    // pr(opcode_oplabel_map);

    vector<long> registers(4, 0);

    for (string operation : operations) {
        auto vec = split(operation, " ");
        long opcode = std::stol(vec[0]);
        long A = std::stol(vec[1]);
        long B = std::stol(vec[2]);
        long C = std::stol(vec[3]);

        ops[opcode_oplabel_map[opcode]](registers, A, B, C);
    }

    return registers[0];
}

int main(int, char** argv) {
    std::ifstream input;
    input.open(argv[1]);

    string line;
    vector<string> lines;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    input.close();

    std::print("\n");
    println("Resultado da parte 1: {}", parte_1(lines));
    println("Resultado da parte 2: {}", parte_2(lines));

    return 0;
}
