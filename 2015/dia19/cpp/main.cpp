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

enum class ModoLeitura { transformações, molécula };

void parte_1(std::vector<std::string>& lines) {
    unordered_map<string, vector<string>> transformations;
    ModoLeitura mode{ModoLeitura::transformações};
    string big_molecule;
    for (string line : lines) {
        if (line == "") {
            mode = ModoLeitura::molécula;
            continue;
        };
        if (mode == ModoLeitura::transformações) {
            std::smatch m;
            std::regex_match(line, m, std::regex{R"((\w+) => (\w+))"});
            if (transformations.contains(m[1])) {
                transformations[m[1]].push_back(m[2]);
            } else {
                transformations[m[1]] = vector<string>{m[2]};
            }
        } else {
            big_molecule = line;
        }
    }

    std::set<string> molecules;
    for (auto& [k, vetor] : transformations) {
        for (string_view item : vetor) {
            size_t pos = big_molecule.find(k, 0);
            while (pos != string::npos) {
                molecules.insert(
                    string{big_molecule}.replace(pos, k.size(), item));
                pos = big_molecule.find(k, pos + 1);
            }
        }
    }

    println("Resultado da parte 1: {}", molecules.size());
};

void parte_2(std::vector<std::string>& lines) {
    unordered_map<string, string> inverse_transformations;
    ModoLeitura mode{ModoLeitura::transformações};
    string big_molecule;
    for (string line : lines) {
        if (line == "") {
            mode = ModoLeitura::molécula;
            continue;
        };
        if (mode == ModoLeitura::transformações) {
            std::smatch m;
            std::regex_match(line, m, std::regex{R"((\w+) => (\w+))"});
            inverse_transformations[m[2]] = m[1];
        } else {
            big_molecule = line;
        }
    }

    unsigned n{0};
    while (big_molecule != "e") {
        for (auto& [k, v] : inverse_transformations) {
            size_t pos = big_molecule.find(k, 0);
            if (pos != string::npos) {
                big_molecule.replace(pos, k.size(), v);
                n++;
            }
        }
    }

    println("Resultado da parte 2: {}", n);
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
