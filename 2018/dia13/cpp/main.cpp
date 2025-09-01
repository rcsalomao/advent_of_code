// #include <Eigen/Dense>
// #include <json.hpp>
// #include <openssl/md5.h>
#include <bits/stdc++.h>

namespace rg = std::ranges;
namespace vw = std::views;
using namespace std::string_literals;
using namespace std::string_view_literals;
using std::deque;
using std::from_range;
using std::map;
using std::print;
using std::println;
using std::queue;
using std::regex;
using std::regex_match;
using std::regex_search;
using std::set;
using std::shared_ptr;
using std::smatch;
using std::string;
using std::string_view;
using std::to_string;
using std::tuple;
using std::unique_ptr;
using std::unordered_map;
using std::unordered_set;
using std::vector;

// using nlohmann::json;

template <typename T>
void pln(T&& a) {
    println("{}", a);
}

template <typename T>
class Grid {
   public:
    Grid() = default;

    T& operator[](int32_t i, int32_t j) {
        std::bitset<32> i_bs = i;
        std::bitset<32> j_bs = j;
        std::bitset<64> key;
        for (size_t m = 0; m < 32; m++) {
            key[m] = i_bs[m];
        }
        for (size_t n = 0; n < 32; n++) {
            key[32 + n] = j_bs[n];
        }
        return grid_map[key];
    }
    bool check_key(int32_t i, int32_t j) {
        std::bitset<32> i_bs = i;
        std::bitset<32> j_bs = j;
        std::bitset<64> key;
        for (size_t m = 0; m < 32; m++) {
            key[m] = i_bs[m];
        }
        for (size_t n = 0; n < 32; n++) {
            key[32 + n] = j_bs[n];
        }
        return grid_map.contains(key);
    }
    auto& get_map() { return grid_map; }

   private:
    unordered_map<std::bitset<64>, T> grid_map;
};

tuple<int32_t, int32_t> decode_grid_key(std::bitset<64> key) {
    std::bitset<32> i_bs;
    std::bitset<32> j_bs;
    for (size_t m = 0; m < 32; m++) {
        i_bs[m] = key[m];
    }
    for (size_t n = 0; n < 32; n++) {
        j_bs[n] = key[32 + n];
    }
    return {i_bs.to_ulong(), i_bs.to_ulong()};
}

template <typename T>
void print_grid(Grid<T>& grid, int32_t min_i, int32_t max_i, int32_t min_j,
                int32_t max_j) {
    for (int32_t i = min_i; i < max_i + 1; i++) {
        for (int32_t j = min_j; j < max_j + 1; j++) {
            print("{:1}", grid[i, j]);
        }
        println();
    }
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

    std::print("\n");
    println("Resultado da parte 1: {}", parte_1(lines));
    println("Resultado da parte 2: {}", parte_2(lines));

    return 0;
}
