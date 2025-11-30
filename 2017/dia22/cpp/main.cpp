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

vector<string> split(const string& str, string_view delim) {
    return vector<string>{std::from_range,
                          str | vw::split(delim) | vw::transform([](auto a) {
                              return string{std::from_range, a};
                          })};
}

vector<string> find_all(string& line, regex& pattern) {
    vector<string> matches;
    // std::for_each(std::sregex_iterator(line.begin(), line.end(), pattern),
    //               std::sregex_iterator(),
    //               [&matches](smatch i) { matches.push_back(i.str()); });
    for (std::sregex_iterator it{line.begin(), line.end(), pattern};
         it != std::sregex_iterator{}; it++) {
        matches.push_back(it->str());
    }
    return matches;
}

template <typename T>
class Grid {
   public:
    Grid() = default;

    T& operator[](string key) { return grid_map[key]; }
    T& operator[](int i, int j) {
        string key = std::vformat("{},{}", std::make_format_args(i, j));
        return grid_map[key];
    }
    bool check_key(string key) { return grid_map.contains(key); }
    bool check_key(int i, int j) {
        string key = std::vformat("{},{}", std::make_format_args(i, j));
        return grid_map.contains(key);
    }
    auto& get_map() { return grid_map; }

   private:
    unordered_map<string, T> grid_map;
};

template <typename T>
void print_grid(Grid<T>& grid, size_t min_i, size_t max_i, size_t min_j,
                size_t max_j) {
    for (size_t i = min_i; i < max_i + 1; i++) {
        for (size_t j = min_j; j < max_j + 1; j++) {
            print("{}", grid[i, j]);
        }
        println();
    }
}

unordered_map<char, tuple<char, char>> dir_change_map{
    {'N', {'E', 'W'}},
    {'E', {'S', 'N'}},
    {'S', {'W', 'E'}},
    {'W', {'N', 'S'}},
};

unordered_map<char, tuple<int, int>> step_map{
    {'N', {-1, 0}},
    {'E', {0, 1}},
    {'S', {1, 0}},
    {'W', {0, -1}},
};

void parte_1(vector<string>& lines) {
    Grid<char> grid;
    long min_i{0};
    long max_i{0};
    long min_j{0};
    long max_j{0};
    for (auto [i, line] : vw::enumerate(lines)) {
        min_i = std::min(i, min_i);
        max_i = std::max(i, max_i);
        for (auto [j, c] : vw::enumerate(line)) {
            min_j = std::min(j, min_j);
            max_j = std::max(j, max_j);
            grid[i, j] = c;
        }
    }

    char carrier_dir{'N'};
    tuple<int, int> carrier_pos{max_i / 2, max_j / 2};

    ulong infection_count{0};
    ulong iters{10000};
    while (iters--) {
        auto [i, j] = carrier_pos;
        if (!grid.check_key(i, j)) grid[i, j] = '.';
        if (grid[i, j] == '#') {
            auto [r, l] = dir_change_map[carrier_dir];
            carrier_dir = r;
            grid[i, j] = '.';
            auto [di, dj] = step_map[carrier_dir];
            carrier_pos = {i + di, j + dj};
        } else {
            auto [r, l] = dir_change_map[carrier_dir];
            carrier_dir = l;
            grid[i, j] = '#';
            auto [di, dj] = step_map[carrier_dir];
            carrier_pos = {i + di, j + dj};
            infection_count++;
        }
    }

    println("Resultado da parte 1: {}", infection_count);
}

unordered_map<char, char> reverse_map{
    {'N', 'S'},
    {'E', 'W'},
    {'S', 'N'},
    {'W', 'E'},
};

void parte_2(vector<string>& lines) {
    Grid<char> grid;
    long min_i{0};
    long max_i{0};
    long min_j{0};
    long max_j{0};
    for (auto [i, line] : vw::enumerate(lines)) {
        min_i = std::min(i, min_i);
        max_i = std::max(i, max_i);
        for (auto [j, c] : vw::enumerate(line)) {
            min_j = std::min(j, min_j);
            max_j = std::max(j, max_j);
            grid[i, j] = c;
        }
    }

    char carrier_dir{'N'};
    tuple<int, int> carrier_pos{max_i / 2, max_j / 2};

    ulong infection_count{0};
    ulong iters{10000000};
    while (iters--) {
        auto [i, j] = carrier_pos;
        if (!grid.check_key(i, j)) grid[i, j] = '.';
        if (grid[i, j] == '.') {
            auto [r, l] = dir_change_map[carrier_dir];
            carrier_dir = l;
            grid[i, j] = 'W';
            auto [di, dj] = step_map[carrier_dir];
            carrier_pos = {i + di, j + dj};
        } else if (grid[i, j] == 'W') {
            grid[i, j] = '#';
            auto [di, dj] = step_map[carrier_dir];
            carrier_pos = {i + di, j + dj};
            infection_count++;
        } else if (grid[i, j] == '#') {
            auto [r, l] = dir_change_map[carrier_dir];
            carrier_dir = r;
            grid[i, j] = 'F';
            auto [di, dj] = step_map[carrier_dir];
            carrier_pos = {i + di, j + dj};
        } else {
            carrier_dir = reverse_map[carrier_dir];
            grid[i, j] = '.';
            auto [di, dj] = step_map[carrier_dir];
            carrier_pos = {i + di, j + dj};
        }
    }

    println("Resultado da parte 2: {}", infection_count);
}

int main(int, char** argv) {
    std::print("\n");

    std::ifstream input;
    input.open(argv[1]);

    string line;
    vector<string> lines;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    input.close();

    parte_1(lines);
    parte_2(lines);

    return 0;
}
