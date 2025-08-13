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

struct Position {
    int x;
    int y;
    long step{0};
};

string to_string(const Position& p) { return std::format("{};{}", p.x, p.y); }

const static unordered_map<string, Position> dir_map{
    {"n", {0, 2}},   {"s", {0, -2}},  {"ne", {1, 1}},
    {"nw", {-1, 1}}, {"se", {1, -1}}, {"sw", {-1, -1}},
};

void parte_1(vector<string>& lines) {
    for (auto line : lines) {
        Position end{0, 0};
        vector<string> directions = split(line, ",");
        for (string dir : directions) {
            end.x += dir_map.at(dir).x;
            end.y += dir_map.at(dir).y;
        }

        Position start{0, 0};
        auto cmp = [&end](const Position& l, const Position& r) {
            double vl{
                std::sqrt(std::pow(end.x - l.x, 2) + std::pow(end.y - l.y, 2))};
            double vr{
                std::sqrt(std::pow(end.x - r.x, 2) + std::pow(end.y - r.y, 2))};
            return vl > vr;
        };
        std::priority_queue<Position, vector<Position>, decltype(cmp)> q(cmp);
        q.push(start);
        unordered_set<string> visited{to_string(start)};
        bool done{false};
        long n_steps{0};
        while (not done) {
            auto p0{q.top()};
            q.pop();
            if (p0.x == end.x and p0.y == end.y) {
                done = true;
                n_steps = p0.step;
                continue;
            }
            for (auto [k, v] : dir_map) {
                auto [x, y, _] = v;
                Position p1{p0.x + x, p0.y + y, p0.step + 1};
                string key{to_string(p1)};
                if (not visited.contains(key)) {
                    q.push(p1);
                    visited.insert(key);
                }
            }
        }
        println("Resultado da parte 1: {}", n_steps);
    }
}

void parte_2(vector<string>& lines) {
    long max_n_steps{0};
    for (auto line : lines) {
        Position end{0, 0};
        vector<string> directions = split(line, ",");
        for (string dir : directions) {
            end.x += dir_map.at(dir).x;
            end.y += dir_map.at(dir).y;

            Position start{0, 0};
            auto cmp = [&end](const Position& l, const Position& r) {
                double vl{std::sqrt(std::pow(end.x - l.x, 2) +
                                    std::pow(end.y - l.y, 2))};
                double vr{std::sqrt(std::pow(end.x - r.x, 2) +
                                    std::pow(end.y - r.y, 2))};
                return vl > vr;
            };
            std::priority_queue<Position, vector<Position>, decltype(cmp)> q(
                cmp);
            q.push(start);
            unordered_set<string> visited{to_string(start)};
            bool done{false};
            long n_steps{0};
            while (not done) {
                auto p0{q.top()};
                q.pop();
                if (p0.x == end.x and p0.y == end.y) {
                    done = true;
                    n_steps = p0.step;
                    continue;
                }
                for (auto [k, v] : dir_map) {
                    auto [x, y, _] = v;
                    Position p1{p0.x + x, p0.y + y, p0.step + 1};
                    string key{to_string(p1)};
                    if (not visited.contains(key)) {
                        q.push(p1);
                        visited.insert(key);
                    }
                }
            }
            max_n_steps = std::max(max_n_steps, n_steps);
        }
        println("Resultado da parte 2: {}", max_n_steps);
    }
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
