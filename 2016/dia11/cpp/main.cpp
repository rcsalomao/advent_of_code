// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
// #include <nlohmann/json.hpp>
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

struct Element {
    int microchip_floor;
    int generator_floor;

    bool operator<(const Element& other) const {
        if (microchip_floor == other.microchip_floor) {
            return generator_floor < other.generator_floor;
        } else {
            return microchip_floor < other.microchip_floor;
        }
    };
};

struct State {
    int elevator;
    vector<Element> elements;
    uint n_steps{0};
};

string to_string(const State& s) {
    auto elems = s.elements;
    sort(elems.begin(), elems.end());
    string key = to_string(s.elevator) + "|";
    for (auto& e : elems) {
        key += to_string(e.microchip_floor);
        key += to_string(e.generator_floor);
    }
    return key;
}

bool safe(const State& s) {
    for (auto& p : s.elements) {
        if (p.microchip_floor != p.generator_floor) {
            for (auto& q : s.elements) {
                if (q.generator_floor == p.microchip_floor) return false;
            }
        }
    }
    return true;
}

int solve(State init, int total_floors) {
    std::queue<State> q;
    std::unordered_set<string> seen;
    q.push(init);
    seen.insert(to_string(init));
    auto dirs = vector{1, -1};
    while (!q.empty()) {
        State cur = q.front();
        q.pop();
        bool done = true;
        for (auto& elem : cur.elements) {
            if (elem.microchip_floor != total_floors - 1 ||
                elem.generator_floor != total_floors - 1) {
                done = false;
                break;
            }
        }
        if (done) return cur.n_steps;

        enum class ToMove { microchip, generator };

        struct Movement {
            int element;
            ToMove to_move;
        };

        vector<Movement> single_moves;
        for (auto [i, elem] : std::views::enumerate(cur.elements)) {
            if (elem.microchip_floor == cur.elevator)
                single_moves.emplace_back(i, ToMove::microchip);
            if (elem.generator_floor == cur.elevator)
                single_moves.emplace_back(i, ToMove::generator);
        }

        vector<vector<Movement>> all_moves;
        int M = single_moves.size();
        // for (int i = 0; i < M; i++) {
        //     all_moves.push_back({single_moves[i]});
        //     for (int j = 0; j < M; j++) {
        //         if (i != j)
        //             all_moves.push_back({single_moves[i], single_moves[j]});
        //     }
        // }
        for (int i = 0; i < M; i++) {
            all_moves.push_back({single_moves[i]});
            for (int j = i + 1; j < M; j++) {
                all_moves.push_back({single_moves[i], single_moves[j]});
            }
        }

        for (int dir : dirs) {
            int nxt_floor = cur.elevator + dir;
            if (nxt_floor < 0 || nxt_floor > total_floors - 1) continue;
            for (auto& mv : all_moves) {
                State nxt = cur;
                nxt.elevator = nxt_floor;
                for (auto& el_to_mv : mv) {
                    if (el_to_mv.to_move == ToMove::microchip)
                        nxt.elements[el_to_mv.element].microchip_floor =
                            nxt_floor;
                    else
                        nxt.elements[el_to_mv.element].generator_floor =
                            nxt_floor;
                }
                if (!safe(nxt)) continue;
                if (seen.insert(to_string(nxt)).second) {
                    nxt.n_steps++;
                    q.push(nxt);
                }
            }
        }
    }
    return -1;  // Unreachable return, if everything runs correctly.
}

void parte_1(std::vector<std::string>& lines) {
    int total_floors = lines.size();
    unordered_map<string, unordered_map<string, int>> elements_map;
    for (auto [floor, line] : vw::enumerate(lines)) {
        regex mch_pat{R"(\w+-compatible microchip)"};
        regex gen_pat{R"(\w+ generator)"};
        vector<string> microchips = find_all(line, mch_pat);
        vector<string> generators = find_all(line, gen_pat);
        vector<string> mch =
            vector{std::from_range,
                   microchips | vw::transform([](string s) {
                       return s.replace(s.find("-compatible microchip"),
                                        sizeof("-compatible microchip"), "");
                   })};
        for (auto m : mch) elements_map[m]["microchip"] = floor;
        vector<string> gen = vector{
            std::from_range, generators | vw::transform([](string s) {
                                 return s.replace(s.find(" generator"),
                                                  sizeof(" generator"), "");
                             })};
        for (auto g : gen) elements_map[g]["generator"] = floor;
    }
    State init{0, vector{from_range, elements_map | vw::transform([](auto& a) {
                                         return Element{a.second["microchip"],
                                                        a.second["generator"]};
                                     })}};
    int n_steps{solve(init, total_floors)};
    println("Resultado da parte 1: {}", n_steps);
};

void parte_2(std::vector<std::string>& lines) {
    int total_floors = lines.size();
    unordered_map<string, unordered_map<string, int>> elements_map;
    for (auto [floor, line] : vw::enumerate(lines)) {
        regex mch_pat{R"(\w+-compatible microchip)"};
        regex gen_pat{R"(\w+ generator)"};
        vector<string> microchips = find_all(line, mch_pat);
        vector<string> generators = find_all(line, gen_pat);
        vector<string> mch =
            vector{std::from_range,
                   microchips | vw::transform([](string s) {
                       return s.replace(s.find("-compatible microchip"),
                                        sizeof("-compatible microchip"), "");
                   })};
        for (auto m : mch) elements_map[m]["microchip"] = floor;
        vector<string> gen = vector{
            std::from_range, generators | vw::transform([](string s) {
                                 return s.replace(s.find(" generator"),
                                                  sizeof(" generator"), "");
                             })};
        for (auto g : gen) elements_map[g]["generator"] = floor;
    }
    State init{0, vector{from_range, elements_map | vw::transform([](auto& a) {
                                         return Element{a.second["microchip"],
                                                        a.second["generator"]};
                                     })}};
    init.elements.emplace_back(0, 0);  // Elerium: {chip_floor, gen_floor}
    init.elements.emplace_back(0, 0);  // Dilithium: {chip_floor, gen_floor}
    int n_steps{solve(init, total_floors)};
    println("Resultado da parte 2: {}", n_steps);
};

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
