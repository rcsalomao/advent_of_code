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

long calc_high_score(int n_players, int last_marble) {
    deque<int> circle{0};
    unordered_map<int, long> scores;
    int j{0};
    for (int i : vw::iota(1, last_marble + 1)) {
        if (i % 23) {
            circle.push_back(i);
            rg::rotate(circle, circle.begin() + 1);
        } else {
            scores[j] += i;
            std::rotate(circle.rbegin(), circle.rbegin() + 8, circle.rend());
            scores[j] += circle.back();
            circle.pop_back();
            rg::rotate(circle, circle.begin() + 2);
        }
        j++;
        j %= n_players;
    }
    long max_score{
        rg::max_element(scores, {}, [](auto& a) { return a.second; })->second};
    return max_score;
}

long calc_high_score_2(int n_players, int last_marble) {
    auto rotate_l = [](deque<int>& dq, int n) {
        for (int _ : vw::iota(0, n)) {
            int i{dq.front()};
            dq.pop_front();
            dq.push_back(i);
        }
    };
    auto rotate_r = [](deque<int>& dq, int n) {
        for (int _ : vw::iota(0, n)) {
            int i{dq.back()};
            dq.pop_back();
            dq.push_front(i);
        }
    };
    deque<int> circle{0};
    unordered_map<int, long> scores;
    int j{0};
    for (int i : vw::iota(1, last_marble + 1)) {
        if (i % 23) {
            circle.push_back(i);
            rotate_l(circle, 1);
        } else {
            scores[j] += i;
            rotate_r(circle, 8);
            scores[j] += circle.back();
            circle.pop_back();
            rotate_l(circle, 2);
        }
        j++;
        j %= n_players;
    }
    long max_score{
        rg::max_element(scores, {}, [](auto& a) { return a.second; })->second};
    return max_score;
}

auto parte_1(vector<string>& lines) {
    regex pat{R"((\d+) players.* (\d+) points.*)"};
    smatch m;
    long r{-1};
    for (auto line : lines) {
        regex_match(line, m, pat);
        int n_players{stoi(m[1])};
        int last_marble{stoi(m[2])};
        // r = calc_high_score(n_players, last_marble);
        r = calc_high_score_2(n_players, last_marble);
    }
    return r;
}

auto parte_2(vector<string>& lines) {
    regex pat{R"((\d+) players.* (\d+) points.*)"};
    smatch m;
    long r{-1};
    for (auto line : lines) {
        regex_match(line, m, pat);
        int n_players{stoi(m[1])};
        int last_marble{stoi(m[2]) * 100};
        r = calc_high_score_2(n_players, last_marble);
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

    std::print("\n");
    println("Resultado da parte 1: {}", parte_1(lines));
    println("Resultado da parte 2: {}", parte_2(lines));

    return 0;
}
