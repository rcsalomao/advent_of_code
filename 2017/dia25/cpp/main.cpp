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

void parte_1(vector<string>& lines) {
    smatch m;
    regex_match(lines[1], m, regex{R"(^.* (\d+) steps.$)"});
    ulong checksum{stoul(m[1].str())};
    unordered_map<string, std::function<void(int&, int&, string&)>>
        state_functions;
    size_t i{3};
    regex state_pat_1{R"(^.*([A-Z]):$)"};
    regex state_pat_2{R"(^.*([A-Z]).$)"};
    regex num_pat_1{R"(^.*([0-9]):$)"};
    regex num_pat_2{R"(^.*([0-9]).$)"};
    regex dir_pat{R"(^.* (\w+).$)"};
    while (i < lines.size()) {
        regex_match(lines[i], m, state_pat_1);
        string state{m[1]};
        regex_match(lines[i + 1], m, num_pat_1);
        int test_1{stoi(m[1].str())};
        regex_match(lines[i + 2], m, num_pat_2);
        int value_1{stoi(m[1].str())};
        regex_match(lines[i + 3], m, dir_pat);
        int shift_1{m[1] == "right" ? 1 : -1};
        regex_match(lines[i + 4], m, state_pat_2);
        string new_state_1{m[1]};
        regex_match(lines[i + 5], m, num_pat_1);
        int test_2{stoi(m[1].str())};
        regex_match(lines[i + 6], m, num_pat_2);
        int value_2{stoi(m[1].str())};
        regex_match(lines[i + 7], m, dir_pat);
        int shift_2{m[1] == "right" ? 1 : -1};
        regex_match(lines[i + 8], m, state_pat_2);
        string new_state_2{m[1]};
        state_functions[state] = [test_1, value_1, shift_1, new_state_1, test_2,
                                  value_2, shift_2, new_state_2](
                                     int& val, int& shift, string& st) {
            if (val == test_1) {
                val = value_1;
                shift = shift_1;
                st = new_state_1;
            } else if (val == test_2) {
                val = value_2;
                shift = shift_2;
                st = new_state_2;
            }
        };
        i += 10;
    }

    constexpr long checksum_max{12399302};
    std::bitset<2 * checksum_max> tape;
    long long idx{checksum_max};
    string state{"A"};
    int shift{0};
    ulong iters{checksum};
    while (iters--) {
        int a{tape[idx]};
        state_functions[state](a, shift, state);
        tape[idx] = a;
        idx += shift;
    }
    println("Resultado da parte 1: {}", tape.count());
}

// void parte_2(vector<string>& lines) { println("Resultado da parte 2: {}",
// ""); }

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
    // parte_2(lines);

    return 0;
}
