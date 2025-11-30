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
using std::pair;
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

bitset<64> encode_key(bitset<64> i, bitset<32> j) {
    for (size_t n = 0; n < 32; n++) {
        i[32 + n] = j[n];
    }
    return i;
}

struct Entity {
    char type;
    pair<int, int> pos;
    int health{200};
    bool dead{false};
};

Grid<char> get_world_map(Grid<char>& terrain, vector<Entity>& entities) {
    Grid<char> map{terrain};
    for (Entity& e : entities) {
        if (not e.dead) map[e.pos.first, e.pos.second] = e.type;
    }
    return map;
}

Grid<Entity*> get_coord_entity_map(vector<Entity>& entities) {
    Grid<Entity*> map;
    for (Entity& e : entities) {
        if (not e.dead) map[e.pos.first, e.pos.second] = &e;
    }
    return map;
}

void print_map(Grid<char>& world_map, int32_t min_i, int32_t max_i,
               int32_t min_j, int32_t max_j) {
    for (int32_t i = min_i; i < max_i + 1; i++) {
        for (int32_t j = min_j; j < max_j + 1; j++) {
            print("{:1}", world_map[i, j]);
        }
        println();
    }
}

const set<pair<int, int>> direction_set{{-1, 0}, {0, -1}, {0, 1}, {1, 0}};
const unordered_map<char, char> enemy_map{{'G', 'E'}, {'E', 'G'}};

vector<pair<int, int>> get_range_tiles(Entity& en, Grid<char>& terrain) {
    vector<pair<int, int>> tiles;
    for (auto [m, n] : direction_set) {
        int p{en.pos.first + m};
        int q{en.pos.second + n};
        if (terrain[p, q] != '#') tiles.emplace_back(p, q);
    }
    return tiles;
}

vector<pair<int, int>> get_move_tiles(int i, int j, Grid<char>& world_map) {
    vector<pair<int, int>> tiles;
    for (auto [m, n] : direction_set) {
        int p{i + m};
        int q{j + n};
        if (world_map[p, q] == '.') tiles.emplace_back(p, q);
    }
    return tiles;
}

vector<pair<int, int>> calc_shortest_path(Grid<char>& world_map,
                                          pair<int, int> source,
                                          pair<int, int> dest) {
    deque<pair<int, int>> q{source};
    unordered_set<bitset<64>> visited{encode_key(source.first, source.second)};
    unordered_map<bitset<64>, pair<int, int>> inv_adj_list;
    bool reached_dest{false};
    while (not q.empty()) {
        if (q.front() == dest) {
            reached_dest = true;
            break;
        }
        auto [i, j] = q.front();
        q.pop_front();
        for (auto [o, p] : get_move_tiles(i, j, world_map)) {
            bitset<64> key{encode_key(o, p)};
            if (visited.contains(key)) continue;
            inv_adj_list[key] = {i, j};
            visited.insert(key);
            q.emplace_back(o, p);
        }
    }
    if (not reached_dest) return {};
    vector<pair<int, int>> v{q.front()};
    while (v.back() != source) {
        v.push_back(inv_adj_list[encode_key(v.back().first, v.back().second)]);
    }
    rg::reverse(v);
    return v;
}

std::optional<pair<int, int>> calc_move_coord(Grid<char>& world_map,
                                              vector<Entity>& entities,
                                              Entity& en) {
    vector<vector<pair<int, int>>> paths;
    for (Entity& e2 : entities) {
        if (e2.dead) continue;
        if (e2.type != enemy_map.at(en.type)) continue;
        for (auto [dm, dn] :
             get_move_tiles(e2.pos.first, e2.pos.second, world_map)) {
            vector<pair<int, int>> path =
                calc_shortest_path(world_map, en.pos, {dm, dn});
            paths.push_back(std::move(path));
        }
    }
    std::optional<pair<int, int>> ending_coord;
    if (!paths.size()) return ending_coord;
    size_t min_size{std::numeric_limits<size_t>::max()};
    for (auto& p : paths) {
        if (!p.size()) continue;
        size_t sz{p.size()};
        if (sz < min_size) {
            min_size = sz;
            ending_coord = p.back();
        } else if (sz == min_size) {
            ending_coord = std::min(ending_coord.value(), p.back());
        }
    }
    if (!ending_coord) return ending_coord;
    paths.clear();
    for (auto [di, dj] :
         get_move_tiles(en.pos.first, en.pos.second, world_map)) {
        vector<pair<int, int>> path =
            calc_shortest_path(world_map, {di, dj}, ending_coord.value());
        paths.push_back(std::move(path));
    }
    std::optional<pair<int, int>> coord;
    min_size = std::numeric_limits<size_t>::max();
    for (auto& p : paths) {
        if (!p.size()) continue;
        size_t sz{p.size()};
        if (sz < min_size) {
            min_size = sz;
            coord = p[0];
        } else if (sz == min_size) {
            coord = std::min(coord.value(), p[0]);
        }
    }
    return coord;
}

bool check_enemy_in_range(Grid<char>& terrain, Entity& en,
                          Grid<Entity*>& coord_entity_map, char enemy_type) {
    vector<pair<int, int>> neighbours = get_range_tiles(en, terrain);
    bool enemy_in_range =
        std::any_of(neighbours.begin(), neighbours.end(),
                    [&coord_entity_map, enemy_type](pair<int, int>& p) {
                        if (coord_entity_map.check_key(p.first, p.second)) {
                            Entity* e{coord_entity_map[p.first, p.second]};
                            return not e->dead and e->type == enemy_type;
                        } else {
                            return false;
                        }
                    });
    return enemy_in_range;
}

std::optional<pair<int, vector<Entity>>> run_simulation(Grid<char>& terrain,
                                                        vector<Entity> entities,
                                                        int elf_attack_power,
                                                        bool part_2 = false) {
    int n_goblins{0};
    int n_elves{0};
    for (auto& e : entities) {
        e.type == 'G' ? n_goblins++ : n_elves++;
    }
    Grid<char> world_map{get_world_map(terrain, entities)};
    Grid<Entity*> coord_entity_map{get_coord_entity_map(entities)};
    // print_map(world_map, 0, max_i, 0, max_j);
    int total_iters{0};
    int max_inters{99};
    while (n_goblins and n_elves and ++total_iters) {
        rg::sort(entities, {}, &Entity::pos);
        // world_map = get_world_map(terrain, entities);
        coord_entity_map = get_coord_entity_map(entities);
        for (Entity& en : entities) {
            if (en.dead) continue;
            // move phase
            bool enemy_in_range = check_enemy_in_range(
                terrain, en, coord_entity_map, enemy_map.at(en.type));
            bool did_move{false};
            if (not enemy_in_range) {
                std::optional<pair<int, int>> coord =
                    calc_move_coord(world_map, entities, en);
                if (coord) {
                    coord_entity_map[en.pos.first, en.pos.second]->pos =
                        coord.value();
                    world_map = get_world_map(terrain, entities);
                    coord_entity_map = get_coord_entity_map(entities);
                    did_move = true;
                }
            }
            // attack phase
            if (did_move) {
                enemy_in_range = check_enemy_in_range(
                    terrain, en, coord_entity_map, enemy_map.at(en.type));
            }
            if (enemy_in_range) {
                int min_health{std::numeric_limits<int>::max()};
                Entity* enemy{nullptr};
                for (auto [m, n] : get_range_tiles(en, terrain)) {
                    if (world_map[m, n] != enemy_map.at(en.type)) continue;
                    Entity* en{coord_entity_map[m, n]};
                    if (en->dead) continue;
                    int health{en->health};
                    if (health < min_health) {
                        min_health = health;
                        enemy = en;
                    } else if (health == min_health) {
                        if (en->pos < enemy->pos) enemy = en;
                    }
                }
                if (enemy->type == 'G') {
                    enemy->health -= elf_attack_power;
                } else {
                    enemy->health -= 3;
                }
                if (enemy->health <= 0) {
                    if (part_2) {
                        if (enemy->type == 'G') {
                            n_goblins--;
                        } else {
                            return {};
                        }
                    } else {
                        enemy->type == 'G' ? n_goblins-- : n_elves--;
                    }
                    enemy->dead = true;
                    world_map = get_world_map(terrain, entities);
                    // coord_entity_map = get_coord_entity_map(entities);
                }
            }
        }
        if (total_iters >= max_inters) {
            break;
        }
    }
    return std::optional{pair{total_iters, entities}};
}

auto parte_1(vector<string>& lines) {
    Grid<char> terrain;
    vector<Entity> entities;
    long max_i{0};
    long max_j{0};
    for (auto [i, line] : vw::enumerate(lines)) {
        max_i = std::max(i, max_i);
        for (auto [j, c] : vw::enumerate(line)) {
            max_j = std::max(j, max_j);
            if ("GE"s.contains(c)) {
                entities.emplace_back(c, pair{i, j});
                terrain[i, j] = '.';
                continue;
            }
            terrain[i, j] = c;
        }
    }
    int total_iters;
    vector<Entity> r_entities;
    bool done{false};
    while (not done) {
        auto res = run_simulation(terrain, entities, 3);
        if (res) {
            done = true;
            total_iters = res->first;
            r_entities = res->second;
        }
    }
    std::erase_if(r_entities, [](Entity& e) { return e.dead; });
    int sum{0};
    for (auto& e : r_entities) {
        sum += e.health;
    }
    return sum * (total_iters - 1);
}

auto parte_2(vector<string>& lines) {
    Grid<char> terrain;
    vector<Entity> entities;
    long max_i{0};
    long max_j{0};
    for (auto [i, line] : vw::enumerate(lines)) {
        max_i = std::max(i, max_i);
        for (auto [j, c] : vw::enumerate(line)) {
            max_j = std::max(j, max_j);
            if ("GE"s.contains(c)) {
                entities.emplace_back(c, pair{i, j});
                terrain[i, j] = '.';
                continue;
            }
            terrain[i, j] = c;
        }
    }
    int total_iters;
    vector<Entity> r_entities;
    bool done{false};
    int elf_attk_pow{33};
    while (not done) {
        auto res = run_simulation(terrain, entities, ++elf_attk_pow, true);
        if (res) {
            done = true;
            total_iters = res->first;
            r_entities = res->second;
        }
    }
    std::erase_if(r_entities, [](Entity& e) { return e.dead; });
    int sum{0};
    for (auto& e : r_entities) {
        sum += e.health;
    }
    return sum * (total_iters - 1);
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
