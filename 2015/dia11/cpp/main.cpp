#include <Eigen/Dense>
#include <algorithm>
#include <boost/functional/hash.hpp>
#include <fstream>
#include <functional>
#include <iomanip>
#include <limits>
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
using std::print;
using std::println;
using std::string;
using std::string_view;
using std::to_string;
using std::tuple;
using std::unordered_map;
using std::vector;

vector<string> split(const string& str, string_view delim) {
    return vector<string>{std::from_range,
                          str | vw::split(delim) | vw::transform([](auto a) {
                              return string{std::from_range, a};
                          })};
}

// string alphabet = "abcdefghijklmnopqrstuvxyz";
string alphabet = "abcdefghjkmnpqrstuvxyz";
size_t n{alphabet.size()};
unordered_map<char, int> alphabet_map{
    std::from_range, alphabet | vw::transform([n = 0](char c) mutable {
                         return std::pair{c, n++};
                     })};

vector<int> get_num(string& word) {
    vector<int> pos;
    for (auto c : word) {
        pos.push_back(alphabet_map[c]);
    }
    return pos;
}

string get_word(vector<int>& digits) {
    string word{""};
    for (auto d : digits) {
        word += alphabet[d];
    }
    return word;
}

vector<int> to_base(size_t number, int base) {
    if (!number) return vector<int>{0};
    vector<int> digits;
    while (number) {
        digits.push_back(number % base);
        number /= base;
    }
    return vector<int>{std::from_range, digits | vw::reverse};
};

size_t from_base(vector<int>& digits, int base) {
    size_t power{1};
    size_t number{0};
    for (int digit : digits | vw::reverse) {
        number += digit * power;
        power *= base;
    }
    return number;
}

string increment_word(string& word) {
    auto digits = get_num(word);
    auto number = from_base(digits, n);
    number += 1;
    auto new_digits = to_base(number, n);
    return get_word(new_digits);
}

vector<std::regex> v_cond_1{
    std::from_range, vw::iota(0u, n - 2) | vw::transform([](auto i) {
                         return std::regex{".*" + alphabet.substr(i, 3) + ".*"};
                     })};

bool check_conditions(string& word) {
    bool cond_1_flag{false};
    for (auto cond : v_cond_1) {
        if (std::regex_match(word, cond)) {
            cond_1_flag = true;
            break;
        }
    }
    std::regex cond_2{R"(.*(\w)\1.*(\w)\2.*)"};
    bool cond_2_flag{std::regex_match(word, cond_2)};
    // println("{} {}", cond_1_flag, cond_2_flag);
    if (cond_1_flag && cond_2_flag) {
        return true;
    } else {
        return false;
    };
}

void parte_1(std::vector<std::string>& lines) {
    string word{lines[0]};
    while (true) {
        word = increment_word(word);
        bool test = check_conditions(word);
        // println("{} {}", word, test);
        if (test) break;
    }
    println("Resultado da parte 1: {}", word);
};

void parte_2(std::vector<std::string>& lines) {
    string word{lines[0]};
    while (true) {
        word = increment_word(word);
        bool test = check_conditions(word);
        // println("{} {}", word, test);
        if (test) break;
    }
    while (true) {
        word = increment_word(word);
        bool test = check_conditions(word);
        // println("{} {}", word, test);
        if (test) break;
    }
    println("Resultado da parte 2: {}", word);
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
    // parte_2(lines);

    return 0;
}
