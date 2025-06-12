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

class Boss;

enum class Action { pass, magic_missile, drain, poison, shield, recharge };

class Player {
   public:
    Player() = default;

    int get_armor_value() {
        if (shield) {
            return armor + 7;
        } else {
            return armor;
        }
    }
    void receive_attack(int damage_value) {
        int armor_value = get_armor_value();
        hp -= std::max(1, damage_value - armor_value);
    }
    void cast_magic_missile(Boss& boss);
    void cast_drain(Boss& boss);
    void cast_poison(Boss& boss);
    void recharge_mana(int value) {
        if (recharge) {
            mana += value;
        }
    }
    void cast_recharge(int turns) {
        mana -= mana_cost[Action::recharge];
        if (recharge < 1) {
            recharge = turns;
        }
    }
    void cast_shield(int turns) {
        mana -= mana_cost[Action::shield];
        if (shield < 1) {
            shield = turns;
        }
    }
    int get_hp_value() { return hp; }
    int get_mana_value() { return mana; }
    bool check_shield() { return shield > 0; }
    void decrease_shield() { shield = std::max(0, shield - 1); }
    bool check_recharge() { return recharge > 0; }
    void decrease_recharge() { recharge = std::max(0, recharge - 1); }
    vector<Action> get_possible_actions(Boss& boss);

   private:
    int hp{50};
    int mana{500};
    int armor{0};
    int shield{0};
    int recharge{0};
    unordered_map<Action, int> mana_cost{
        {Action::magic_missile, 53}, {Action::drain, 73},
        {Action::shield, 113},       {Action::poison, 173},
        {Action::recharge, 229},
    };
};

class Boss {
   public:
    Boss(int hp_, int damage_) : hp(hp_), damage(damage_) {};

    void attack(Player& player) { player.receive_attack(damage); }
    void receive_attack(int damage_value) { hp -= damage_value; }
    void poison_damage() {
        if (poison) {
            hp -= 3;
        }
    }
    void apply_poison(int turns) {
        if (poison < 1) {
            poison = turns;
        }
    }
    int get_hp_value() { return hp; }
    bool check_poison() { return poison > 0; }
    void decrease_poison() { poison = std::max(0, poison - 1); }

   private:
    int hp;
    int damage;
    int armor{0};
    int poison{0};
};

void Player::cast_magic_missile(Boss& boss) {
    mana -= mana_cost[Action::magic_missile];
    boss.receive_attack(4);
}
void Player::cast_drain(Boss& boss) {
    boss.receive_attack(2);
    hp += 2;
}
void Player::cast_poison(Boss& boss) {
    mana -= mana_cost[Action::poison];
    boss.apply_poison(6);
}

vector<Action> Player::get_possible_actions(Boss& boss) {
    int mana_value = get_mana_value();
    vector<Action> actions;
    for (auto& [action, val] : mana_cost) {
        switch (action) {
            case Action::magic_missile:
                if (mana_value >= val) actions.emplace_back(action);
                break;
            case Action::drain:
                if (mana_value >= val) actions.emplace_back(action);
                break;
            case Action::shield:
                if (mana_value >= val and !check_shield())
                    actions.emplace_back(action);
                break;
            case Action::recharge:
                if (mana_value >= val and !check_recharge())
                    actions.emplace_back(action);
                break;
            case Action::poison:
                if (mana_value >= val and !boss.check_poison())
                    actions.emplace_back(action);
                break;
            default:
                break;
        }
    }
    return actions.size() ? actions : vector<Action>{Action::pass};
}
void process_turn(Player& player, Boss& boss, int& min_mana) {
    if (player.get_mana_value() > min_mana) return;
    if (player.get_hp_value() < 1) return;
    if (boss.check_poison()) {
        boss.receive_attack(3);
    }

    // player turn
    auto actions = player.get_possible_actions(boss);
    for (auto action : actions) {
        switch (action) {
            case Action::pass:
                break;
            case Action::magic_missile:
                player.cast_magic_missile(boss);
                break;
            case Action::drain:
                player.cast_drain(boss);
                break;
            case Action::poison:
                player.cast_poison(boss);
                break;
            case Action::shield:
                player.cast_shield(6);
                break;
            case Action::recharge:
                player.cast_shield(5);
                break;
            default:
                break;
        }
        if (boss.get_hp_value() < 1) {
            min_mana = std::min(min_mana, player.get_mana_value());
            return;
        }
        // boss turn
        if (boss.check_poison()) {
            boss.receive_attack(3);
        }
        boss.attack(player);
        process_turn(player, boss, min_mana);
    }
}

void parte_1(std::vector<std::string>& lines) {
    unordered_map<string, int> boss_map;
    for (string line : lines) {
        std::smatch m;
        std::regex_match(line, m, std::regex{R"((\w+\s?\w+?): (\d+))"});
        boss_map[m[1]] = std::stoi(m[2]);
    }
    println("{}", boss_map);

    Boss boss{boss_map["Hit Points"], boss_map["Damage"]};
    Player player{};

    int min_mana = std::numeric_limits<int>::max();
    process_turn(player, boss, min_mana);
    println("{} {}", player.get_hp_value(), player.get_mana_value());
    println("{}", boss.get_hp_value());

    println("Resultado da parte 1: {}", min_mana);
};

void parte_2(std::vector<std::string>& lines) {
    unordered_map<string, int> boss;
    for (string line : lines) {
        std::smatch m;
        std::regex_match(line, m, std::regex{R"((\w+\s?\w+?): (\d+))"});
        boss[m[1]] = std::stoi(m[2]);
    }
    // println("{}", boss);

    println("Resultado da parte 2: {}", "");
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
