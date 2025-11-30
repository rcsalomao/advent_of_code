// #include <Eigen/Dense>
// #include <nlohmann/json.hpp>
// #include <openssl/md5.h>
#include <bits/stdc++.h>

namespace rg = std::ranges;
namespace vw = std::views;
using namespace std::string_literals;
using namespace std::string_view_literals;
using std::bitset;
using std::deque;
using std::from_range;
using std::map;
using std::print;
using std::println;
using std::priority_queue;
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

void pr(auto&& val, auto&&... vals) {
    std::stringstream ss;
    ss << std::format("{}", val);
    (..., (ss << " " << std::format("{}", vals)));
    println("{}", ss.str());
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

const unordered_map<char, std::pair<int, int>> direction_map{
    {'>', {0, 1}}, {'<', {0, -1}}, {'^', {-1, 0}}, {'v', {1, 0}}};

const unordered_map<char, char> turn_map{{'l', 's'}, {'s', 'r'}, {'r', 'l'}};

const unordered_map<char, unordered_map<char, char>> corner_direction_map{
    {'>', {{'/', '^'}, {'\\', 'v'}}},
    {'<', {{'/', 'v'}, {'\\', '^'}}},
    {'v', {{'/', '<'}, {'\\', '>'}}},
    {'^', {{'/', '>'}, {'\\', '<'}}}};

const unordered_map<char, unordered_map<char, char>> turn_direction_map{
    {'>', {{'s', '>'}, {'l', '^'}, {'r', 'v'}}},
    {'<', {{'s', '<'}, {'l', 'v'}, {'r', '^'}}},
    {'v', {{'s', 'v'}, {'l', '>'}, {'r', '<'}}},
    {'^', {{'s', '^'}, {'l', '<'}, {'r', '>'}}},
};

struct Cart {
    std::pair<int, int> pos;
    char direction;
    char turn;
    bool crashed{false};
};

auto parte_1(vector<string>& lines) {
    vector<Cart> carts;
    Grid<char> grid;
    for (auto [i, line] : vw::enumerate(lines)) {
        for (auto [j, c] : vw::enumerate(line)) {
            switch (c) {
                case '<':
                    grid[i, j] = '-';
                    carts.emplace_back(std::pair<int, int>{i, j}, c, 'l');
                    break;
                case '>':
                    grid[i, j] = '-';
                    carts.emplace_back(std::pair<int, int>{i, j}, c, 'l');
                    break;
                case '^':
                    grid[i, j] = '|';
                    carts.emplace_back(std::pair<int, int>{i, j}, c, 'l');
                    break;
                case 'v':
                    grid[i, j] = '|';
                    carts.emplace_back(std::pair<int, int>{i, j}, c, 'l');
                    break;
                default:
                    grid[i, j] = c;
                    break;
            }
        }
    }
    std::pair<int, int> coord;
    bool done{false};
    while (not done) {
        for (auto [m, cart] : vw::enumerate(carts)) {
            if (cart.crashed) continue;
            auto [i, j] = direction_map.at(cart.direction);
            cart.pos.first += i;
            cart.pos.second += j;
            char new_pos{grid[cart.pos.first, cart.pos.second]};
            if ("/\\"s.contains(new_pos)) {
                cart.direction =
                    corner_direction_map.at(cart.direction).at(new_pos);
            } else if (new_pos == '+') {
                cart.direction =
                    turn_direction_map.at(cart.direction).at(cart.turn);
                cart.turn = turn_map.at(cart.turn);
            }
            for (auto [n, other_cart] : vw::enumerate(carts)) {
                if (m == n or other_cart.crashed) continue;
                if (cart.pos == other_cart.pos) {
                    cart.crashed = true;
                    other_cart.crashed = true;
                    coord = cart.pos;
                    done = true;
                    break;
                }
            }
            if (done) break;
        }
        rg::sort(carts, {}, &Cart::pos);
    }
    return std::format("{},{}", coord.second, coord.first);
}

auto parte_2(vector<string>& lines) {
    vector<Cart> carts;
    Grid<char> grid;
    for (auto [i, line] : vw::enumerate(lines)) {
        for (auto [j, c] : vw::enumerate(line)) {
            switch (c) {
                case '<':
                    grid[i, j] = '-';
                    carts.emplace_back(std::pair<int, int>{i, j}, c, 'l');
                    break;
                case '>':
                    grid[i, j] = '-';
                    carts.emplace_back(std::pair<int, int>{i, j}, c, 'l');
                    break;
                case '^':
                    grid[i, j] = '|';
                    carts.emplace_back(std::pair<int, int>{i, j}, c, 'l');
                    break;
                case 'v':
                    grid[i, j] = '|';
                    carts.emplace_back(std::pair<int, int>{i, j}, c, 'l');
                    break;
                default:
                    grid[i, j] = c;
                    break;
            }
        }
    }
    std::pair<int, int> coord;
    bool done{false};
    int n_crashed{0};
    while (not done) {
        for (auto [m, cart] : vw::enumerate(carts)) {
            if (cart.crashed) continue;
            auto [i, j] = direction_map.at(cart.direction);
            cart.pos.first += i;
            cart.pos.second += j;
            char new_pos{grid[cart.pos.first, cart.pos.second]};
            if ("/\\"s.contains(new_pos)) {
                cart.direction =
                    corner_direction_map.at(cart.direction).at(new_pos);
            } else if (new_pos == '+') {
                cart.direction =
                    turn_direction_map.at(cart.direction).at(cart.turn);
                cart.turn = turn_map.at(cart.turn);
            }
            for (auto [n, other_cart] : vw::enumerate(carts)) {
                if (m == n or other_cart.crashed) continue;
                if (cart.pos == other_cart.pos) {
                    cart.crashed = true;
                    other_cart.crashed = true;
                    n_crashed += 2;
                    break;
                }
            }
        }
        rg::sort(carts, {}, &Cart::pos);
        if (n_crashed == (int)carts.size() - 1) {
            done = true;
            for (Cart& c : carts) {
                if (!c.crashed) coord = c.pos;
            }
        }
    }
    return std::format("{},{}", coord.second, coord.first);
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
