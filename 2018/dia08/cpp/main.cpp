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

struct Node {
    vector<int> metadata;
    vector<unique_ptr<Node>> children;
};

unique_ptr<Node> parse_nodes(deque<int>& vi) {
    int n_ch{vi.front()};
    vi.pop_front();
    int n_md{vi.front()};
    vi.pop_front();

    vector<unique_ptr<Node>> children;
    for (int i = 0; i < n_ch; i++) {
        children.push_back(parse_nodes(vi));
    }

    vector<int> metadata;
    for (int i = 0; i < n_md; i++) {
        metadata.push_back(vi.front());
        vi.pop_front();
    }

    return std::make_unique<Node>(metadata, std::move(children));
}

auto parte_1(vector<string>& lines) {
    deque<int> input =
        deque{from_range, split(lines[0], " ") |
                              vw::transform([](auto& s) { return stoi(s); })};
    auto root = parse_nodes(input);
    deque<Node*> q{root.get()};
    long total{0};
    while (!q.empty()) {
        Node* n{q.front()};
        q.pop_front();
        total += std::reduce(n->metadata.begin(), n->metadata.end(), 0);
        for (auto& c : n->children) {
            q.push_back(c.get());
        }
    }
    return total;
}

int total(unique_ptr<Node>& n) {
    if (n->children.empty())
        return std::reduce(n->metadata.begin(), n->metadata.end(), 0);
    int t{0};
    for (int i : n->metadata) {
        int j{i - 1};
        if (j < 0) continue;
        if ((size_t)j >= n->children.size()) continue;
        t += total(n->children[j]);
    }
    return t;
}

auto parte_2(vector<string>& lines) {
    deque<int> input =
        deque{from_range, split(lines[0], " ") |
                              vw::transform([](auto& s) { return stoi(s); })};
    auto root = parse_nodes(input);
    return total(root);
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
