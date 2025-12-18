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

using grid_key_t = bitset<64>;

template <typename T>
class Grid {
   public:
    Grid() = default;

    T& at(grid_key_t const key) { return grid_map.at(key); }
    T& at(grid_key_t i, bitset<32> j) {
        for (size_t n = 0; n < 32; n++) {
            i[32 + n] = j[n];
        }
        return grid_map.at(i);
    }
    T& operator[](grid_key_t const key) { return grid_map[key]; }
    bool contains(grid_key_t const key) { return grid_map.contains(key); }
    T& operator[](grid_key_t i, bitset<32> j) {
        for (size_t n = 0; n < 32; n++) {
            i[32 + n] = j[n];
        }
        return grid_map[i];
    }
    bool contains(grid_key_t i, bitset<32> j) {
        for (size_t n = 0; n < 32; n++) {
            i[32 + n] = j[n];
        }
        return grid_map.contains(i);
    }
    auto& get_map() { return grid_map; }

   private:
    unordered_map<grid_key_t, T> grid_map;
};

tuple<int32_t, int32_t> dec_key(grid_key_t key) {
    bitset<32> j;
    for (size_t n = 0; n < 32; n++) {
        j[n] = key[32 + n];
    }
    return {key.to_ulong(), j.to_ulong()};
}

grid_key_t enc_key(grid_key_t i, bitset<32> j) {
    for (size_t n = 0; n < 32; n++) {
        i[32 + n] = j[n];
    }
    return i;
}

auto parte_1(vector<string>& lines) {
    Grid<char> grid;
    pair<int, int> start;
    long max_i{0};
    long max_j{0};
    for (auto [i, line] : vw::enumerate(lines)) {
        for (auto [j, c] : vw::enumerate(line)) {
            grid[i, j] = c;
            if (c == 'S') start = {i, j};
            max_j = max(j, max_j);
        }
        max_i = max(i, max_i);
    }

    // BFS
    long total{0};
    deque<pair<int, int>> q;
    q.emplace_front(start.first + 1, start.second);
    while (not q.empty()) {
        auto [i, j] = q.back();
        q.pop_back();
        if (i > max_i) continue;
        if (grid[i, j] == '.') {
            grid[i, j] = '|';
            q.emplace_front(i + 1, j);
        }
        if (grid[i, j] == '^') {
            total++;
            q.emplace_front(i, j - 1);
            q.emplace_front(i, j + 1);
        }
    }

    return total;
}

auto parte_2(vector<string>& lines) {
    Grid<char> grid;
    pair<int, int> start;
    long max_i{0};
    long max_j{0};
    for (auto [i, line] : vw::enumerate(lines)) {
        for (auto [j, c] : vw::enumerate(line)) {
            grid[i, j] = c;
            if (c == 'S') start = {i, j};
            max_j = max(j, max_j);
        }
        max_i = max(i, max_i);
    }

    ulong total{0};

    // create graph/adjacency list
    unordered_map<grid_key_t, vector<grid_key_t>> graph;
    unordered_map<grid_key_t, int> indegree;

    deque<pair<pair<int, int>, pair<int, int>>> dq;
    dq.push_front(
        {{start.first, start.second}, {start.first + 1, start.second}});
    graph[enc_key(start.first, start.second)] = {};
    indegree[enc_key(start.first, start.second)] = 0;
    while (not dq.empty()) {
        auto [u, v] = dq.back();
        dq.pop_back();
        auto [pi, pj] = u;
        auto [i, j] = v;
        if (i > max_i) continue;
        if (grid[i, j] == '.') {
            grid[i, j] = '|';
            graph[enc_key(pi, pj)].push_back(enc_key(i, j));
            indegree[enc_key(i, j)]++;
            dq.push_front({{i, j}, {i + 1, j}});
            graph[enc_key(i, j)] = {};
        } else if (grid[i, j] == '|') {
            graph[enc_key(pi, pj)].push_back(enc_key(i, j));
            indegree[enc_key(i, j)]++;
        } else if (grid[i, j] == '^') {
            graph[enc_key(pi, pj)].push_back(enc_key(i, j));
            indegree[enc_key(i, j)]++;
            dq.push_front({{i, j}, {i, j - 1}});
            dq.push_front({{i, j}, {i, j + 1}});
            graph[enc_key(i, j)] = {};
        }
    }

    // topological sort: Kahn's algorithm
    queue<grid_key_t> q;
    for (auto [bts, ind] : indegree) {
        if (ind == 0) {
            q.push(bts);
        }
    }
    vector<grid_key_t> topo_order;
    while (not q.empty()) {
        auto key = q.front();
        q.pop();
        topo_order.push_back(key);
        for (auto neighbor : graph[key]) {
            indegree[neighbor]--;
            if (indegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }

    // calc number of possible paths to each node
    unordered_map<grid_key_t, ulong> n_paths;
    n_paths[enc_key(start.first, start.second)] = 1;
    for (auto key : topo_order) {
        for (auto neighbor : graph[key]) {
            n_paths[neighbor] += n_paths[key];
        }
    }

    for (auto [k, v] : n_paths) {
        auto [i, j] = dec_key(k);
        if (i == max_i) total += v;
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
