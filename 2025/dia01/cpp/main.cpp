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

auto parte_1(vector<string>& lines) {
    int dial_pointer{50};
    int i{0};
    for (string_view line : lines) {
        int sign = line.starts_with("R") ? 1 : -1;
        int num = std::stoi(string{line.substr(1, line.size() - 1)}) % 100;
        dial_pointer += (sign * num);
        dial_pointer =
            dial_pointer >= 0 ? dial_pointer % 100 : (100 + dial_pointer);
        if (dial_pointer == 0) ++i;
    }
    return i;
}

auto parte_2(vector<string>& lines) {
    int dial_pointer{50};
    int i{0};
    for (string_view line : lines) {
        int sign = line.starts_with("R") ? 1 : -1;
        int a = std::stoi(string{line.substr(1, line.size() - 1)});
        int num = a % 100;
        i += a / 100;
        int previous_dial_pointer{dial_pointer};
        dial_pointer += (sign * num);
        if (dial_pointer > 100) {
            ++i;
            dial_pointer %= 100;
        } else if (dial_pointer < 0) {
            if (previous_dial_pointer != 0) ++i;
            dial_pointer += 100;
        } else if (dial_pointer == 100 or dial_pointer == 0) {
            ++i;
            dial_pointer %= 100;
        }
    }
    return i;
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
