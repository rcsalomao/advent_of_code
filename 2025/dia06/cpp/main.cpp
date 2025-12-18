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
    vector<vector<string>> data;
    for (string line : lines) {
        auto v = split(line, " ") |
                 vw::filter([](string_view s) { return s != ""; });
        data.emplace_back(v | rg::to<vector<string>>());
    }
    ulong total{0};
    for (size_t j : vw::iota(0u, data[0].size())) {
        vector<ulong> nums{};
        for (size_t i : vw::iota(0u, data.size())) {
            if (i < data.size() - 1) {
                nums.push_back(stoul(data[i][j]));
            } else {
                total += data[i][j] == "+" ? reduce(nums.begin(), nums.end())
                                           : reduce(nums.begin(), nums.end(),
                                                    1ul, multiplies<>{});
            }
        }
    }
    return total;
}

template <typename T>
class Grid {
   public:
    Grid() = default;

    T& operator[](bitset<64> i, bitset<32> j) {
        for (size_t n = 0; n < 32; n++) {
            i[32 + n] = j[n];
        }
        return grid_map[i];
    }
    bool check_key(bitset<64> i, bitset<32> j) {
        for (size_t n = 0; n < 32; n++) {
            i[32 + n] = j[n];
        }
        return grid_map.contains(i);
    }
    auto& get_map() { return grid_map; }

   private:
    unordered_map<bitset<64>, T> grid_map;
};

tuple<int32_t, int32_t> decode_grid_key(bitset<64> key) {
    bitset<32> j;
    for (size_t n = 0; n < 32; n++) {
        j[n] = key[32 + n];
    }
    return {key.to_ulong(), j.to_ulong()};
}

bitset<64> encode_grid_key(bitset<64> i, bitset<32> j) {
    for (size_t n = 0; n < 32; n++) {
        i[32 + n] = j[n];
    }
    return i;
}

auto parte_2(vector<string>& lines) {
    Grid<string> grid;
    uint32_t max_i{0}, max_j{0};
    for (auto [i, line] : vw::enumerate(lines)) {
        for (auto [j, c] : vw::enumerate(line)) {
            grid[i, j] = c;
            max_j = max(max_j, static_cast<uint32_t>(j));
        }
        max_i = max(max_i, static_cast<uint32_t>(i));
    }
    ulong total{0};
    uint32_t k{0};
    bool done{false};
    while (not done) {
        string op{grid[max_i, k]};
        uint32_t m{k};
        while (++m) {
            if (m > max_j) {
                m++;
                done = true;
                break;
            }
            if (grid[max_i, m] == "+" or grid[max_i, m] == "*") break;
        }
        ulong equation_number = op == "+" ? 0 : 1;
        for (auto j : vw::iota(k, m - 1)) {
            string str;
            for (auto i : vw::iota(0u, max_i)) {
                str += grid[i, j];
            }
            if (op == "+") {
                equation_number += stoi(str);
            } else {
                equation_number *= stoi(str);
            }
        }
        total += equation_number;
        k = m;
    }
    return total;
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
