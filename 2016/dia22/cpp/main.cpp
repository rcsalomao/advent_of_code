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

struct Node {
    int x;
    int y;
    int size;
    int used;
    int avail;
};

string to_string(const Node& node) {
    string s;
    s += "{(" + to_string(node.x) + "," + to_string(node.y) + ")-" +
         to_string(node.size) + "-" + to_string(node.used) + "-" +
         to_string(node.avail) + "}";
    return s;
}

template <>
struct std::formatter<Node> : std::formatter<std::string> {
    auto format(const Node& node, std::format_context& ctx) const {
        return std::formatter<std::string>::format(to_string(node), ctx);
    }
};

void parte_1(std::vector<std::string>& lines) {
    regex pat{R"(.*node-x(\d+)-y(\d+)\s+(\d+)T\s+(\d+)T\s+(\d+)T.*)",
              regex::optimize};
    vector<Node> nodes;
    for (auto line : lines) {
        smatch m;
        if (regex_match(line, m, pat)) {
            int x = stoi(m[1].str());
            int y = stoi(m[2].str());
            int size = stoi(m[3].str());
            int used = stoi(m[4].str());
            int avail = stoi(m[5].str());
            nodes.push_back({x, y, size, used, avail});
        }
    }

    uint t{0};
    for (auto [i, node_a] : vw::enumerate(nodes)) {
        for (auto [j, node_b] : vw::enumerate(nodes)) {
            if (i == j) continue;
            if (node_a.used > 0 and node_b.avail >= node_a.used) t++;
        }
    }

    println("Resultado da parte 1: {}", t);
}

void parte_2(std::vector<std::string>& lines) {
    regex pat{R"(.*node-x(\d+)-y(\d+)\s+(\d+)T\s+(\d+)T\s+(\d+)T.*)",
              regex::optimize};
    vector<Node> nodes;
    for (auto line : lines) {
        smatch m;
        if (regex_match(line, m, pat)) {
            int x = stoi(m[1].str());
            int y = stoi(m[2].str());
            int size = stoi(m[3].str());
            int used = stoi(m[4].str());
            int avail = stoi(m[5].str());
            nodes.push_back({x, y, size, used, avail});
        }
    }

    size_t w{30};
    size_t h{32};
    vector<string> grid;
    for (size_t i = 0; i < h; i++) {
        grid.push_back(string{from_range, vw::repeat('.', w)});
    }

    for (auto& node : nodes) {
        if (node.size > 400) {
            grid[node.y][node.x] = '#';
        }
        if (node.used == 0) {
            grid[node.y][node.x] = '_';
        }
    }
    grid[0][0] = '*';
    grid[0][w - 1] = 'G';

    println("Resultado da parte 2: {}", "");
    for (auto& r : grid) {
        println("{}", r);
    }
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
