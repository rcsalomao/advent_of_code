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
            print("{}", grid[i, j]);
        }
        println();
    }
}

struct Particle {
    tuple<int, int> position;
    tuple<int, int> velocity;
};

struct BoundingBox {
    int min_x;
    int min_y;
    int max_x;
    int max_y;
};

BoundingBox get_bounding_box(unordered_map<int, Particle>& p) {
    int min_x{std::numeric_limits<int>::max()};
    int max_x{std::numeric_limits<int>::min()};
    int min_y{std::numeric_limits<int>::max()};
    int max_y{std::numeric_limits<int>::min()};
    for (auto& [k, v] : p) {
        auto [x, y] = v.position;
        min_x = std::min(min_x, x);
        max_x = std::max(max_x, x);
        min_y = std::min(min_y, y);
        max_y = std::max(max_y, y);
    }
    return {min_x, min_y, max_x, max_y};
}

void print_particles(unordered_map<int, Particle>& p, BoundingBox& bb) {
    Grid<string> grid;
    for (auto& [idx, particle] : p) {
        auto [j, i] = particle.position;
        grid[i, j] = "#";
    }

    for (int32_t i = bb.min_y; i < bb.max_y + 1; i++) {
        for (int32_t j = bb.min_x; j < bb.max_x + 1; j++) {
            print("{}", grid.check_key(i, j) ? grid[i, j] : " ");
        }
        println();
    }
}

ulong calc_bounding_box_area(unordered_map<int, Particle>& p) {
    BoundingBox bb{get_bounding_box(p)};
    return abs(bb.max_x - bb.min_x) * abs(bb.max_y - bb.min_y);
}

auto parte_1(vector<string>& lines) {
    smatch m;
    regex pat{
        R"(position=<\s*(-?[0-9]*),\s*(-?[0-9]*)> velocity=<\s*(-?[0-9]*),\s*(-?[0-9]*)>)"};
    unordered_map<int, Particle> particles;
    unordered_map<int, Particle> particles0;
    int idx{0};
    for (auto line : lines) {
        regex_match(line, m, pat);
        int p1{stoi(m[1])};
        int p2{stoi(m[2])};
        int v1{stoi(m[3])};
        int v2{stoi(m[4])};
        particles[idx] = Particle{{p1, p2}, {v1, v2}};
        particles0[idx] = Particle{{p1, p2}, {v1, v2}};
        idx++;
    }

    int time{0};
    int msg_time{-1};
    ulong min_area{std::numeric_limits<ulong>::max()};
    while (time < 15000) {
        for (auto& [idx, particle] : particles) {
            auto& [p1, p2] = particle.position;
            auto [p01, p02] = particles0[idx].position;
            auto [v01, v02] = particles0[idx].velocity;
            p1 = p01 + time * v01;
            p2 = p02 + time * v02;
        }
        ulong area = calc_bounding_box_area(particles);
        if (area < min_area) {
            min_area = area;
            msg_time = time;
        }
        time++;
    }

    for (auto& [idx, particle] : particles) {
        auto& [p1, p2] = particle.position;
        auto [p01, p02] = particles0[idx].position;
        auto [v01, v02] = particles0[idx].velocity;
        p1 = p01 + msg_time * v01;
        p2 = p02 + msg_time * v02;
    }
    BoundingBox bb{get_bounding_box(particles)};
    print_particles(particles, bb);

    return "";
}

auto parte_2(vector<string>& lines) {
    smatch m;
    regex pat{
        R"(position=<\s*(-?[0-9]*),\s*(-?[0-9]*)> velocity=<\s*(-?[0-9]*),\s*(-?[0-9]*)>)"};
    unordered_map<int, Particle> particles;
    unordered_map<int, Particle> particles0;
    int idx{0};
    for (auto line : lines) {
        regex_match(line, m, pat);
        int p1{stoi(m[1])};
        int p2{stoi(m[2])};
        int v1{stoi(m[3])};
        int v2{stoi(m[4])};
        particles[idx] = Particle{{p1, p2}, {v1, v2}};
        particles0[idx] = Particle{{p1, p2}, {v1, v2}};
        idx++;
    }

    int time{0};
    int msg_time{-1};
    ulong min_area{std::numeric_limits<ulong>::max()};
    while (time < 15000) {
        for (auto& [idx, particle] : particles) {
            auto& [p1, p2] = particle.position;
            auto [p01, p02] = particles0[idx].position;
            auto [v01, v02] = particles0[idx].velocity;
            p1 = p01 + time * v01;
            p2 = p02 + time * v02;
        }
        ulong area = calc_bounding_box_area(particles);
        if (area < min_area) {
            min_area = area;
            msg_time = time;
        }
        time++;
    }

    return msg_time;
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
