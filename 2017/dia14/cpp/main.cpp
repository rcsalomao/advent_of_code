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

string knot_hash(string s) {
    vector<int> lengths =
        s | vw::transform([](char c) { return static_cast<int>(c); }) |
        rg::to<vector<int>>();
    lengths.insert(lengths.end(), {17, 31, 73, 47, 23});

    uint cursor{0};
    uint skip{0};
    vector<int> seq{from_range, vw::iota(0, 256)};
    size_t seq_size{seq.size()};
    uint iters{64};
    while (iters--) {
        for (int l : lengths) {
            if ((size_t)l > seq_size) continue;
            rg::rotate(seq, std::begin(seq) + cursor);
            rg::reverse(seq.begin(), seq.begin() + l);
            rg::rotate(seq, std::end(seq) - cursor);
            cursor = (cursor + l + skip) % seq_size;
            skip++;
        }
    }

    vector<int> dh;
    for (uint i{0}; i < 16; i++) {
        int v{0};
        for (int j :
             rg::subrange(seq.begin() + i * 16, seq.begin() + i * 16 + 16)) {
            v ^= j;
        }
        dh.push_back(v);
    }
    string hash;
    for (int i : dh) {
        hash += std::format("{:02x}", i);
    }

    return hash;
}

string expand_to_bits(string_view hash) {
    auto calc_bits = [](char c) {
        switch (c) {
            case '0':
                return "0000";
            case '1':
                return "0001";
            case '2':
                return "0010";
            case '3':
                return "0011";
            case '4':
                return "0100";
            case '5':
                return "0101";
            case '6':
                return "0110";
            case '7':
                return "0111";
            case '8':
                return "1000";
            case '9':
                return "1001";
            case 'a':
                return "1010";
            case 'b':
                return "1011";
            case 'c':
                return "1100";
            case 'd':
                return "1101";
            case 'e':
                return "1110";
            // case 'f':
            //     return "1111";
            default:
                return "1111";
        }
    };
    std::ostringstream oss;
    for (char c : hash) {
        oss << calc_bits(c);
    }
    return oss.str();
}

void parte_1(vector<string>& lines) {
    string root{lines[0]};
    int used_squares{0};
    for (int i : vw::iota(0, 128)) {
        string hash = knot_hash(root + "-" + to_string(i));
        string bits_expanded = expand_to_bits(hash);
        used_squares += rg::count(bits_expanded, '1');
    }

    println("Resultado da parte 1: {}", used_squares);
}

string to_string(tuple<int, int> coord) {
    auto [x, y] = coord;
    return std::format("{};{}", x, y);
}

tuple<int, int> to_tuple(string s) {
    vector<string> vs = split(s, ";");
    return {stoi(vs[0]), stoi(vs[1])};
}

void parte_2(vector<string>& lines) {
    string root{lines[0]};
    vector<string> grid;
    unordered_map<string, bool> has_group;
    for (int i : vw::iota(0, 128)) {
        string hash = knot_hash(root + "-" + to_string(i));
        grid.emplace_back(expand_to_bits(hash));
        for (auto [j, c] : vw::enumerate(grid.back())) {
            if (c == '1') has_group[to_string({i, j})] = false;
        }
    }

    string srrc;
    auto all_true = [&srrc](unordered_map<string, bool>& hg) {
        for (auto [k, v] : hg) {
            if (!v) {
                srrc = k;
                return false;
            }
        }
        return true;
    };

    // vector<vector<int>> groups;
    vector<tuple<int, int>> dirs{{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    long groups_num{0};
    while (not all_true(has_group)) {
        // vector<int> group{srrc};
        deque<string> q{srrc};
        unordered_set<string> visited{srrc};
        has_group[srrc] = true;
        while (not q.empty()) {
            string s{q.front()};
            q.pop_front();
            for (auto [a, b] : dirs) {
                auto [x, y] = to_tuple(s);
                int new_x{x + a};
                int new_y{y + b};
                if (new_x < 0 or new_y < 0 or new_x > 127 or new_y > 127)
                    continue;
                if (grid[new_x][new_y] == '0') continue;
                string key = to_string({new_x, new_y});
                if (not visited.contains(key)) {
                    q.push_back(key);
                    visited.insert(key);
                    has_group[key] = true;
                    // group.push_back(v);
                }
            }
        }
        // groups.push_back(group);
        groups_num++;
    }

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
