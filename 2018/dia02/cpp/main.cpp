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
    long pat2_count{0};
    long pat3_count{0};
    for (auto line : lines) {
        unordered_map<char, int> count;
        for (char c : line) {
            count[c]++;
        }
        for (auto [k, v] : count) {
            if (v == 2) {
                pat2_count++;
                break;
            }
        }
        for (auto [k, v] : count) {
            if (v == 3) {
                pat3_count++;
                break;
            }
        }
    }
    return pat2_count * pat3_count;
}

auto parte_2(vector<string>& lines) {
    string w1;
    string w2;
    for (auto word_1 : lines) {
        bool stop{false};
        for (auto word_2 : lines) {
            if (word_1 == word_2) continue;
            int total_diff{0};
            bool jump_word{false};
            for (auto [a, b] : vw::zip(word_1, word_2)) {
                if (a != b) {
                    total_diff++;
                };
                if (total_diff > 1) {
                    jump_word = true;
                    break;
                }
            }
            if (jump_word) continue;
            stop = true;
            w1 = word_1;
            w2 = word_2;
        }
        if (stop) break;
    }

    int pos{0};
    for (auto [i, c] : vw::enumerate(w1)) {
        if (c != w2[i]) pos = i;
    }

    return w1.replace(pos, 1, "");
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
