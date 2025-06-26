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
    unsigned long sum_ids{0};
    for (auto line : lines) {
        std::regex pattern{R"((.+)(\d{3})\[(\w+)\])"};
        std::smatch m;
        std::regex_match(line, m, pattern);
        unordered_map<char, unsigned> word_count;
        string room_name{m[1]};
        for (char c : room_name) {
            if (c == '-') continue;
            word_count[c]++;
        }
        std::map<unsigned, std::set<char>> char_map;
        for (auto& [c, n] : word_count) {
            char_map[n].insert(c);
        }
        std::stringstream sstream;
        for (auto& [n, seq] : vw::reverse(char_map)) {
            for (auto& c : seq) {
                sstream << c;
            }
        }
        string check_sum{m[3]};
        string computed_check_sum;
        sstream >> computed_check_sum;
        if (computed_check_sum.size() < 5) continue;
        if (computed_check_sum.substr(0, 5) != check_sum) continue;
        sum_ids += std::stoul(m[2].str());
    }
    println("Resultado da parte 1: {}", sum_ids);
};

void parte_2(std::vector<std::string>& lines) {
    unsigned long storage_room_id;
    string alphabet{"abcdefghijklmnopqrstuvwxyz"};
    for (auto line : lines) {
        std::regex pattern{R"((.+)(\d{3})\[(\w+)\])"};
        std::smatch m;
        std::regex_match(line, m, pattern);
        unordered_map<char, unsigned> word_count;
        string room_name{m[1]};
        for (char c : room_name) {
            if (c == '-') continue;
            word_count[c]++;
        }
        std::map<unsigned, std::set<char>> char_map;
        for (auto& [c, n] : word_count) {
            char_map[n].insert(c);
        }
        std::stringstream sstream;
        for (auto& [n, seq] : vw::reverse(char_map)) {
            for (auto& c : seq) {
                sstream << c;
            }
        }
        string check_sum{m[3]};
        string computed_check_sum;
        sstream >> computed_check_sum;
        if (computed_check_sum.size() < 5) continue;
        if (computed_check_sum.substr(0, 5) != check_sum) continue;
        unsigned long room_id{std::stoul(m[2].str())};
        for (char& c : room_name) {
            switch (c) {
                case '-':
                    c = ' ';
                    break;
                default:
                    c = alphabet[(alphabet.find(c) + room_id) %
                                 alphabet.size()];
                    break;
            }
        }
        if (room_name.contains("northpole")) {
            storage_room_id = room_id;
            break;
        }
    }

    println("Resultado da parte 2: {}", storage_room_id);
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
