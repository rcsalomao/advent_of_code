// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
// #include <json.hpp>
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

struct Bot {
    ulong id;
    std::function<void(ulong, vector<ulong>&)> pass_low;
    std::function<void(ulong, vector<ulong>&)> pass_high;
    vector<ulong> chips{};

    Bot(ulong bot_id) : id(bot_id) {};
    Bot(Bot&& bot) {
        id = std::move(bot.id);
        pass_low = bot.pass_low;
        pass_high = bot.pass_high;
        chips = std::move(bot.chips);
    };

    void receive_value(ulong value, vector<ulong>& processing_bots) {
        chips.push_back(value);
        if (chips.size() > 1) {
            processing_bots.push_back(id);
        };
    }
};

struct Output {
    ulong id;
    vector<ulong> chips{};

    Output(ulong output_id) : id(output_id) {};

    void receive_value(ulong value, vector<ulong>&) { chips.push_back(value); }
};

void parte_1(std::vector<std::string>& lines) {
    unordered_map<ulong, Bot> bots;
    unordered_map<ulong, Output> outputs;
    vector<ulong> processing_bots;
    for (string line : lines) {
        regex value_pat{R"(^value (\d+) .* bot (\d+)$)"};
        regex distribution_pat{
            R"(^bot (\d+) .* low to (\w+) (\d+) .* high to (\w+) (\d+)$)"};
        smatch m;
        if (regex_match(line, m, value_pat)) {
            ulong bot_id{std::stoul(m[2])};
            if (!bots.contains(bot_id)) {
                bots.insert({bot_id, Bot{bot_id}});
            }
            auto& bot = bots.at(bot_id);
            bot.chips.push_back(std::stoul(m[1]));
            if (bot.chips.size() > 1) {
                processing_bots.push_back(bot_id);
            }
        }
        if (regex_match(line, m, distribution_pat)) {
            ulong bot_id0{std::stoul(m[1])};
            if (!bots.contains(bot_id0)) {
                bots.insert({bot_id0, Bot{bot_id0}});
            }
            if (m[2] == "bot") {
                ulong bot_id1{std::stoul(m[3])};
                if (!bots.contains(bot_id1)) {
                    bots.insert({bot_id1, Bot{bot_id1}});
                }
                bots.at(bot_id0).pass_low =
                    [&bots, bot_id1](ulong value,
                                     vector<ulong>& processing_bots) {
                        bots.at(bot_id1).receive_value(value, processing_bots);
                    };
            }
            if (m[2] == "output") {
                ulong output_id1{std::stoul(m[3])};
                if (!outputs.contains(output_id1)) {
                    outputs.insert({output_id1, Output{output_id1}});
                }
                bots.at(bot_id0).pass_low =
                    [&outputs, output_id1](ulong value,
                                           vector<ulong>& processing_bots) {
                        outputs.at(output_id1)
                            .receive_value(value, processing_bots);
                    };
            }
            if (m[4] == "bot") {
                ulong bot_id2{std::stoul(m[5])};
                if (!bots.contains(bot_id2)) {
                    bots.insert({bot_id2, Bot{bot_id2}});
                }
                bots.at(bot_id0).pass_high =
                    [&bots, bot_id2](ulong value,
                                     vector<ulong>& processing_bots) {
                        bots.at(bot_id2).receive_value(value, processing_bots);
                    };
            }
            if (m[4] == "output") {
                ulong output_id2{std::stoul(m[5])};
                if (!outputs.contains(output_id2)) {
                    outputs.insert({output_id2, Output{output_id2}});
                }
                bots.at(bot_id0).pass_high =
                    [&outputs, output_id2](ulong value,
                                           vector<ulong>& processing_bots) {
                        outputs.at(output_id2)
                            .receive_value(value, processing_bots);
                    };
            }
        }
    }

    // println("bots:");
    // for (auto& [id, bot] : bots) {
    //     println("{} {}", id, bot.chips);
    // }
    // println("Output bins:");
    // for (auto& [id, output] : outputs) {
    //     println("{} {}", id, output.chips);
    // }

    ulong res;
    while (!processing_bots.empty()) {
        ulong next_bot{processing_bots.back()};
        processing_bots.pop_back();
        auto& bot = bots.at(next_bot);
        auto [min_v, max_v] = rg::minmax(bot.chips);
        bot.pass_low(min_v, processing_bots);
        bot.pass_high(max_v, processing_bots);
        // println("{} {} {} {} {}", bot.id, processing_bots, min_v, max_v,
        //         bot.chips);
        std::erase(bot.chips, min_v);
        std::erase(bot.chips, max_v);

        if (min_v == 17 and max_v == 61) res = bot.id;
    }

    // println("bots:");
    // for (auto& [id, bot] : bots) {
    //     println("{} {}", id, bot.chips);
    // }
    // println("Output bins:");
    // for (auto& [id, output] : outputs) {
    //     println("{} {}", id, output.chips);
    // }

    println("Resultado da parte 1: {}", res);
};

void parte_2(std::vector<std::string>& lines) {
    unordered_map<ulong, Bot> bots;
    unordered_map<ulong, Output> outputs;
    vector<ulong> processing_bots;
    for (string line : lines) {
        regex value_pat{R"(^value (\d+) .* bot (\d+)$)"};
        regex distribution_pat{
            R"(^bot (\d+) .* low to (\w+) (\d+) .* high to (\w+) (\d+)$)"};
        smatch m;
        if (regex_match(line, m, value_pat)) {
            ulong bot_id{std::stoul(m[2])};
            if (!bots.contains(bot_id)) {
                bots.insert({bot_id, Bot{bot_id}});
            }
            auto& bot = bots.at(bot_id);
            bot.chips.push_back(std::stoul(m[1]));
            if (bot.chips.size() > 1) {
                processing_bots.push_back(bot_id);
            }
        }
        if (regex_match(line, m, distribution_pat)) {
            ulong bot_id0{std::stoul(m[1])};
            if (!bots.contains(bot_id0)) {
                bots.insert({bot_id0, Bot{bot_id0}});
            }
            if (m[2] == "bot") {
                ulong bot_id1{std::stoul(m[3])};
                if (!bots.contains(bot_id1)) {
                    bots.insert({bot_id1, Bot{bot_id1}});
                }
                bots.at(bot_id0).pass_low =
                    [&bots, bot_id1](ulong value,
                                     vector<ulong>& processing_bots) {
                        bots.at(bot_id1).receive_value(value, processing_bots);
                    };
            }
            if (m[2] == "output") {
                ulong output_id1{std::stoul(m[3])};
                if (!outputs.contains(output_id1)) {
                    outputs.insert({output_id1, Output{output_id1}});
                }
                bots.at(bot_id0).pass_low =
                    [&outputs, output_id1](ulong value,
                                           vector<ulong>& processing_bots) {
                        outputs.at(output_id1)
                            .receive_value(value, processing_bots);
                    };
            }
            if (m[4] == "bot") {
                ulong bot_id2{std::stoul(m[5])};
                if (!bots.contains(bot_id2)) {
                    bots.insert({bot_id2, Bot{bot_id2}});
                }
                bots.at(bot_id0).pass_high =
                    [&bots, bot_id2](ulong value,
                                     vector<ulong>& processing_bots) {
                        bots.at(bot_id2).receive_value(value, processing_bots);
                    };
            }
            if (m[4] == "output") {
                ulong output_id2{std::stoul(m[5])};
                if (!outputs.contains(output_id2)) {
                    outputs.insert({output_id2, Output{output_id2}});
                }
                bots.at(bot_id0).pass_high =
                    [&outputs, output_id2](ulong value,
                                           vector<ulong>& processing_bots) {
                        outputs.at(output_id2)
                            .receive_value(value, processing_bots);
                    };
            }
        }
    }

    // println("bots:");
    // for (auto& [id, bot] : bots) {
    //     println("{} {}", id, bot.chips);
    // }
    // println("Output bins:");
    // for (auto& [id, output] : outputs) {
    //     println("{} {}", id, output.chips);
    // }

    while (!processing_bots.empty()) {
        ulong next_bot{processing_bots.back()};
        processing_bots.pop_back();
        auto& bot = bots.at(next_bot);
        auto [min_v, max_v] = rg::minmax(bot.chips);
        bot.pass_low(min_v, processing_bots);
        bot.pass_high(max_v, processing_bots);
        // println("{} {} {} {} {}", bot.id, processing_bots, min_v, max_v,
        //         bot.chips);
        std::erase(bot.chips, min_v);
        std::erase(bot.chips, max_v);
    }

    // println("bots:");
    // for (auto& [id, bot] : bots) {
    //     println("{} {}", id, bot.chips);
    // }
    // println("Output bins:");
    // for (auto& [id, output] : outputs) {
    //     println("{} {}", id, output.chips);
    // }

    ulong res{outputs.at(0).chips[0] * outputs.at(1).chips[0] *
              outputs.at(2).chips[0]};
    println("Resultado da parte 2: {}", res);
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
