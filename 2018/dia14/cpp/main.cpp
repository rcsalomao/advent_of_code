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

auto parte_1(vector<string>& lines) {
    vector<char> recipes{'3', '7'};
    size_t idx_1{0};
    size_t idx_2{1};
    ulong after{std::stoul(lines[0])};
    bool done{false};
    while (not done) {
        int idx_1_num{recipes[idx_1] - '0'};
        int idx_2_num{recipes[idx_2] - '0'};
        int num{idx_1_num + idx_2_num};
        for (char c : to_string(num)) {
            recipes.push_back(c);
        }
        idx_1 = (idx_1 + idx_1_num + 1) % recipes.size();
        idx_2 = (idx_2 + idx_2_num + 1) % recipes.size();
        if (recipes.size() >= after + 10) done = true;
    }
    string r{recipes.begin() + after, recipes.begin() + after + 10};
    return r;
}

auto parte_2(vector<string>& lines) {
    vector<int8_t> recipes{3, 7};
    size_t idx_1{0};
    size_t idx_2{1};
    vector<int8_t> digits{
        from_range, lines[0] | vw::transform([](char c) { return c - '0'; })};
    size_t sz{digits.size()};
    bool done{false};
    int to_subtract{0};
    while (not done) {
        int8_t idx_1_num{recipes[idx_1]};
        int8_t idx_2_num{recipes[idx_2]};
        int8_t num = idx_1_num + idx_2_num;
        if (num < 10) {
            recipes.push_back(num);
        } else {
            recipes.push_back(num / 10);
            recipes.push_back(num % 10);
        }
        size_t rsz{recipes.size()};
        idx_1 = (idx_1 + idx_1_num + 1) % rsz;
        idx_2 = (idx_2 + idx_2_num + 1) % rsz;
        if (rsz < sz + 1) continue;
        if (std::equal(digits.begin(), digits.end(), recipes.end() - sz))
            done = true;
        if (std::equal(digits.begin(), digits.end(), recipes.end() - 1 - sz)) {
            done = true;
            to_subtract = 1;
        }
    }
    return recipes.size() - sz - to_subtract;
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
