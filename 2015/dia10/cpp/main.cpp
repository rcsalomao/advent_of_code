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

string look_and_say(string& s) {
    string word{""};
    for (size_t i{0}; i < s.size();) {
        int count{0};
        char c = s[i];
        for (size_t j{i};; j++) {
            if (s[j] != c) {
                i = j;
                break;
            }
            count++;
        }
        word += to_string(count) + c;
    }
    return word;
}

void parte_1(std::vector<std::string>& lines) {
    string input = lines[0];
    string output{input};
    int n_turns{40};
    for (int _ : vw::iota(0, n_turns)) {
        output = look_and_say(output);
    }
    println("{}", output.size());
};

void parte_2(std::vector<std::string>& lines) {
    string input = lines[0];
    string output{input};
    int n_turns{50};
    for (int _ : vw::iota(0, n_turns)) {
        output = look_and_say(output);
    }
    println("{}", output.size());
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
