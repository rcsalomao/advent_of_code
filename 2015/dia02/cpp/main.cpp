#include <algorithm>
#include <fstream>
#include <numeric>
#include <print>
#include <ranges>
#include <vector>

namespace rg = std::ranges;
namespace vw = std::views;

using std::string;

std::string to_string(std::ranges::view auto a) {
    // return std::string{a.begin(), a.end()};
    return std::string{std::from_range, a};
}

void parte_1(std::vector<std::string>& lines) {
    int total = 0;
    for (std::string_view line : lines) {
        auto splitted_line =
            line | vw::split('x') |
            vw::transform([](auto a) { return std::stoi(to_string(a)); }) |
            rg::to<std::vector<int>>();
        int a = splitted_line[0];
        int b = splitted_line[1];
        int c = splitted_line[2];
        // std::print("{} {} {}\n", a, b, c);
        std::vector faces_square_feet = {a * b, a * c, b * c};
        int total_square_feet = 2 * std::reduce(faces_square_feet.begin(),
                                                faces_square_feet.end(), 0) +
                                *rg::min_element(faces_square_feet);
        total += total_square_feet;
    }
    std::print("Total parte 1: {}\n", total);
};

void parte_2(std::vector<std::string>& lines) {
    int total = 0;
    for (std::string_view line : lines) {
        auto splitted_line =
            line | vw::split('x') | vw::transform([](auto a) {
                return std::stoi(std::string{std::from_range, a});
            }) |
            rg::to<std::vector<int>>();
        rg::partial_sort(splitted_line.begin(), splitted_line.begin() + 2,
                         splitted_line.end());
        int a = splitted_line[0];
        int b = splitted_line[1];
        // int c = splitted_line[2];
        // std::print("{} {} {}\n", a, b, c);
        int feet_1 = 2 * (a + b);
        int feet_2 = std::reduce(splitted_line.begin(), splitted_line.end(), 1,
                                 std::multiplies<>{});
        total += feet_1 + feet_2;
    }
    std::print("Total parte 2: {}\n", total);
};

int main(int argc, char** argv) {
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
