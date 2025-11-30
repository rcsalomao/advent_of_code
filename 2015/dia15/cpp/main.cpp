// #include <Eigen/Dense>
#include <algorithm>
// #include <boost/functional/hash.hpp>
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
    map<string, vector<int>> ingredients;
    for (auto line : lines) {
        std::smatch m;
        std::regex_match(
            line, m,
            std::regex{
                R"((\w+): capacity (-?\d+), durability (-?\d+), flavor (-?\d+), texture (-?\d+), calories (-?\d+))"});
        ingredients[m[1]] = vector<int>{
            std::from_range, m | vw::drop(2) | vw::transform([](auto& a) {
                                 return std::stoi(a);
                             })};
    }

    int max_score{0};
    // vector<int> proportions(4);

    for (int i : vw::iota(0, 100)) {
        auto vi{ingredients["Sprinkles"]};
        for (int j : vw::iota(0, 100 - i)) {
            auto vj{ingredients["PeanutButter"]};
            for (int k : vw::iota(0, 100 - i - j)) {
                auto vk{ingredients["Frosting"]};
                int l = 100 - i - j - k;
                auto vl{ingredients["Sugar"]};
                vector<int> val(4);
                for (int m : vw::iota(0u, val.size() + 1)) {
                    val[m] = std::max(
                        i * vi[m] + j * vj[m] + k * vk[m] + l * vl[m], 0);
                }
                int score =
                    std::reduce(val.begin(), val.end(), 1, std::multiplies<>{});
                if (score > max_score) {
                    max_score = score;
                    // proportions = vector<int>{i, j, k, l};
                    // println("{} {}", max_score, proportions);
                }
                // println("{} {} {} {}", i, j, k, l);
            }
        }
    }

    // mock_input
    // for (int i : vw::iota(0, 100)) {
    //     auto vi{ingredients["Butterscotch"]};
    //     int j = 100 - i;
    //     auto vj{ingredients["Cinnamon"]};
    //     vector<int> val(4);
    //     for (int m : vw::iota(0, val.size() + 1)) {
    //         val[m] = std::max(i * vi[m] + j * vj[m], 0);
    //     }
    //     int score = std::reduce(val.begin(), val.end(), 1,
    //     std::multiplies<>{}); if (score > max_score) {
    //         max_score = score;
    //         // proportions = vector<int>{i, j};
    //         // println("{} {}", max_score, proportions);
    //     }
    //     // println("{} {} {} {}", i, j, k, l);
    // }
    println("Resultado da parte 1: {}", max_score);
};

void parte_2(std::vector<std::string>& lines) {
    map<string, vector<int>> ingredients;
    for (auto line : lines) {
        std::smatch m;
        std::regex_match(
            line, m,
            std::regex{
                R"((\w+): capacity (-?\d+), durability (-?\d+), flavor (-?\d+), texture (-?\d+), calories (-?\d+))"});
        ingredients[m[1]] = vector<int>{
            std::from_range, m | vw::drop(2) | vw::transform([](auto& a) {
                                 return std::stoi(a);
                             })};
    }

    int max_score{0};
    // vector<int> proportions(4);

    for (int i : vw::iota(0, 100)) {
        auto vi{ingredients["Sprinkles"]};
        for (int j : vw::iota(0, 100 - i)) {
            auto vj{ingredients["PeanutButter"]};
            for (int k : vw::iota(0, 100 - i - j)) {
                auto vk{ingredients["Frosting"]};
                int l = 100 - i - j - k;
                auto vl{ingredients["Sugar"]};
                vector<int> val(5);
                for (int m : vw::iota(0u, val.size() + 1)) {
                    val[m] = std::max(
                        i * vi[m] + j * vj[m] + k * vk[m] + l * vl[m], 0);
                }
                int score = std::reduce(val.begin(), val.end() - 1, 1,
                                        std::multiplies<>{});
                if (score > max_score and val[4] == 500) {
                    max_score = score;
                    // proportions = vector<int>{i, j, k, l};
                    // println("{} {}", max_score, proportions);
                }
                // println("{} {} {} {}", i, j, k, l);
            }
        }
    }

    // mock_input
    // for (int i : vw::iota(0, 100)) {
    //     auto vi{ingredients["Butterscotch"]};
    //     int j = 100 - i;
    //     auto vj{ingredients["Cinnamon"]};
    //     vector<int> val(5);
    //     for (int m : vw::iota(0u, val.size() + 1)) {
    //         val[m] = std::max(i * vi[m] + j * vj[m], 0);
    //     }
    //     int score =
    //         std::reduce(val.begin(), val.end() - 1, 1, std::multiplies<>{});
    //     if (score > max_score and val[4] == 500) {
    //         max_score = score;
    //         // proportions = vector<int>{i, j};
    //         // println("{} {}", max_score, proportions);
    //     }
    //     // println("{} {} {} {}", i, j, k, l);
    // }
    println("Resultado da parte 2: {}", max_score);
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
