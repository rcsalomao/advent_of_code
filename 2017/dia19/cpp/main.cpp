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

    void set_value(int i, int j, T value) {
        string key = std::vformat("{};{}", std::make_format_args(i, j));
        grid_map[key] = value;
    }
    void set_value(string key, T value) { grid_map[key] = value; }

    T& get_value(string key) { return grid_map[key]; }
    T& get_value(int i, int j) {
        string key = std::vformat("{};{}", std::make_format_args(i, j));
        return grid_map[key];
    }

    bool check_key(string key) { return grid_map.contains(key); }
    bool check_key(int i, int j) {
        string key = std::vformat("{};{}", std::make_format_args(i, j));
        return grid_map.contains(key);
    }

    auto& get_map() { return grid_map; }

   private:
    unordered_map<string, T> grid_map;
};

void print_maze(long max_i, long max_j, auto& maze) {
    for (long i = 0; i < max_i + 1; i++) {
        for (long j = 0; j < max_j + 1; j++) {
            print("{}", maze.get_value(i, j));
        }
        std::cout << std::endl;
    }
}

string to_string(const tuple<int, int>& node) {
    auto [i, j] = node;
    return std::vformat("{};{}", std::make_format_args(i, j));
}

void parte_1(vector<string>& lines) {
    Grid<char> grid;
    for (auto [i, line] : vw::enumerate(lines)) {
        for (auto [j, c] : vw::enumerate(line)) {
            grid.set_value(i, j, c);
        }
    }

    tuple<int, int> start;
    for (int j : vw::iota(0u, lines[0].size())) {
        if (grid.get_value(0, j) == '|') start = {0, j};
    }
    char direction{'|'};

    tuple<int, int> actual{start};
    deque<string> visited{to_string(start), to_string(start)};
    vector<tuple<char, int, int>> dirs{
        {'-', 0, 1}, {'-', 0, -1}, {'|', 1, 0}, {'|', -1, 0}};
    string alphabet{"ABCDEFGHIJKLMNOPQRSTUVXYWZ"};
    string word;

    bool done{false};
    while (not done) {
        done = true;
        string key{to_string(actual)};
        if (rg::find(visited, key) == visited.end()) {
            visited.push_back(key);
            visited.pop_front();
        }
        char value{grid.get_value(key)};
        if (alphabet.contains(value)) {
            word += value;
        }
        auto [ni, nj] = actual;
        for (auto [dir, di, dj] : dirs) {
            if (ni + di < 0 or nj + dj < 0) continue;
            tuple<int, int> new_n{ni + di, nj + dj};
            string n_key{to_string(new_n)};
            char n_value{grid.get_value(n_key)};
            if (!n_value or n_value == ' ') continue;
            if (rg::find(visited, n_key) != visited.end()) continue;
            if (value != '+' and dir != direction) continue;
            if (value == '+') direction = dir;
            actual = new_n;
            done = false;
        }
    }

    println("Resultado da parte 1: {}", word);
}

void parte_2(vector<string>& lines) {
    Grid<char> grid;
    for (auto [i, line] : vw::enumerate(lines)) {
        for (auto [j, c] : vw::enumerate(line)) {
            grid.set_value(i, j, c);
        }
    }

    tuple<int, int> start;
    for (int j : vw::iota(0u, lines[0].size())) {
        if (grid.get_value(0, j) == '|') start = {0, j};
    }
    char direction{'|'};

    tuple<int, int> actual{start};
    deque<string> visited{to_string(start), to_string(start)};
    vector<tuple<char, int, int>> dirs{
        {'-', 0, 1}, {'-', 0, -1}, {'|', 1, 0}, {'|', -1, 0}};
    string alphabet{"ABCDEFGHIJKLMNOPQRSTUVXYWZ"};
    string word;

    long n_steps{0};
    bool done{false};
    while (not done) {
        n_steps++;
        done = true;
        string key{to_string(actual)};
        if (rg::find(visited, key) == visited.end()) {
            visited.push_back(key);
            visited.pop_front();
        }
        char value{grid.get_value(key)};
        if (alphabet.contains(value)) {
            word += value;
        }
        auto [ni, nj] = actual;
        for (auto [dir, di, dj] : dirs) {
            if (ni + di < 0 or nj + dj < 0) continue;
            tuple<int, int> new_n{ni + di, nj + dj};
            string n_key{to_string(new_n)};
            char n_value{grid.get_value(n_key)};
            if (!n_value or n_value == ' ') continue;
            if (rg::find(visited, n_key) != visited.end()) continue;
            if (value != '+' and dir != direction) continue;
            if (value == '+') direction = dir;
            actual = new_n;
            done = false;
        }
    }

    println("Resultado da parte 2: {}", n_steps);
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
