// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <type_traits>
// #include <nlohmann/json.hpp>
#include <math.h>
#include <openssl/md5.h>

#include <deque>
#include <format>
#include <limits>
#include <map>
#include <numeric>
#include <print>
#include <queue>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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
using std::smatch;
using std::string;
using std::string_view;
using std::to_string;
using std::tuple;
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

std::vector<std::string> find_all(std::string& line, std::regex& pattern) {
    std::vector<std::string> matches;
    // std::for_each(std::sregex_iterator(line.begin(), line.end(), pattern),
    //               std::sregex_iterator(),
    //               [&matches](std::smatch i) { matches.push_back(i.str()); });
    for (std::sregex_iterator it{line.begin(), line.end(), pattern};
         it != std::sregex_iterator{}; it++) {
        matches.push_back(it->str());
    }
    return matches;
}

struct Node {
    string id;
    int weight;
    vector<string> children{};
    string parent{""};
};

// ktlj (57)
// fwft (72) -> ktlj, cntj, xhth

vector<Node> get_node_list(vector<string>& lines) {
    vector<Node> node_list;
    regex pat1{R"((\w+) \((\d+)\))"};
    regex pat2{R"((\w+) \((\d+)\) -> (.*)$)"};
    for (auto line : lines) {
        smatch m;
        if (regex_match(line, m, pat1)) {
            node_list.emplace_back(m[1].str(), stoi(m[2].str()));
        } else if (regex_match(line, m, pat2)) {
            node_list.emplace_back(m[1].str(), stoi(m[2].str()),
                                   split(m[3].str(), ", "));
        }
    }
    for (Node& n : node_list) {
        for (string id : n.children) {
            auto node = rg::find(node_list, id, [](Node& n) { return n.id; });
            node->parent = n.id;
        }
    }
    return node_list;
};

void parte_1(vector<string>& lines) {
    auto node_list = get_node_list(lines);
    auto bottom_node =
        rg::find(node_list, "", [](Node& n) { return n.parent; });
    println("Resultado da parte 1: {}", bottom_node->id);
}

void parte_2(vector<string>& lines) {
    auto node_list = get_node_list(lines);
    unordered_map<string, Node*> node_map;
    for (Node& node : node_list) {
        node_map[node.id] = &node;
    }
    unordered_map<string, int> node_total_weights;
    for (auto& n : node_list) {
        deque<Node*> q;
        unordered_set<string> visited;
        q.push_back(&n);
        visited.insert(n.id);
        node_total_weights[n.id] = n.weight;
        while (not q.empty()) {
            Node* o = q.front();
            q.pop_front();
            for (string s : o->children) {
                node_total_weights[n.id] += node_map[s]->weight;
                if (not visited.contains(s)) {
                    q.push_back(node_map[s]);
                    visited.insert(s);
                }
            }
        }
    }
    tuple<string, vector<int>, vector<string>> problematic_node;
    bool done{false};
    for (Node& n : node_list) {
        if (n.children.size() < 1) continue;
        int first_w{node_total_weights[n.children[0]]};
        for (string s : n.children) {
            if (node_total_weights[s] != first_w) {
                problematic_node = {
                    n.id,
                    {from_range,
                     n.children |
                         vw::transform([&node_total_weights](string s) {
                             return node_total_weights[s];
                         })},
                    n.children};
                break;
            }
        }
        if (done) break;
    }
    // println("{}", problematic_node);
    unordered_map<int, vector<string>> pn_children;
    for (auto [i, s] : vw::zip(std::get<1>(problematic_node),
                               std::get<2>(problematic_node))) {
        pn_children[i].push_back(s);
    }
    int base{0}, odd{0};
    string odd_node;
    for (auto [k, v] : pn_children) {
        if (v.size() == 1) {
            odd = k;
            odd_node = v[0];
        } else {
            base = k;
        }
    }
    int diff{odd - base};

    println("Resultado da parte 2: {}", node_map[odd_node]->weight - diff);
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
