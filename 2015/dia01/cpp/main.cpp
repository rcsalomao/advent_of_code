#include <fstream>
#include <print>
#include <vector>

void parte_1(std::string line) {
    int floor{0};
    for (char c : line) {
        if (c == '(') ++floor;
        if (c == ')') --floor;
    }
    std::print("{}\n", floor);
};

void parte_2(std::string line) {
    int floor{0};
    int position{0};
    for (char c : line) {
        if (c == '(') ++floor;
        if (c == ')') --floor;
        ++position;
        if (floor == -1) break;
    }
    std::print("{}\n", position);
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

    parte_1(lines[0]);
    parte_2(lines[0]);

    return 0;
}
