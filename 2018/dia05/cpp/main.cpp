// #include <Eigen/Dense>
// #include <json.hpp>
// #include <openssl/md5.h>
#include <bits/stdc++.h>

namespace rg = std::ranges;
namespace vw = std::views;
using namespace std::string_literals;
using namespace std::string_view_literals;
using std::deque;
using std::from_range;
using std::map;
using std::print;
using std::println;
using std::queue;
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

auto parte_1(vector<string>& lines) {
    string polymer{lines.front()};

    bool done{false};
    while (not done) {
        done = true;
        for (auto i : vw::iota(0u, polymer.size() - 1)) {
            if (std::isupper(polymer[i]) and std::isupper(polymer[i + 1]))
                continue;
            if (std::islower(polymer[i]) and std::islower(polymer[i + 1]))
                continue;
            if (std::tolower(polymer[i]) == std::tolower(polymer[i + 1])) {
                polymer.erase(i, 2);
                done = false;
            }
        }
    }

    return polymer.size();
}

auto parte_2(vector<string>& lines) {
    string start_polymer{lines.front()};

    string alphabet{"abcdefghijklmnopqrstuvxywz"};

    unordered_map<char, size_t> lengths;
    for (char c : alphabet) {
        string polymer{start_polymer};
        bool done{false};
        while (not done) {
            done = true;
            for (auto i : vw::iota(0u, polymer.size())) {
                if (c == std::tolower(polymer[i])) {
                    polymer.erase(i, 1);
                    done = false;
                }
            }
        }
        done = false;
        while (not done) {
            done = true;
            for (auto i : vw::iota(0u, polymer.size() - 1)) {
                if (std::isupper(polymer[i]) and std::isupper(polymer[i + 1]))
                    continue;
                if (std::islower(polymer[i]) and std::islower(polymer[i + 1]))
                    continue;
                if (std::tolower(polymer[i]) == std::tolower(polymer[i + 1])) {
                    polymer.erase(i, 2);
                    done = false;
                }
            }
        }
        lengths[c] = polymer.size();
    }

    size_t min_length{std::numeric_limits<size_t>::max()};
    for (auto [k, v] : lengths) {
        min_length = std::min(v, min_length);
    }

    return min_length;
}

int main(int, char** argv) {
    std::ifstream input;
    input.open(argv[1]);

    string line;
    vector<string> lines;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    input.close();

    std::print("\n");
    println("Resultado da parte 1: {}", parte_1(lines));
    println("Resultado da parte 2: {}", parte_2(lines));

    return 0;
}
