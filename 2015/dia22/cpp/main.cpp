// #include <Eigen/Dense>
#include <algorithm>
#include <boost/functional/hash.hpp>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
// #include <json.hpp>
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

struct State {
    int boss_hp;
    int boss_damage;
    int player_hp{50};
    int player_mana{500};
    int shield_timer{0};
    int poison_timer{0};
    int recharge_timer{0};
    int mana_spent{0};
    bool player_turn{true};

    State(int boss_hp, int boss_damage)
        : boss_hp{boss_hp}, boss_damage{boss_damage} {}
};

void simulate(State&& state, int& min_mana, bool hard_mode) {
    if (state.mana_spent >= min_mana) return;

    if (state.boss_hp <= 0) {
        min_mana = std::min(min_mana, state.mana_spent);
        return;
    }

    // Apply hard mode
    if (state.player_turn and hard_mode) {
        state.player_hp -= 1;
    }

    if (state.player_hp <= 0) return;

    // Apply effects
    if (state.shield_timer > 0) {
        state.shield_timer--;
    }
    if (state.poison_timer > 0) {
        state.boss_hp -= 3;
        state.poison_timer--;
    }
    if (state.recharge_timer > 0) {
        state.player_mana += 101;
        state.recharge_timer--;
    }

    if (state.boss_hp <= 0) {
        min_mana = std::min(min_mana, state.mana_spent);
        return;
    }

    if (state.player_turn) {
        // Player's turn
        if (state.player_mana >= 53) {
            State new_state{state};
            new_state.player_mana -= 53;
            new_state.mana_spent += 53;
            new_state.boss_hp -= 4;
            new_state.player_turn = false;
            simulate(std::move(new_state), min_mana, hard_mode);
        }
        if (state.player_mana >= 73) {
            State new_state{state};
            new_state.player_mana -= 73;
            new_state.mana_spent += 73;
            new_state.boss_hp -= 2;
            new_state.player_hp += 2;
            new_state.player_turn = false;
            simulate(std::move(new_state), min_mana, hard_mode);
        }
        if (state.player_mana >= 113 && state.shield_timer == 0) {
            State new_state{state};
            new_state.player_mana -= 113;
            new_state.mana_spent += 113;
            new_state.shield_timer = 6;
            new_state.player_turn = false;
            simulate(std::move(new_state), min_mana, hard_mode);
        }
        if (state.player_mana >= 173 && state.poison_timer == 0) {
            State new_state{state};
            new_state.player_mana -= 173;
            new_state.mana_spent += 173;
            new_state.poison_timer = 6;
            new_state.player_turn = false;
            simulate(std::move(new_state), min_mana, hard_mode);
        }
        if (state.player_mana >= 229 && state.recharge_timer == 0) {
            State new_state{state};
            new_state.player_mana -= 229;
            new_state.mana_spent += 229;
            new_state.recharge_timer = 5;
            new_state.player_turn = false;
            simulate(std::move(new_state), min_mana, hard_mode);
        }
    } else {
        // Boss's turn
        int damage = state.boss_damage;
        if (state.shield_timer > 0) {
            damage = std::max(1, damage - 7);
        }
        state.player_hp -= damage;
        state.player_turn = true;
        simulate(std::move(state), min_mana, hard_mode);
    }
}

void parte_1(std::vector<std::string>& lines) {
    unordered_map<string, int> boss_map;
    for (string line : lines) {
        std::smatch m;
        std::regex_match(line, m, std::regex{R"((\w+\s?\w+?): (\d+))"});
        boss_map[m[1]] = std::stoi(m[2]);
    }

    int min_spent_mana = std::numeric_limits<int>::max();
    State initial_state{boss_map["Hit Points"], boss_map["Damage"]};
    simulate(std::move(initial_state), min_spent_mana, false);

    println("Resultado da parte 1: {}", min_spent_mana);
};

void parte_2(std::vector<std::string>& lines) {
    unordered_map<string, int> boss_map;
    for (string line : lines) {
        std::smatch m;
        std::regex_match(line, m, std::regex{R"((\w+\s?\w+?): (\d+))"});
        boss_map[m[1]] = std::stoi(m[2]);
    }

    int min_spent_mana = std::numeric_limits<int>::max();
    State initial_state{boss_map["Hit Points"], boss_map["Damage"]};
    simulate(std::move(initial_state), min_spent_mana, true);

    println("Resultado da parte 2: {}", min_spent_mana);
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
