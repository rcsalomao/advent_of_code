// #include <Eigen/Dense>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
// #include <json.hpp>
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

void parte_1(std::vector<std::string>& lines) {
    const string salt = lines[0];
    unordered_map<int, string> cache;
    auto getHash = [&](int idx) {
        if (cache.contains(idx)) return cache[idx];
        string hash = openssl_md5(salt + to_string(idx));
        return cache[idx] = hash;
    };

    auto firstTrip = [&](const string& hash) -> char {
        for (size_t i = 0; i + 2 < hash.size(); i++)
            if (hash[i] == hash[i + 1] && hash[i] == hash[i + 2])
                return hash[i];
        return 0;
    };

    int found{0};
    int idx{0};
    bool done{false};
    int res{-1};
    while (not done) {
        string h = getHash(idx);
        char c = firstTrip(h);
        if (c) {
            string quint = string{from_range, std::views::repeat(c, 5)};
            for (int j : vw::iota(idx + 1, idx + 1001)) {
                if (getHash(j).contains(quint)) {
                    found++;
                    if (found == 64) {
                        res = idx;
                        done = true;
                    }
                    break;
                }
            }
        }
        idx++;
    }

    println("Resultado da parte 1: {}", res);
}

void parte_2(std::vector<std::string>& lines) {
    const string salt = lines[0];
    const int stretch = 2016;
    unordered_map<int, string> cache;
    auto getHash = [&](int idx) {
        if (cache.contains(idx)) return cache[idx];
        string hash = openssl_md5(salt + to_string(idx));
        for (int i = 0; i < stretch; i++) hash = openssl_md5(hash);
        return cache[idx] = hash;
    };

    auto firstTrip = [&](const string& hash) -> char {
        for (size_t i = 0; i + 2 < hash.size(); i++)
            if (hash[i] == hash[i + 1] && hash[i] == hash[i + 2])
                return hash[i];
        return 0;
    };

    int found{0};
    int idx{0};
    bool done{false};
    int res{-1};
    while (not done) {
        string h = getHash(idx);
        char c = firstTrip(h);
        if (c) {
            string quint = string{from_range, std::views::repeat(c, 5)};
            for (int j : vw::iota(idx + 1, idx + 1001)) {
                if (getHash(j).contains(quint)) {
                    found++;
                    if (found == 64) {
                        res = idx;
                        done = true;
                    }
                    break;
                }
            }
        }
        idx++;
    }

    println("Resultado da parte 2: {}", res);
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
