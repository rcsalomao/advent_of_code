// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
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
    regex pat_swap_pos{R"(swap position (\d+) with position (\d+))"};
    regex pat_swap_let{R"(swap letter (\w+) with letter (\w+))"};
    regex pat_move_pos{R"(move position (\d+) to position (\d+))"};
    regex pat_revr_pos{R"(reverse positions (\d+) through (\d+))"};
    regex pat_rotate_dir{R"(rotate (right|left) (\d+) steps?)"};
    regex pat_rotate_based{R"(rotate based on position of letter (\w+))"};

    std::deque<string> password{"a", "b", "c", "d", "e", "f", "g", "h"};
    // std::deque<string> password{"a", "b", "c", "d", "e"};

    for (auto line : lines) {
        smatch m;
        if (regex_match(line, m, pat_swap_pos)) {
            int pos1{stoi(m[1].str())};
            int pos2{stoi(m[2].str())};
            std::swap(password[pos1], password[pos2]);
        } else if (regex_match(line, m, pat_swap_let)) {
            string let1{m[1].str()};
            string let2{m[2].str()};
            std::iter_swap(rg::find(password, let1), rg::find(password, let2));
        } else if (regex_match(line, m, pat_move_pos)) {
            int pos1{stoi(m[1].str())};
            int pos2{stoi(m[2].str())};
            auto v{password[pos1]};
            password.erase(password.begin() + pos1);
            password.insert(password.begin() + pos2, v);
        } else if (regex_match(line, m, pat_revr_pos)) {
            int pos1{stoi(m[1].str())};
            int pos2{stoi(m[2].str())};
            std::reverse(password.begin() + pos1, password.begin() + pos2 + 1);
        } else if (regex_match(line, m, pat_rotate_dir)) {
            string dir{m[1].str()};
            int n_steps{stoi(m[2].str())};
            if (dir == "left") {
                for (int _ : vw::iota(0, n_steps)) {
                    auto f{password.front()};
                    password.pop_front();
                    password.push_back(f);
                }
            } else {
                for (int _ : vw::iota(0, n_steps)) {
                    auto f{password.back()};
                    password.pop_back();
                    password.push_front(f);
                }
            }
        } else if (regex_match(line, m, pat_rotate_based)) {
            string let{m[1].str()};
            int idx = std::distance(password.begin(), rg::find(password, let));
            for (int _ : vw::iota(0, 1 + idx + (idx >= 4 ? 1 : 0))) {
                auto b{password.back()};
                password.pop_back();
                password.push_front(b);
            }
        }
    }

    println("Resultado da parte 1: {}",
            string{from_range, password | vw::join});
}

void parte_2(std::vector<std::string>& lines) {
    regex pat_swap_pos{R"(swap position (\d+) with position (\d+))"};
    regex pat_swap_let{R"(swap letter (\w+) with letter (\w+))"};
    regex pat_move_pos{R"(move position (\d+) to position (\d+))"};
    regex pat_revr_pos{R"(reverse positions (\d+) through (\d+))"};
    regex pat_rotate_dir{R"(rotate (right|left) (\d+) steps?)"};
    regex pat_rotate_based{R"(rotate based on position of letter (\w+))"};

    std::deque<string> password{"f", "b", "g", "d", "c", "e", "a", "h"};
    // std::deque<string> password{"d", "e", "c", "a", "b"};

    for (auto line : lines | vw::reverse) {
        smatch m;
        if (regex_match(line, m, pat_swap_pos)) {
            int pos1{stoi(m[1].str())};
            int pos2{stoi(m[2].str())};
            std::swap(password[pos1], password[pos2]);
        } else if (regex_match(line, m, pat_swap_let)) {
            string let1{m[1].str()};
            string let2{m[2].str()};
            std::iter_swap(rg::find(password, let1), rg::find(password, let2));
        } else if (regex_match(line, m, pat_move_pos)) {
            int pos1{stoi(m[1].str())};
            int pos2{stoi(m[2].str())};
            auto v{password[pos2]};
            password.erase(password.begin() + pos2);
            password.insert(password.begin() + pos1, v);
        } else if (regex_match(line, m, pat_revr_pos)) {
            int pos1{stoi(m[1].str())};
            int pos2{stoi(m[2].str())};
            std::reverse(password.begin() + pos1, password.begin() + pos2 + 1);
        } else if (regex_match(line, m, pat_rotate_dir)) {
            string dir{m[1].str()};
            int n_steps{stoi(m[2].str())};
            if (dir == "right") {
                for (int _ : vw::iota(0, n_steps)) {
                    auto f{password.front()};
                    password.pop_front();
                    password.push_back(f);
                }
            } else {
                for (int _ : vw::iota(0, n_steps)) {
                    auto b{password.back()};
                    password.pop_back();
                    password.push_front(b);
                }
            }
        } else if (regex_match(line, m, pat_rotate_based)) {
            string let{m[1].str()};
            int idx = std::distance(password.begin(), rg::find(password, let));
            for (int _ : vw::iota(
                     0, idx / 2 + ((idx % 2 == 1) or (idx == 0) ? 1 : 5))) {
                auto f{password.front()};
                password.pop_front();
                password.push_back(f);
            }
        }
    }

    println("Resultado da parte 2: {}",
            string{from_range, password | vw::join});
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
