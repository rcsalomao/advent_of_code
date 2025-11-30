// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <type_traits>
// #include <nlohmann/json.hpp>
#include <openssl/md5.h>

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
using std::from_range;
using std::map;
using std::print;
using std::println;
using std::regex;
using std::regex_match;
using std::regex_search;
using std::smatch;
using std::string;
using std::string_view;
using std::to_string;
using std::tuple;
using std::unordered_map;
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

string openssl_md5(const string& str) {
    unsigned char hash[MD5_DIGEST_LENGTH];

    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, str.c_str(), str.size());
    MD5_Final(hash, &md5);

    std::stringstream ss;

    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(hash[i]);
    }
    return ss.str();
};

static const vector<tuple<int, int, string>> dirs{
    {0, -1, "U"}, {0, 1, "D"}, {-1, 0, "L"}, {1, 0, "R"}};

string coord_to_str(int x, int y) {
    return {to_string(x) + "," + to_string(y)};
}

struct State {
    string passcode;
    tuple<int, int> position{0, 0};
};

void parte_1(std::vector<std::string>& lines) {
    State init{lines[0]};
    size_t init_passcode_size{lines[0].size()};
    tuple<int, int> end{3, 3};

    std::queue<State> q;
    q.push(init);
    string res{" :c"};
    while (not q.empty()) {
        State stt = q.front();
        if (stt.position == end) {
            res = stt.passcode.substr(init_passcode_size,
                                      stt.passcode.size() - init_passcode_size);
            break;
        }
        q.pop();
        string hash = openssl_md5(stt.passcode);
        for (auto [i, c] : hash | vw::take(4) | vw::enumerate) {
            auto [x, y, dir] = dirs[i];
            auto [stt_x, stt_y] = stt.position;
            if ((stt_x + x) < 0 or (stt_y + y) < 0) continue;
            if ((stt_x + x) > 3 or (stt_y + y) > 3) continue;
            if (not "bcdef"s.contains(c)) continue;
            State new_state{stt.passcode + dir, {stt_x + x, stt_y + y}};
            q.push(new_state);
        }
    }

    println("Resultado da parte 1: {}", res);
}

void parte_2(std::vector<std::string>& lines) {
    State init{lines[0]};
    size_t init_passcode_size{lines[0].size()};
    tuple<int, int> end{3, 3};

    std::queue<State> q;
    q.push(init);
    string res{" :c"};
    while (not q.empty()) {
        State stt = q.front();
        q.pop();
        string hash = openssl_md5(stt.passcode);
        for (auto [i, c] : hash | vw::take(4) | vw::enumerate) {
            auto [x, y, dir] = dirs[i];
            auto [stt_x, stt_y] = stt.position;
            if ((stt_x + x) < 0 or (stt_y + y) < 0) continue;
            if ((stt_x + x) > 3 or (stt_y + y) > 3) continue;
            if (not "bcdef"s.contains(c)) continue;
            if ((stt_x + x) == 3 and (stt_y + y) == 3) {
                string final_passcode = stt.passcode + dir;
                res = final_passcode.substr(
                    init_passcode_size,
                    final_passcode.size() - init_passcode_size);
                continue;
            }
            State new_state{stt.passcode + dir, {stt_x + x, stt_y + y}};
            q.push(new_state);
        }
    }

    println("Resultado da parte 2: {}", res.size());
}

int main(int, char** argv) {
    std::print("\n");

    std::ifstream input;
    input.open(argv[1]);

    std::string line;
    std::vector<std::string> lines;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    input.close();

    parte_1(lines);
    parte_2(lines);

    return 0;
}
