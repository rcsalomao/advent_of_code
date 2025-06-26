#include <algorithm>
#include <fstream>
#include <functional>
#include <numeric>
#include <print>
#include <ranges>
#include <regex>
#include <unordered_set>
#include <vector>

namespace rg = std::ranges;
namespace vw = std::views;

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

void parte_1(std::vector<std::string>& lines) {
    int total = 0;
    for (auto line : lines) {
        std::regex pattern_1{R"([aeiou])"};
        std::regex pattern_2{R"((\w)\1)"};
        std::regex pattern_3{R"(ab|cd|pq|xy)"};
        std::vector<std::string> matches_1 = find_all(line, pattern_1);
        std::vector<std::string> matches_2 = find_all(line, pattern_2);
        std::vector<std::string> matches_3 = find_all(line, pattern_3);
        if (matches_1.size() > 2 && matches_2.size() > 0 && matches_3.empty()) {
            total++;
        }
    }
    std::println("Total da parte 1: {}", total);
};

void parte_2(std::vector<std::string>& lines) {
    int total = 0;
    for (auto line : lines) {
        std::regex pattern_1{R"((\w)\w\1)"};
        std::regex pattern_2{R"((\w\w)\w*\1)"};
        std::vector<std::string> matches_1 = find_all(line, pattern_1);
        std::vector<std::string> matches_2 = find_all(line, pattern_2);
        if (!matches_1.empty() && !matches_2.empty()) {
            total++;
        }
    }
    std::println("Total da parte 2: {}", total);
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
