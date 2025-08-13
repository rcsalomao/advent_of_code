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

void parte_1(vector<string>& lines) {
    regex pat{R"(^p=<(.*)>, v=<(.*)>, a=<(.*)>$)"};
    vector<int> accelerations;
    for (auto line : lines) {
        smatch m;
        regex_match(line, m, pat);
        vector<int> a_i = vector{
            from_range, split(m[3], ",") |
                            vw::transform([](string sv) { return stoi(sv); })};
        accelerations.push_back(
            std::reduce(a_i.begin(), a_i.end(), 0,
                        [](int a, int b) { return a + abs(b); }));
    }
    auto min_it = rg::min_element(accelerations);
    println("Resultado da parte 1: {}",
            std::distance(accelerations.begin(), min_it));
}

struct Particle {
    vector<long> pos;
    vector<long> vel;
    vector<long> acc;
};

void sum_vector(vector<long>& v1, vector<long>& v2) {
    for (auto [a, b] : vw::zip(v1, v2)) {
        a += b;
    }
}

string to_string(vector<long>& v) {
    string r{"["};
    for (auto& l : rg::subrange(v.begin(), v.end() - 1)) {
        r += to_string(l) + ";";
    }
    r += to_string(v.back()) + "]";
    return r;
}

void parte_2(vector<string>& lines) {
    regex pat{R"(^p=<(.*)>, v=<(.*)>, a=<(.*)>$)"};
    unordered_map<int, Particle> particles;
    for (auto [i, line] : vw::enumerate(lines)) {
        smatch m;
        regex_match(line, m, pat);
        vector<long> p_i = vector{
            from_range,
            split(m[1], ",") | vw::transform([](string s) { return stol(s); })};
        vector<long> v_i = vector{
            from_range,
            split(m[2], ",") | vw::transform([](string s) { return stol(s); })};
        vector<long> a_i = vector{
            from_range,
            split(m[3], ",") | vw::transform([](string s) { return stol(s); })};
        particles[i] = {p_i, v_i, a_i};
    }

    int64_t iters{100};
    while (iters--) {
        for (auto& [k, particle] : particles) {
            sum_vector(particle.vel, particle.acc);
            sum_vector(particle.pos, particle.vel);
        }

        unordered_map<string, vector<int>> positions_count;
        for (auto& [k, particle] : particles) {
            positions_count[to_string(particle.pos)].push_back(k);
        }

        for (auto& [p, ps] : positions_count) {
            if (ps.size() > 1) {
                for (auto k : ps) {
                    particles.erase(k);
                }
            }
        }
    }

    size_t n_particles{particles.size()};

    println("Resultado da parte 2: {}", n_particles);
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
