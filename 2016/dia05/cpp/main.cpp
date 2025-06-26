// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
// #include <json.hpp>
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

#include "openssl/md5.h"

namespace rg = std::ranges;
namespace vw = std::views;
using namespace std::string_view_literals;
using std::map;
using std::print;
using std::println;
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

std::string openssl_md5(const std::string& str) {
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

void parte_1(std::vector<std::string>& lines) {
    std::string input = lines[0];

    std::string zeros = "00000";

    string password{};
    int max_iter{10000000};
    for (int i : vw::iota(0, max_iter)) {
        std::string _try = input + to_string(i);
        string hex_try = openssl_md5(_try);
        if (hex_try.substr(0, 5) == zeros) {
            password += hex_try[5];
            if (password.size() == 8) break;
        };
        if (i == max_iter - 1) {
            println("Número máximo de iterações alcançado :c");
        }
    };

    println("Resultado da parte 1: {}", password);
};

void parte_2(std::vector<std::string>& lines) {
    std::string input = lines[0];

    vector<bool> filled_positions(8, false);
    std::string zeros = "00000";

    string password{"________"};
    int max_iter{50000000};
    unsigned n_filled_positions{0};
    for (int i : vw::iota(0, max_iter)) {
        std::string _try = input + to_string(i);
        string hex_try = openssl_md5(_try);
        if (hex_try.substr(0, 5) == zeros) {
            int position = hex_try[5] - '0';
            if (position < 8) {
                if (not filled_positions[position]) {
                    password[position] = hex_try[6];
                    filled_positions[position] = true;
                    n_filled_positions++;
                }
            }
        };
        if (n_filled_positions == 8) break;
        if (i == max_iter - 1) {
            println("Número máximo de iterações alcançado :c");
        }
    };

    println("Resultado da parte 2: {}", password);
};

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
