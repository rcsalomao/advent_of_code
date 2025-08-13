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

void parte_1(vector<string>& lines) {
    int max_layer_id{0};
    unordered_map<int, int> layer_depths;
    regex pat{R"((\d+): (\d+))"};
    for (auto line : lines) {
        smatch m;
        regex_match(line, m, pat);
        int layer_id{stoi(m[1].str())};
        layer_depths[layer_id] = stoi(m[2].str());
        max_layer_id = std::max(max_layer_id, layer_id);
    }
    int n_layers{max_layer_id + 1};

    vector<int> caught_layers;
    for (int i : vw::iota(0, n_layers)) {
        if (layer_depths.contains(i)) {
            int cicle{2 * (layer_depths[i] - 1)};
            if (i % cicle == 0) {
                caught_layers.push_back(i);
            }
        }
    }

    long severity = std::accumulate(
        caught_layers.begin(), caught_layers.end(), 0,
        [&layer_depths](long acc, int i) { return acc + i * layer_depths[i]; });

    println("Resultado da parte 1: {}", severity);
}

void parte_2(vector<string>& lines) {
    int max_layer_id{0};
    unordered_map<int, int> layer_depths;
    regex pat{R"((\d+): (\d+))"};
    for (auto line : lines) {
        smatch m;
        regex_match(line, m, pat);
        int layer_id{stoi(m[1].str())};
        layer_depths[layer_id] = stoi(m[2].str());
        max_layer_id = std::max(max_layer_id, layer_id);
    }
    int n_layers{max_layer_id + 1};

    int delay{0};
    while (true) {
        int caught_layers{0};
        for (int i : vw::iota(0, n_layers)) {
            if (layer_depths.contains(i)) {
                int cicle{2 * (layer_depths[i] - 1)};
                if ((i + delay) % cicle == 0) {
                    caught_layers++;
                }
            }
        }

        if (caught_layers == 0) break;
        delay++;
    }

    println("Resultado da parte 2: {}", delay);
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
