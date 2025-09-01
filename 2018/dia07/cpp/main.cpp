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
    regex pat{R"(Step ([A-Z]) .* step ([A-Z]) .*)"};
    smatch m;
    map<string, set<string>> inv_adj_list;
    for (auto line : lines) {
        regex_match(line, m, pat);
        string u{m[1]};
        string v{m[2]};
        if (!inv_adj_list.contains(v)) inv_adj_list[v] = set<string>{};
        if (!inv_adj_list.contains(u)) inv_adj_list[u] = set<string>{};
        inv_adj_list[v].insert(u);
    }

    std::stringstream stst;
    while (!inv_adj_list.empty()) {
        string to_remove;
        for (auto& [k, v] : inv_adj_list) {
            if (v.empty()) {
                to_remove = k;
                break;
            }
        }
        for (auto& [k, v] : inv_adj_list) {
            v.erase(to_remove);
        }
        inv_adj_list.erase(to_remove);
        stst << to_remove;
    }

    return stst.str();
}

struct Worker {
    char to_remove{' '};
    long time{-1};
    bool working{false};
};

auto parte_2(vector<string>& lines) {
    regex pat{R"(Step ([A-Z]) .* step ([A-Z]) .*)"};
    smatch m;
    map<string, set<string>> inv_adj_list;
    for (auto line : lines) {
        regex_match(line, m, pat);
        string u{m[1]};
        string v{m[2]};
        if (!inv_adj_list.contains(v)) inv_adj_list[v] = set<string>{};
        if (!inv_adj_list.contains(u)) inv_adj_list[u] = set<string>{};
        inv_adj_list[v].insert(u);
    }

    // constexpr int n_workers{2};
    constexpr int n_workers{5};
    std::array<Worker, n_workers> workers;
    auto is_scheduled_to_remove = [&workers](string c) {
        for (auto& w : workers) {
            if (c == string{w.to_remove}) return true;
        }
        return false;
    };

    std::stringstream stst;
    long time{0};
    while (!inv_adj_list.empty()) {
        for (auto& w : workers) {
            if (not w.working) {
                for (auto& [k, v] : inv_adj_list) {
                    if (v.empty() and not is_scheduled_to_remove(k)) {
                        w.to_remove = *k.c_str();
                        // w.time = time + w.to_remove - 'A';
                        w.time = 60 + time + w.to_remove - 'A';
                        w.working = true;
                        break;
                    }
                }
            }
        }
        for (auto& w : workers) {
            if (time == w.time) {
                for (auto& [k, v] : inv_adj_list) {
                    v.erase(string{w.to_remove});
                }
                inv_adj_list.erase(string{w.to_remove});
                stst << w.to_remove;
                w.to_remove = ' ';
                w.time = -1;
                w.working = false;
            }
        }
        time++;
    }
    return time;
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
