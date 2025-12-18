// #include <Eigen/Dense>
// #include <nlohmann/json.hpp>
// #include <openssl/md5.h>
#include <bits/stdc++.h>

namespace rg = std::ranges;
namespace vw = std::views;
using namespace std;
using namespace std::string_literals;
using namespace std::string_view_literals;

// using nlohmann::json;

void pr(auto&& val, auto&&... vals) {
    std::stringstream ss;
    ss << std::format("{}", val);
    (..., (ss << " " << std::format("{}", vals)));
    println("{}", ss.str());
}

vector<string> split(string const& str, string_view delim) {
    return vector<string>{std::from_range,
                          str | vw::split(delim) | vw::transform([](auto a) {
                              return string{std::from_range, a};
                          })};
}

auto parte_1(vector<string>& lines) {
    vector<int> banks_joltage{};
    for (string& line : lines) {
        for (int i{9}; i > 0; i--) {
            auto a = line.substr(0, line.size() - 1).find(to_string(i));
            if (a >= line.size() - 1) continue;
            for (int j{9}; j > 0; j--) {
                auto b = line.find(to_string(j), a + 1);
                if (b < line.size()) {
                    string c{line[a]};
                    string d{line[b]};
                    banks_joltage.emplace_back(stoi(c + d));
                    break;
                }
            }
            break;
        }
    }
    return reduce(banks_joltage.begin(), banks_joltage.end());
}

auto parte_2(vector<string>& lines) {
    int const n{12};
    vector<uint64_t> banks_joltage{};
    for (string& line : lines) {
        vector<char> vc{};
        int k{0};
        for (int i{0}; i < n; i++) {
            for (int j{9}; j > 0; j--) {
                auto a = line.substr(k, line.size() - k - (n - i - 1))
                             .find(to_string(j));
                if (a > line.size() - k - (n - i - 1)) continue;
                vc.emplace_back(line[k + a]);
                k += a + 1;
                break;
            }
        }
        banks_joltage.emplace_back(stoll(string{from_range, vc}));
    }
    return reduce(banks_joltage.begin(), banks_joltage.end());
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

    println();
    println("Resultado da parte 1: {}", parte_1(lines));
    println("Resultado da parte 2: {}", parte_2(lines));

    return 0;
}
