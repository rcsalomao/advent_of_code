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
    set<pair<ulong, ulong>> id_ranges;
    int fresh_ids{0};
    for (string line : lines) {
        if (line == "") continue;
        if (line.contains('-')) {
            auto v = split(line, "-");
            ulong a = stoll(v[0]);
            ulong b = stoll(v[1]);
            id_ranges.insert(pair{a, b});
        } else {
            ulong id = stoll(line);
            for (auto [a, b] : id_ranges) {
                if (id >= a and id <= b) {
                    fresh_ids++;
                    break;
                }
            }
        }
    }
    return fresh_ids;
}

auto parte_2(vector<string>& lines) {
    set<pair<ulong, ulong>> id_ranges;
    for (string line : lines) {
        if (line == "") continue;
        if (line.contains('-')) {
            auto v = split(line, "-");
            ulong a = stoll(v[0]);
            ulong b = stoll(v[1]);
            id_ranges.insert(pair{a, b});
        }
    }
    ulong total_ids{0};
    ulong max_id_num{0};
    for (auto [a, b] : id_ranges) {
        if (b <= max_id_num) continue;
        if (a <= max_id_num) {
            total_ids += b - max_id_num;
            max_id_num = b;
        } else {
            total_ids += b - a + 1;
            max_id_num = b;
        }
    }
    return total_ids;
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
