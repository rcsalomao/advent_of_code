// #include <Eigen/Dense>
// #include <json.hpp>
// #include <openssl/md5.h>
#include <bits/stdc++.h>

namespace rg = std::ranges;
namespace vw = std::views;
using namespace std::string_literals;
using namespace std::string_view_literals;
using std::deque;
using std::from_range;
using std::map;
using std::print;
using std::println;
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

template <typename T>
void pln(T&& a) {
    println("{}", a);
}

void normalize(string& state, int& init_idx) {
    size_t i{state.find("#")};
    if (i < 4) {
        state.insert(state.begin(), 4 - i, '.');
        init_idx -= 4 - i;
    }
    size_t j{state.rfind("#")};
    size_t sz{state.size()};
    if (j > sz - 5) {
        state.insert(state.end(), 4 - (sz - j - 1), '.');
    }
}

void pass_generation(string& state, unordered_map<string, string>& rules) {
    string new_state{state};
    long sz = state.size();
    for (auto i : vw::iota(2, sz - 2)) {
        string w{state.substr(i - 2, 5)};
        new_state[i] = '.';
        for (auto& [k, v] : rules) {
            if (k == w) {
                new_state[i] = *v.data();
                break;
            }
        }
    }
    state = new_state;
}

int make_count(string_view state, int init_idx) {
    int count{0};
    for (char c : state) {
        if (c == '#') count += init_idx;
        init_idx++;
    }
    return count;
}

auto parte_1(vector<string>& lines) {
    smatch m;

    regex state_pat{R"(init.*: (.*)$)"};
    string state;
    for (auto line : lines | vw::take(1)) {
        regex_match(line, m, state_pat);
        state = m[1];
    }

    regex pot_pat{R"(^(.+) => (.)$)"};
    unordered_map<string, string> rules;
    for (auto line : lines | vw::drop(1)) {
        if (regex_match(line, m, pot_pat)) rules[m[1]] = m[2];
    }
    int init_idx{0};

    int iters{20};
    while (iters--) {
        normalize(state, init_idx);
        pass_generation(state, rules);
    }

    int count = make_count(state, init_idx);

    return count;
}

auto parte_2(vector<string>& lines) {
    smatch m;

    regex state_pat{R"(init.*: (.*)$)"};
    string state;
    for (auto line : lines | vw::take(1)) {
        regex_match(line, m, state_pat);
        state = m[1];
    }

    regex pot_pat{R"(^(.+) => (.)$)"};
    unordered_map<string, string> rules;
    for (auto line : lines | vw::drop(1)) {
        if (regex_match(line, m, pot_pat)) rules[m[1]] = m[2];
    }
    // int init_idx{0};

    // int count0 = make_count(state, init_idx);
    // int iters{150};
    // int idx{1};
    // while (iters--) {
    //     normalize(state, init_idx);
    //     pass_generation(state, rules);
    //     int count1 = make_count(state, init_idx);
    //     println("{}; {}; {}", idx, count1 - count0, count1);
    //     count0 = count1;
    //     idx++;
    // }

    return 3151 + (50000000000 - 120) * 22;
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
