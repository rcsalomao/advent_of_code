// #include <Eigen/Dense>
#include <algorithm>
#include <boost/functional/hash.hpp>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
// #include <nlohmann/json.hpp>
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
#include <vector>

namespace rg = std::ranges;
namespace vw = std::views;
using namespace std::string_view_literals;
using std::map;
using std::print;
using std::println;
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

void parte_1(std::vector<std::string>& lines) {
    unordered_map<string, int> boss;
    for (string line : lines) {
        std::smatch m;
        std::regex_match(line, m, std::regex{R"((\w+\s?\w+?): (\d+))"});
        boss[m[1]] = std::stoi(m[2]);
    }
    // println("{}", boss);

    map<string, map<string, int>> weapons{
        {"Dagger", {{"Cost", 8}, {"Damage", 4}, {"Armor", 0}}},
        {"Shortsword", {{"Cost", 10}, {"Damage", 5}, {"Armor", 0}}},
        {"Warhammer", {{"Cost", 25}, {"Damage", 6}, {"Armor", 0}}},
        {"Longsword", {{"Cost", 40}, {"Damage", 7}, {"Armor", 0}}},
        {"Greataxe", {{"Cost", 74}, {"Damage", 8}, {"Armor", 0}}},
    };
    map<string, map<string, int>> armors{
        {"No armor", {{"Cost", 0}, {"Damage", 0}, {"Armor", 0}}},
        {"Leather", {{"Cost", 13}, {"Damage", 0}, {"Armor", 1}}},
        {"Chainmail", {{"Cost", 31}, {"Damage", 0}, {"Armor", 2}}},
        {"Splintmail", {{"Cost", 53}, {"Damage", 0}, {"Armor", 3}}},
        {"Bandedmail", {{"Cost", 75}, {"Damage", 0}, {"Armor", 4}}},
        {"Platemail", {{"Cost", 102}, {"Damage", 0}, {"Armor", 5}}},
    };
    map<string, map<string, int>> dmg_rings{
        {"No ring", {{"Cost", 0}, {"Damage", 0}, {"Armor", 0}}},
        {"Damage +1", {{"Cost", 25}, {"Damage", 1}, {"Armor", 0}}},
        {"Damage +2", {{"Cost", 50}, {"Damage", 2}, {"Armor", 0}}},
        {"Damage +3", {{"Cost", 100}, {"Damage", 3}, {"Armor", 0}}},
    };
    map<string, map<string, int>> arm_rings{
        {"No ring", {{"Cost", 0}, {"Damage", 0}, {"Armor", 0}}},
        {"Defense +1", {{"Cost", 20}, {"Damage", 0}, {"Armor", 1}}},
        {"Defense +2", {{"Cost", 40}, {"Damage", 0}, {"Armor", 2}}},
        {"Defense +3", {{"Cost", 80}, {"Damage", 0}, {"Armor", 3}}},
    };

    int total_cost{0};
    int total_damage{0};
    int total_armor{0};
    int min_cost = std::numeric_limits<int>::max();
    for (auto& [_, w] : weapons) {
        for (auto& [_, a] : armors) {
            for (auto& [_, dr] : dmg_rings) {
                for (auto& [_, ar] : arm_rings) {
                    total_cost =
                        w["Cost"] + a["Cost"] + dr["Cost"] + ar["Cost"];
                    total_damage =
                        w["Damage"] + a["Damage"] + dr["Damage"] + ar["Damage"];
                    total_armor =
                        w["Armor"] + a["Armor"] + dr["Armor"] + ar["Armor"];

                    int player_turns_to_kill =
                        boss["Hit Points"] /
                        (std::max(total_damage - boss["Armor"], 1));
                    int boss_turns_to_kill =
                        100 / (std::max(boss["Damage"] - total_armor, 1));

                    if (player_turns_to_kill <= boss_turns_to_kill) {
                        // min_cost = std::min(total_cost, min_cost);
                        min_cost = total_cost;  // ???
                    }
                }
            }
        }
    }

    println("Resultado da parte 1: {}", min_cost);
};

void parte_2(std::vector<std::string>& lines) {
    unordered_map<string, int> boss;
    for (string line : lines) {
        std::smatch m;
        std::regex_match(line, m, std::regex{R"((\w+\s?\w+?): (\d+))"});
        boss[m[1]] = std::stoi(m[2]);
    }
    // println("{}", boss);

    map<string, map<string, int>> weapons{
        {"Dagger", {{"Cost", 8}, {"Damage", 4}, {"Armor", 0}}},
        {"Shortsword", {{"Cost", 10}, {"Damage", 5}, {"Armor", 0}}},
        {"Warhammer", {{"Cost", 25}, {"Damage", 6}, {"Armor", 0}}},
        {"Longsword", {{"Cost", 40}, {"Damage", 7}, {"Armor", 0}}},
        {"Greataxe", {{"Cost", 74}, {"Damage", 8}, {"Armor", 0}}},
    };
    map<string, map<string, int>> armors{
        {"No armor", {{"Cost", 0}, {"Damage", 0}, {"Armor", 0}}},
        {"Leather", {{"Cost", 13}, {"Damage", 0}, {"Armor", 1}}},
        {"Chainmail", {{"Cost", 31}, {"Damage", 0}, {"Armor", 2}}},
        {"Splintmail", {{"Cost", 53}, {"Damage", 0}, {"Armor", 3}}},
        {"Bandedmail", {{"Cost", 75}, {"Damage", 0}, {"Armor", 4}}},
        {"Platemail", {{"Cost", 102}, {"Damage", 0}, {"Armor", 5}}},
    };
    map<string, map<string, int>> dmg_rings{
        {"No ring", {{"Cost", 0}, {"Damage", 0}, {"Armor", 0}}},
        {"Damage +1", {{"Cost", 25}, {"Damage", 1}, {"Armor", 0}}},
        {"Damage +2", {{"Cost", 50}, {"Damage", 2}, {"Armor", 0}}},
        {"Damage +3", {{"Cost", 100}, {"Damage", 3}, {"Armor", 0}}},
    };
    map<string, map<string, int>> arm_rings{
        {"No ring", {{"Cost", 0}, {"Damage", 0}, {"Armor", 0}}},
        {"Defense +1", {{"Cost", 20}, {"Damage", 0}, {"Armor", 1}}},
        {"Defense +2", {{"Cost", 40}, {"Damage", 0}, {"Armor", 2}}},
        {"Defense +3", {{"Cost", 80}, {"Damage", 0}, {"Armor", 3}}},
    };

    int total_cost{0};
    int total_damage{0};
    int total_armor{0};
    int max_cost = std::numeric_limits<int>::min();
    for (auto& [_, w] : weapons) {
        for (auto& [_, a] : armors) {
            for (auto& [_, dr] : dmg_rings) {
                for (auto& [_, ar] : arm_rings) {
                    total_cost =
                        w["Cost"] + a["Cost"] + dr["Cost"] + ar["Cost"];
                    total_damage =
                        w["Damage"] + a["Damage"] + dr["Damage"] + ar["Damage"];
                    total_armor =
                        w["Armor"] + a["Armor"] + dr["Armor"] + ar["Armor"];

                    int player_turns_to_kill =
                        boss["Hit Points"] /
                        (std::max(total_damage - boss["Armor"], 1));
                    int boss_turns_to_kill =
                        100 / (std::max(boss["Damage"] - total_armor, 1));

                    if (player_turns_to_kill > boss_turns_to_kill) {
                        max_cost = std::max(total_cost, max_cost);
                    }
                }
            }
        }
    }

    println("Resultado da parte 2: {}", max_cost);
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
