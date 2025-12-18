// #include <Eigen/Dense>
// #include <nlohmann/json.hpp>
// #include <openssl/md5.h>
#include <bits/stdc++.h>

namespace rg = std::ranges;
namespace vw = std::views;
using namespace std;
using namespace std::string_literals;
using namespace std::string_view_literals;

// using nlohmann::json;

void pr(auto&& val, auto&&... vals) {
    std::stringstream ss;
    ss << std::format("{}", val);
    (..., (ss << " " << std::format("{}", vals)));
    println("{}", ss.str());
}

vector<string> split(string const& str, string_view delim) {
    return vector<string>{std::from_range,
                          str | vw::split(delim) | vw::transform([](auto a) {
                              return string{std::from_range, a};
                          })};
}

long pow(long exponent, long base = 10) {
    if (exponent == 0) return 1;
    if (exponent == 1) return base;
    return base * pow(exponent - 1, base);
}

auto parte_1(vector<string>& lines) {
    auto ranges = split(lines[0], ",");
    long total{0};
    for (auto range : ranges) {
        auto vs = split(range, "-");
        long a{stol(vs[0])};
        long b{stol(vs[1])};
        for (long id : vw::iota(a, b + 1)) {
            long const n_digits{long(log10(id) + 1)};
            bool is_even_num_digits = n_digits % 2 == 0 ? true : false;
            if (not is_even_num_digits) {
                continue;
            }
            long c{id % pow(n_digits / 2)};
            long d{id / pow(n_digits / 2)};
            if (c != d) {
                continue;
            }
            total += id;
        }
    }
    return total;
}

long get_digits(long num, long num_sz, long group_idx, long group_sz,
                long base = 10) {
    return num / pow(num_sz - group_sz * (1 + group_idx), base) -
           num / pow(num_sz - group_idx * group_sz, base) * pow(group_sz, base);
}

auto parte_2(vector<string>& lines) {
    auto ranges = split(lines[0], ",");
    long total{0};
    for (auto range : ranges) {
        auto vs = split(range, "-");
        long a{stol(vs[0])};
        long b{stol(vs[1])};
        for (long id : vw::iota(a, b + 1)) {
            long const n_digits{long(log10(id) + 1)};
            long half_n_digits = n_digits / 2;
            bool is_id{false};
            for (long i{half_n_digits}; i >= 1; i--) {
                if (n_digits % i == 0) {
                    long x0{get_digits(id, n_digits, 0, i)};
                    bool inner_test{true};
                    for (long j{1}; j < n_digits / i; j++) {
                        long x{get_digits(id, n_digits, j, i)};
                        if (x0 != x) {
                            inner_test = false;
                            break;
                        }
                    }
                    if (not inner_test) continue;
                    is_id = true;
                    break;
                }
            }
            if (is_id) {
                total += id;
            }
        }
    }
    return total;
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

    println();
    println("Resultado da parte 1: {}", parte_1(lines));
    println("Resultado da parte 2: {}", parte_2(lines));

    return 0;
}
