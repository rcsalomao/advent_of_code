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
using coord_t = tuple<int32_t, int32_t>;

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

coord_t dec(grid_key_t key) {
    bitset<32> j;
    for (size_t n = 0; n < 32; n++) {
        j[n] = key[32 + n];
    }
    return {key.to_ulong(), j.to_ulong()};
}

grid_key_t enc(grid_key_t i, bitset<32> j) {
    for (size_t n = 0; n < 32; n++) {
        i[32 + n] = j[n];
    }
    return i;
}

grid_key_t enc(coord_t const& coord) {
    grid_key_t k = get<1>(coord);
    k <<= 32;
    k |= get<0>(coord);
    return k;
}

string to_string(coord_t const& c) {
    return format("({}, {})", std::get<0>(c), std::get<1>(c));
}

uint64_t calc_area(coord_t const& c1, coord_t const& c2) {
    auto [i, j] = c1;
    auto [x, y] = c2;
    return static_cast<uint64_t>(abs(i - x) + 1) *
           static_cast<uint64_t>(abs(j - y) + 1);
}

auto parte_1(vector<string>& lines) {
    vector<coord_t> coords;
    for (string& line : lines) {
        auto v = split(line, ",");
        coords.emplace_back(stoi(v[0]), stoi(v[1]));
    }
    coords.emplace_back(get<0>(coords[0]), get<1>(coords[0]));

    uint64_t a{0};
    for (size_t i{0}; i < coords.size() - 1; i++) {
        for (size_t j{i + 1}; j < coords.size(); j++) {
            a = max(a, calc_area(coords[i], coords[j]));
        }
    }
    return a;
}

bool check_coord_inside(coord_t& coord, pair<coord_t, coord_t>& coords_pair) {
    auto [x, y] = coords_pair.first;
    auto [m, n] = coords_pair.second;
    auto i = min(x, m);
    auto j = min(y, n);
    auto p = max(x, m);
    auto q = max(y, n);
    auto [a, b] = coord;
    if (i < a and a < p and j < b and b < q) {
        return true;
    } else {
        return false;
    }
}

bool check_edge_cross(pair<coord_t, coord_t> const& edge,
                      pair<coord_t, coord_t>& coords_pair) {
    auto [px, py] = coords_pair.first;
    auto [pm, pn] = coords_pair.second;
    auto pi = min(px, pm);
    auto pj = min(py, pn);
    auto pp = max(px, pm);
    auto pq = max(py, pn);
    auto [ex, ey] = edge.first;
    auto [em, en] = edge.second;
    bool horiz = en - ey == 0 ? true : false;
    if (horiz) {
        if (pj < ey and ey < pq) {
            if (min(ex, em) <= pi and pp <= max(ex, em)) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        if (pi < ex and ex < pp) {
            if (min(ey, en) <= pj and pq <= max(ey, en)) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
}

auto parte_2(vector<string>& lines) {
    vector<coord_t> coords;
    for (string& line : lines) {
        auto v = split(line, ",");
        coords.emplace_back(stoi(v[0]), stoi(v[1]));
    }
    coords.emplace_back(get<0>(coords[0]), get<1>(coords[0]));

    vector<pair<uint64_t, pair<coord_t, coord_t>>> areas;
    size_t n_coords = coords.size();
    for (size_t i{0}; i < n_coords - 1; i++) {
        for (size_t j{i + 1}; j < n_coords; j++) {
            areas.emplace_back(calc_area(coords[i], coords[j]),
                               pair{coords[i], coords[j]});
        }
    }

    rg::sort(areas, greater<>{}, [](auto& a) { return a.first; });

    uint64_t r{0};
    for (auto& [area, area_coords] : areas) {
        bool valid{true};
        for (auto& coord : coords) {
            if (check_coord_inside(coord, area_coords)) {
                valid = false;
                break;
            }
        }
        if (not valid) continue;
        for (size_t i{0}; i < n_coords - 1; i++) {
            coord_t c1 = coords[i];
            coord_t c2 = coords[i + 1];
            if (check_edge_cross({c1, c2}, area_coords)) {
                valid = false;
                break;
            }
        }
        if (valid) {
            r = area;
            break;
        }
    }
    return r;
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
