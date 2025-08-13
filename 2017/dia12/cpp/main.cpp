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
    unordered_map<int, unordered_set<int>> adj_list;
    regex pat{R"((\d+) <-> (.*))"};
    for (auto line : lines) {
        smatch m;
        regex_match(line, m, pat);
        int val{stoi(m[1].str())};
        vector<int> vi{from_range,
                       split(m[2].str(), ", ") |
                           vw::transform([](string sv) { return stoi(sv); })};
        for (int v : vi) {
            adj_list[val].insert(v);
        }
    }
    int srrc{0};
    // vector<int> group{srrc};
    long group_num{1};
    deque<int> q{srrc};
    unordered_set<int> visited{srrc};
    while (not q.empty()) {
        int s{q.front()};
        q.pop_front();
        for (int v : adj_list[s]) {
            if (not visited.contains(v)) {
                q.push_back(v);
                visited.insert(v);
                // group.push_back(v);
                group_num++;
            }
        }
    }
    // println("Resultado da parte 1: {}", group.size());
    println("Resultado da parte 1: {}", group_num);
}

void parte_2(vector<string>& lines) {
    unordered_map<int, unordered_set<int>> adj_list;
    regex pat{R"((\d+) <-> (.*))"};
    for (auto line : lines) {
        smatch m;
        regex_match(line, m, pat);
        int val{stoi(m[1].str())};
        vector<int> vi{from_range,
                       split(m[2].str(), ", ") |
                           vw::transform([](string sv) { return stoi(sv); })};
        for (int v : vi) {
            adj_list[val].insert(v);
        }
    }
    unordered_map<int, bool> has_group;
    for (int k : adj_list | vw::keys) {
        has_group[k] = false;
    }

    int srrc{0};
    auto all_true = [&srrc](unordered_map<int, bool>& hg) {
        for (auto [k, v] : hg) {
            if (!v) {
                srrc = k;
                return false;
            }
        }
        return true;
    };

    // vector<vector<int>> groups;
    long groups_num{0};
    while (not all_true(has_group)) {
        // vector<int> group{srrc};
        deque<int> q{srrc};
        unordered_set<int> visited{srrc};
        has_group[srrc] = true;
        while (not q.empty()) {
            int s{q.front()};
            q.pop_front();
            for (int v : adj_list[s]) {
                if (not visited.contains(v)) {
                    q.push_back(v);
                    visited.insert(v);
                    has_group[v] = true;
                    // group.push_back(v);
                }
            }
        }
        // groups.push_back(group);
        groups_num++;
    }
    // println("Resultado da parte 2: {}", groups.size());
    println("Resultado da parte 2: {}", groups_num);
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
