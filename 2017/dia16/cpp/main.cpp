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
    vector<string> instructions{split(lines[0], ",")};

    regex pat_s{R"(s(\d+))"};
    regex pat_x{R"(x(\d+)/(\d+))"};
    regex pat_p{R"(p(\w+)/(\w+))"};
    string s{"abcdefghijklmnop"};
    // string s{"abcde"};
    for (string ins : instructions) {
        smatch m;
        if (regex_match(ins, m, pat_s)) {
            rg::rotate(s, s.end() - stoi(m[1].str()));
        } else if (regex_match(ins, m, pat_x)) {
            std::swap(s[stoi(m[1].str())], s[stoi(m[2].str())]);
        } else if (regex_match(ins, m, pat_p)) {
            std::swap(s[s.find(m[1])], s[s.find(m[2])]);
        }
    }

    println("Resultado da parte 1: {}", s);
}

void parte_2(vector<string>& lines) {
    vector<string> instructions{split(lines[0], ",")};

    regex pat_s{R"(s(\d+))", std::regex::optimize};
    regex pat_x{R"(x(\d+)/(\d+))", std::regex::optimize};
    regex pat_p{R"(p(\w+)/(\w+))", std::regex::optimize};
    string s{"abcdefghijklmnop"};
    // string s{"abcde"};
    for (string ins : instructions) {
        smatch m;
        if (regex_match(ins, m, pat_s)) {
            rg::rotate(s, s.end() - stoi(m[1].str()));
        } else if (regex_match(ins, m, pat_x)) {
            std::swap(s[stoi(m[1].str())], s[stoi(m[2].str())]);
        } else if (regex_match(ins, m, pat_p)) {
            std::swap(s[s.find(m[1])], s[s.find(m[2])]);
        }
    }

    long cycle{0};
    string end{s};
    for (;;) {
        for (string ins : instructions) {
            smatch m;
            if (regex_match(ins, m, pat_s)) {
                rg::rotate(s, s.end() - stoi(m[1].str()));
            } else if (regex_match(ins, m, pat_x)) {
                std::swap(s[stoi(m[1].str())], s[stoi(m[2].str())]);
            } else if (regex_match(ins, m, pat_p)) {
                std::swap(s[s.find(m[1])], s[s.find(m[2])]);
            }
        }
        cycle++;
        if (s == end) break;
    }
    long n{1000000000};
    for (long i{1}; i < n % cycle; i++) {
        for (string ins : instructions) {
            smatch m;
            if (regex_match(ins, m, pat_s)) {
                rg::rotate(s, s.end() - stoi(m[1].str()));
            } else if (regex_match(ins, m, pat_x)) {
                std::swap(s[stoi(m[1].str())], s[stoi(m[2].str())]);
            } else if (regex_match(ins, m, pat_p)) {
                std::swap(s[s.find(m[1])], s[s.find(m[2])]);
            }
        }
    }

    println("Resultado da parte 2: {}", s);
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
