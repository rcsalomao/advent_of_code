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
#include <openssl/md5.h>

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

struct Node {
    int val;
    shared_ptr<Node> next;
};

void parte_1(vector<string>& lines) {
    int steps{stoi(lines[0])};

    shared_ptr<Node> c = std::make_shared<Node>(0);
    c->next = c;

    size_t n{2017};
    for (size_t i = 1; i < n + 1; i++) {
        for (int j = 0; j < steps; j++) {
            c = c->next;
        }
        auto tmp = c->next;
        c->next = std::make_shared<Node>(i);
        c = c->next;
        c->next = tmp;
    }

    println("Resultado da parte 1: {}", c->next->val);
}

// void parte_2(vector<string>& lines) {
//     int steps{stoi(lines[0])};
//
//     shared_ptr<Node> c = std::make_shared<Node>(0);
//     auto zero = c;
//     c->next = c;
//
//     size_t n{50000000};
//     for (size_t i = 1; i < n + 1; i++) {
//         for (int j = 0; j < steps; j++) {
//             c = c->next;
//         }
//         auto tmp = c->next;
//         c->next = std::make_shared<Node>(i);
//         c = c->next;
//         c->next = tmp;
//     }
//
//     println("Resultado da parte 2: {}", zero->next->val);
// }
void parte_2(vector<string>& lines) {
    int steps{stoi(lines[0])};

    int r;
    int c{0};
    size_t n{50000000};
    for (size_t i = 1; i < n + 1; i++) {
        c = (c + steps) % i;
        c++;
        if (c == 1) r = i;
    }

    println("Resultado da parte 2: {}", r);
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
