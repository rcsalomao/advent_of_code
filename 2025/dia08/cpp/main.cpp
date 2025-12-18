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

using coord_t = tuple<int32_t, int32_t, int32_t>;
using coord_key_t = bitset<96>;

struct Segment {
    coord_t juncbox_1;
    coord_t juncbox_2;
    int64_t dist;
};

string to_string(coord_t const& c) {
    return format("({}, {}, {})", std::get<0>(c), std::get<1>(c),
                  std::get<2>(c));
}

coord_key_t enc(coord_t const& c) {
    auto [i, j, k] = c;
    coord_key_t r = i;
    r <<= 32;
    r |= j;
    r <<= 32;
    r |= k;
    return r;
};

coord_t dec(coord_key_t const& b) {
    bitset<32> i, j, k;
    for (size_t n = 0; n < 32; n++) {
        i[n] = b[64 + n];
        j[n] = b[32 + n];
        k[n] = b[n];
    }
    return {i.to_ulong(), j.to_ulong(), k.to_ulong()};
};

auto parte_1(vector<string>& lines) {
    vector<coord_t> coords;
    for (string line : lines) {
        auto v = split(line, ",");
        coords.emplace_back(stoi(v[0]), stoi(v[1]), stoi(v[2]));
    }

    auto cmp = [](Segment const& s1, Segment const& s2) {
        return s1.dist > s2.dist;
    };
    priority_queue<Segment, vector<Segment>, decltype(cmp)> pq(cmp);
    for (size_t i : vw::iota(0u, coords.size() - 1)) {
        for (size_t j : vw::iota(i + 1, coords.size())) {
            auto [x, y, z] = coords[i];
            auto [w, u, v] = coords[j];
            int64_t di = x - w;
            int64_t dj = y - u;
            int64_t dk = z - v;
            pq.emplace(coords[i], coords[j], sqrt(di * di + dj * dj + dk * dk));
        }
    }

    unordered_map<uint, unordered_set<coord_key_t>> circuits;
    unordered_map<coord_key_t, uint> coord_key_t_to_circuit_idx;
    int max_circuit_idx{-1};
    uint max_connections{1000};
    // uint max_connections{10};
    while (max_connections--) {
        auto a = pq.top();
        pq.pop();
        auto key1 = enc(a.juncbox_1);
        auto key2 = enc(a.juncbox_2);
        bool key1_connected = coord_key_t_to_circuit_idx.contains(key1);
        bool key2_connected = coord_key_t_to_circuit_idx.contains(key2);
        if (key1_connected and key2_connected) {
            auto key1_idx = coord_key_t_to_circuit_idx[key1];
            auto key2_idx = coord_key_t_to_circuit_idx[key2];
            if (key1_idx == key2_idx)
                continue;
            else {
                circuits[key1_idx].insert_range(circuits[key2_idx]);
                for (auto k : circuits[key2_idx]) {
                    coord_key_t_to_circuit_idx[k] = key1_idx;
                }
                circuits.erase(key2_idx);
            }
        } else if (key1_connected) {
            auto key1_idx = coord_key_t_to_circuit_idx[key1];
            circuits[key1_idx].insert(key2);
            coord_key_t_to_circuit_idx[key2] = key1_idx;
        } else if (key2_connected) {
            auto key2_idx = coord_key_t_to_circuit_idx[key2];
            circuits[key2_idx].insert(key1);
            coord_key_t_to_circuit_idx[key1] = key2_idx;
        } else {
            max_circuit_idx++;
            circuits[max_circuit_idx].insert(key1);
            circuits[max_circuit_idx].insert(key2);
            coord_key_t_to_circuit_idx[key1] = max_circuit_idx;
            coord_key_t_to_circuit_idx[key2] = max_circuit_idx;
        }
    }

    auto b =
        circuits | vw::values |
        vw::transform([](unordered_set<coord_key_t>& a) { return a.size(); }) |
        rg::to<vector<size_t>>();
    sort(b.begin(), b.end(), greater<>{});

    return b[0] * b[1] * b[2];
}

auto parte_2(vector<string>& lines) {
    vector<coord_t> coords;
    for (string line : lines) {
        auto v = split(line, ",");
        coords.emplace_back(stoi(v[0]), stoi(v[1]), stoi(v[2]));
    }

    auto cmp = [](Segment const& s1, Segment const& s2) {
        return s1.dist > s2.dist;
    };
    priority_queue<Segment, vector<Segment>, decltype(cmp)> pq(cmp);
    for (size_t i : vw::iota(0u, coords.size() - 1)) {
        for (size_t j : vw::iota(i + 1, coords.size())) {
            auto [x, y, z] = coords[i];
            auto [w, u, v] = coords[j];
            int64_t di = x - w;
            int64_t dj = y - u;
            int64_t dk = z - v;
            pq.emplace(coords[i], coords[j], sqrt(di * di + dj * dj + dk * dk));
        }
    }

    unordered_map<uint, unordered_set<coord_key_t>> circuits;
    unordered_map<coord_key_t, uint> coord_key_t_to_circuit_idx;
    int max_circuit_idx{-1};
    Segment last_segment;
    bool done{false};
    while (not done) {
        auto a = pq.top();
        pq.pop();
        last_segment = a;
        auto key1 = enc(a.juncbox_1);
        auto key2 = enc(a.juncbox_2);
        bool key1_connected = coord_key_t_to_circuit_idx.contains(key1);
        bool key2_connected = coord_key_t_to_circuit_idx.contains(key2);
        if (key1_connected and key2_connected) {
            auto key1_idx = coord_key_t_to_circuit_idx[key1];
            auto key2_idx = coord_key_t_to_circuit_idx[key2];
            if (key1_idx == key2_idx)
                continue;
            else {
                circuits[key1_idx].insert_range(circuits[key2_idx]);
                for (auto k : circuits[key2_idx]) {
                    coord_key_t_to_circuit_idx[k] = key1_idx;
                }
                circuits.erase(key2_idx);
            }
        } else if (key1_connected) {
            auto key1_idx = coord_key_t_to_circuit_idx[key1];
            circuits[key1_idx].insert(key2);
            coord_key_t_to_circuit_idx[key2] = key1_idx;
        } else if (key2_connected) {
            auto key2_idx = coord_key_t_to_circuit_idx[key2];
            circuits[key2_idx].insert(key1);
            coord_key_t_to_circuit_idx[key1] = key2_idx;
        } else {
            max_circuit_idx++;
            circuits[max_circuit_idx].insert(key1);
            circuits[max_circuit_idx].insert(key2);
            coord_key_t_to_circuit_idx[key1] = max_circuit_idx;
            coord_key_t_to_circuit_idx[key2] = max_circuit_idx;
        }
        if (coord_key_t_to_circuit_idx.size() == coords.size()) done = true;
    }

    int64_t x1{get<0>(last_segment.juncbox_1)};
    int64_t x2{get<0>(last_segment.juncbox_2)};

    return x1 * x2;
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
