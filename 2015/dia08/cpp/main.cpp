#include <Eigen/Dense>
#include <algorithm>
#include <fstream>
#include <functional>
#include <iomanip>
#include <numeric>
#include <print>
#include <queue>
#include <ranges>
#include <regex>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace rg = std::ranges;
namespace vw = std::views;

size_t get_size_inmemory(const std::string& line) {
    size_t size{0};
    for (size_t index{0}; index < line.size(); index++) {
        size++;
        if (line[index] == '\\' and line[index + 1] == '\\') {
            index++;
            continue;
        };
        if (line[index] == '\\' and line[index + 1] == '"') {
            index++;
            continue;
        };
        if (line[index] == '\\' and line[index + 1] == 'x') {
            index += 3;
            continue;
        }
    }
    return size - 2;
}

void parte_1(std::vector<std::string>& lines) {
    // int total_len =
    //     std::transform_reduce(lines.begin(), lines.end(), 0, std::plus<>{},
    //                           [](std::string s) { return s.length(); });
    int total_len = 0;
    int total_inmemory = 0;
    for (auto line : lines) {
        total_len += line.size();
        total_inmemory += get_size_inmemory(line);
    }
    std::println("Total da parte 1: {}", total_len - total_inmemory);
};

void parte_2(std::vector<std::string>& lines) {
    int total_len = 0;
    int total_new_encoded = 0;
    for (auto line : lines) {
        total_len += line.size();
        std::stringstream ss;
        ss << std::quoted(line);
        total_new_encoded += ss.str().size();
    }
    std::println("Total da parte 2: {}", total_new_encoded - total_len);
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
