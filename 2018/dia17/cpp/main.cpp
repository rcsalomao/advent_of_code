// #include <Eigen/Dense>
// #include <nlohmann/json.hpp>
// #include <openssl/md5.h>

#include <bits/stdc++.h>
using namespace std;

namespace rg = std::ranges;
namespace vw = std::views;
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

auto parte_1(vector<string>& lines) { return 0; }

auto parte_2(vector<string>& lines) { return 0; }

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
