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

regex num_pat{R"(-?[0-9]+)", std::regex::optimize};
bool is_integer(const std::string& s) { return std::regex_match(s, num_pat); }

long long get_val(const string& s, unordered_map<string, long long>& reg) {
    if (is_integer(s)) return stoll(s);
    return reg[s];
}

void parte_1(vector<string>& lines) {
    vector<string> instructions = lines;
    unordered_map<string, long long> reg;
    for (char c : "abcdefgh") {
        reg[to_string(c)] = 0;
    }

    long count{0};
    int idx{0};
    bool done{false};
    while (not done) {
        std::istringstream ss{instructions[idx]};
        string op, X, Y;
        ss >> op >> X >> Y;
        if (op == "set") {
            reg[X] = get_val(Y, reg);
            idx++;
        } else if (op == "sub") {
            reg[X] -= get_val(Y, reg);
            idx++;
        } else if (op == "mul") {
            reg[X] *= get_val(Y, reg);
            idx++;
            count++;
        } else if (op == "jnz") {
            if (get_val(X, reg) != 0)
                idx += get_val(Y, reg);
            else
                idx++;
        }
        if (idx < 0 || idx >= (int)instructions.size()) {
            // println("Instruction index out of bounds!");
            done = true;
        } else {
            done = false;
        }
    }
    println("Resultado da parte 1: {}", count);
}

void parte_2(vector<string>& lines) {
    int64_t b{99 * 100 + 100000};
    const int64_t c{b + 17000};
    int64_t d{0};
    int64_t e{0};
    int64_t f{0};
    int64_t h{0};
    bool done{false};
    while (not done) {
        f = 1;
        d = 2;
        do {
            // e = 2;
            // do {
            //     if (d * e == b) f = 0;
            //     --e;
            // } while (e != b);
            if (b % d == 0) f = 0;

            d++;
        } while (d != b);

        if (f == 0) h++;
        if (b == c) {
            done = true;
        }
        b += 17;
    }
    println("Resultado da parte 2: {}", h);
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
