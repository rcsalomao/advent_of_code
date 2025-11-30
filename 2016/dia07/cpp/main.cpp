// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
// #include <nlohmann/json.hpp>
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

bool has_abba(string& seq) {
    std::smatch m;
    std::regex pat{R"(.*(\w)(\w)\2\1.*)"};
    return std::regex_match(seq, m, pat) and m[1] != m[2];
}

void parte_1(std::vector<std::string>& lines) {
    unsigned n_TLS_IPs{0};
    vector<std::regex> patterns;
    for (int i : vw::iota(0, 3)) {
        string pat{R"(^(\w+))"};
        for (int _ : vw::iota(0, i + 1)) {
            pat += R"(\[(\w+)\](\w+))";
        }
        pat += R"($)";
        patterns.emplace_back(pat);
    }
    for (auto line : lines) {
        vector<string> super;
        vector<string> hyper;
        for (auto& p : patterns) {
            std::smatch m;
            if (std::regex_match(line, m, p)) {
                for (unsigned i : vw::iota(1u, m.size())) {
                    if (i % 2) {
                        super.emplace_back(m[i]);
                    } else {
                        hyper.emplace_back(m[i]);
                    }
                }
                break;
            }
        }
        bool super_has_abba{false};
        bool hyper_has_abba{false};
        for (string w : super) {
            if (has_abba(w)) {
                super_has_abba = true;
                break;
            }
        }
        for (string w : hyper) {
            if (has_abba(w)) {
                hyper_has_abba = true;
                break;
            }
        }
        if (super_has_abba and !hyper_has_abba) n_TLS_IPs++;
    }
    println("Resultado da parte 1: {}", n_TLS_IPs);
};

void parte_2(std::vector<std::string>& lines) {
    unsigned n_SSL_IPs{0};
    vector<std::regex> patterns;
    for (int i : vw::iota(0, 3)) {
        string pat{R"(^(\w+))"};
        for (int _ : vw::iota(0, i + 1)) {
            pat += R"(\[(\w+)\](\w+))";
        }
        pat += R"($)";
        patterns.emplace_back(pat);
    }
    for (auto line : lines) {
        vector<string> super;
        vector<string> hyper;
        for (auto& p : patterns) {
            std::smatch m;
            if (std::regex_match(line, m, p)) {
                for (unsigned i : vw::iota(1u, m.size())) {
                    if (i % 2) {
                        super.emplace_back(m[i]);
                    } else {
                        hyper.emplace_back(m[i]);
                    }
                }
                break;
            }
        }
        vector<string> babs;
        for (string w : super) {
            for (auto rng : w | vw::slide(3)) {
                if (rng[0] == rng[2] and rng[0] != rng[1]) {
                    babs.push_back(string{rng[1], rng[0], rng[1]});
                }
            }
        }
        bool is_SSL{false};
        for (string w : babs) {
            for (string h : hyper) {
                if (h.contains(w)) {
                    is_SSL = true;
                    break;
                }
            }
            if (is_SSL) break;
        }
        if (is_SSL) n_SSL_IPs++;
    }
    println("Resultado da parte 2: {}", n_SSL_IPs);
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
