// #include <Eigen/Dense>
// #include <nlohmann/json.hpp>
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
            print("{:2}", grid[i, j]);
        }
        println();
    }
}

int calc_power_level(int serial_n, int x, int y) {
    int rack_id{x + 10};
    int power_level{y * rack_id};
    power_level += serial_n;
    power_level *= rack_id;
    power_level %= 1000;
    power_level /= 100;
    return power_level - 5;
}

tuple<int, int> calc_coordinate_p1(int serial_number) {
    Grid<int> grid;
    for (size_t i = 0; i < 300; i++) {
        for (size_t j = 0; j < 300; j++) {
            int pl = calc_power_level(serial_number, j, i);
            grid[i, j] = pl;
        }
    }
    int max_power{std::numeric_limits<int>::min()};
    tuple<int, int> coord;
    for (size_t i = 0; i < 300 - 3; i++) {
        for (size_t j = 0; j < 300 - 3; j++) {
            int total_power{0};
            for (size_t m = 0; m < 3; m++) {
                for (size_t n = 0; n < 3; n++) {
                    total_power += grid[i + m, j + n];
                }
            }
            if (total_power > max_power) {
                max_power = total_power;
                coord = {j, i};
            }
        }
    }
    return coord;
}

auto parte_1(vector<string>& lines) {
    int rx{-1};
    int ry{-1};
    for (auto line : lines) {
        int serial_number{stoi(line)};
        auto [x, y] = calc_coordinate_p1(serial_number);
        // println("{},{}", x, y);
        rx = x;
        ry = y;
    }
    return std::format("{},{}", rx, ry);
}

tuple<int, int, int> calc_coordinate_p2(int serial_number) {
    Grid<int> grid;
    for (size_t i = 0; i < 300; i++) {
        for (size_t j = 0; j < 300; j++) {
            int pl = calc_power_level(serial_number, j, i);
            grid[i, j] = pl;
        }
    }

    Grid<int> summed_area;
    for (size_t j = 0; j < 300; j++) {
        int total_power{0};
        for (size_t n = 0; n < j + 1; n++) {
            total_power += grid[0, n];
        }
        summed_area[0, j] = total_power;
    }
    for (size_t i = 1; i < 300; i++) {
        for (size_t j = 0; j < 300; j++) {
            summed_area[i, j] = summed_area[i - 1, j] + summed_area[i, j - 1] -
                                summed_area[i - 1, j - 1] + grid[i, j];
        }
    }

    int max_power{std::numeric_limits<int>::min()};
    tuple<int, int, int> coord;
    for (size_t sz = 2; sz < 300 - 2; sz++) {
        for (size_t i = 0; i < 300 - sz; i++) {
            for (size_t j = 0; j < 300 - sz; j++) {
                int A = summed_area[i, j];
                int B = summed_area[i, j + sz];
                int C = summed_area[i + sz, j];
                int D = summed_area[i + sz, j + sz];
                int total_power{D - C - B + A};
                if (total_power > max_power) {
                    max_power = total_power;
                    coord = {j + 1, i + 1, sz};
                }
            }
        }
    }

    return coord;
}

auto parte_2(vector<string>& lines) {
    int rx{-1};
    int ry{-1};
    int rs{-1};
    for (auto line : lines | vw::take(1)) {
        int serial_number{stoi(line)};
        auto [x, y, s] = calc_coordinate_p2(serial_number);
        // println("{},{}", x, y);
        rx = x;
        ry = y;
        rs = s;
    }
    return std::format("{},{},{}", rx, ry, rs);
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

    std::print("\n");
    println("Resultado da parte 1: {}", parte_1(lines));
    println("Resultado da parte 2: {}", parte_2(lines));

    return 0;
}
