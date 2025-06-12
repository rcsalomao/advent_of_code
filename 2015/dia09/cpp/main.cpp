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
using std::vector;

void parte_1(std::vector<std::string>& lines) {
    std::set<string> all_cities;
    vector<vector<string>> permutations;
    std::unordered_map<std::tuple<string, string>, int,
                       boost::hash<std::tuple<string, string>>>
        cities_distances;
    for (string_view line : lines) {
        vector<string> cities_dist{
            std::from_range,
            line | vw::split(" = "sv) | vw::transform([](auto a) {
                return std::string{std::from_range, a};
            })};
        vector<string> cities{
            std::from_range,
            cities_dist[0] | vw::split(" to "sv) | vw::transform([](auto a) {
                return std::string{std::from_range, a};
            })};
        for (auto& c : cities) {
            all_cities.insert(c);
        };
        cities_distances[std::make_tuple(cities[0], cities[1])] =
            std::stoi(cities_dist[1]);
        cities_distances[std::make_tuple(cities[1], cities[0])] =
            std::stoi(cities_dist[1]);
    }
    vector<string> ac{std::from_range, all_cities};
    permutations.emplace_back(ac);
    while (std::next_permutation(ac.begin(), ac.end())) {
        permutations.emplace_back(ac);
    }
    int min_dist = std::numeric_limits<int>::max();
    for (auto& p : permutations) {
        int dist{0};
        for (size_t i{0}; i < p.size() - 1; i++) {
            dist += cities_distances[std::tuple{p[i], p[i + 1]}];
        }
        if (dist < min_dist) {
            min_dist = dist;
        }
    }
    println("Resultado da parte 1: {}", min_dist);
};

void parte_2(std::vector<std::string>& lines) {
    std::set<string> all_cities;
    vector<vector<string>> permutations;
    std::unordered_map<std::tuple<string, string>, int,
                       boost::hash<std::tuple<string, string>>>
        cities_distances;
    for (string_view line : lines) {
        vector<string> cities_dist{
            std::from_range,
            line | vw::split(" = "sv) | vw::transform([](auto a) {
                return std::string{std::from_range, a};
            })};
        vector<string> cities{
            std::from_range,
            cities_dist[0] | vw::split(" to "sv) | vw::transform([](auto a) {
                return std::string{std::from_range, a};
            })};
        for (auto& c : cities) {
            all_cities.insert(c);
        };
        cities_distances[std::make_tuple(cities[0], cities[1])] =
            std::stoi(cities_dist[1]);
        cities_distances[std::make_tuple(cities[1], cities[0])] =
            std::stoi(cities_dist[1]);
    }
    vector<string> ac{std::from_range, all_cities};
    permutations.emplace_back(ac);
    while (std::next_permutation(ac.begin(), ac.end())) {
        permutations.emplace_back(ac);
    }
    int max_dist = std::numeric_limits<int>::min();
    for (auto& p : permutations) {
        int dist{0};
        for (size_t i{0}; i < p.size() - 1; i++) {
            dist += cities_distances[std::tuple{p[i], p[i + 1]}];
        }
        if (dist > max_dist) {
            max_dist = dist;
        }
    }
    println("Resultado da parte 2: {}", max_dist);
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
