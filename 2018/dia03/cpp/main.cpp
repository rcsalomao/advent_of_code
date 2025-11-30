// #include <Eigen/Dense>
// #include <nlohmann/json.hpp>
// #include <openssl/md5.h>
#include <bits/stdc++.h>

#include <boost/functional/hash.hpp>

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

auto parte_1(vector<string>& lines) {
    unordered_map<long long, long> coords_count;
    regex pat{R"(.* ([0-9]+),([0-9]+): ([0-9]+)x([0-9]+)$)"};
    smatch m;
    for (auto line : lines) {
        regex_match(line, m, pat);
        long j0{stol(m[1].str())};
        long i0{stol(m[2].str())};
        long dj{stol(m[3].str())};
        long di{stol(m[4].str())};
        for (long i = 0; i < di; i++) {
            for (long j = 0; j < dj; j++) {
                long long coord{((i0 + i) << 32) | (j0 + j)};
                coords_count[coord]++;
            }
        }
    }

    ulong count{0};
    for (auto [k, v] : coords_count) {
        if (v > 1) count++;
    }

    // long b{30};
    // long c{24};
    // long long a{(b << 32) | c};
    // println("{}", a);
    // long d{a & 0xffff};
    // long e{a >> 32};
    // println("{}, {}", d, e);

    return count;
}

auto parte_2(vector<string>& lines) {
    unordered_map<long long, vector<int>> ids_per_coord;
    regex pat{R"(#([0-9]+) @ ([0-9]+),([0-9]+): ([0-9]+)x([0-9]+)$)"};
    smatch m;
    for (auto line : lines) {
        regex_match(line, m, pat);
        int id{stoi(m[1].str())};
        long j0{stol(m[2].str())};
        long i0{stol(m[3].str())};
        long dj{stol(m[4].str())};
        long di{stol(m[5].str())};
        for (long i = 0; i < di; i++) {
            for (long j = 0; j < dj; j++) {
                long long coord{((i0 + i) << 32) | (j0 + j)};
                ids_per_coord[coord].push_back(id);
            }
        }
    }

    unordered_set<int> conflicting_ids;
    for (auto [k, v] : ids_per_coord) {
        if (v.size() > 1) {
            for (auto i : v) {
                conflicting_ids.insert(i);
            }
        }
    }
    for (auto i : vw::iota(1u, lines.size() + 1)) {
        if (rg::find(conflicting_ids, i) != conflicting_ids.end()) {
            continue;
        } else {
            return static_cast<int>(i);
        }
    }
    return -1;
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
