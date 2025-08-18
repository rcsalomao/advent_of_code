// #include <Eigen/Dense>
// #include <json.hpp>
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

class Conector {
   public:
    Conector(int val_a, int val_b) : val_a(val_a), val_b(val_b) {};
    Conector(Conector&&) = default;
    Conector(const Conector&) = default;
    Conector& operator=(Conector&&) = default;
    Conector& operator=(const Conector&) = default;
    ~Conector() = default;

    int val_a;
    int val_b;
    bool used{false};

   private:
};

string to_string(const Conector& conec) {
    return std::format("{}/{}", conec.val_a, conec.val_b);
}

void recurr_1(vector<Conector>& conectors, int val, int score, int& max_score) {
    max_score = std::max(max_score, score);
    for (auto& c : conectors) {
        int va{c.val_a};
        int vb{c.val_b};
        bool bva{va == val};
        if (!c.used and (bva or vb == val)) {
            c.used = true;
            recurr_1(conectors, bva ? vb : va, score + va + vb, max_score);
            c.used = false;
        }
    }
}

void parte_1(vector<string>& lines) {
    vector<Conector> conectors;
    for (auto line : lines) {
        vector<string> vs = split(line, "/");
        conectors.emplace_back(stoi(vs[0]), stoi(vs[1]));
    }

    int max_score{0};
    recurr_1(conectors, 0, 0, max_score);

    println("Resultado da parte 1: {}", max_score);
}

void recurr_2(vector<Conector>& conectors, int val, int score, int& max_score,
              int length, int& max_length) {
    if (length > max_length) {
        max_length = length;
        max_score = 0;
    }
    if (length == max_length) max_score = std::max(max_score, score);
    for (auto& c : conectors) {
        int va{c.val_a};
        int vb{c.val_b};
        bool bva{va == val};
        if (!c.used and (bva or vb == val)) {
            c.used = true;
            recurr_2(conectors, bva ? vb : va, score + va + vb, max_score,
                     length + 1, max_length);
            c.used = false;
        }
    }
}

void parte_2(vector<string>& lines) {
    vector<Conector> conectors;
    for (auto line : lines) {
        vector<string> vs = split(line, "/");
        conectors.emplace_back(stoi(vs[0]), stoi(vs[1]));
    }

    int max_score{0};
    int max_length{0};
    recurr_2(conectors, 0, 0, max_score, 0, max_length);

    println("Resultado da parte 2: {}", max_score);
}

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
    parte_2(lines);

    return 0;
}
