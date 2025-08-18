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

template <typename T>
struct Matrix {
    vector<T> v;
    size_t m;
    size_t n;
    size_t sz;

    Matrix(size_t m, size_t n, T initial_value) : m{m}, n{n}, sz{m * n} {
        assert(m == n);
        v = vector<T>(sz, initial_value);
    }

    T& operator[](size_t i, size_t j) { return v[i * m + j]; }

    void transpose() {
        // only works for square matrixes
        for (size_t i = 0; i < m; i++) {
            for (size_t j = i; j < n; j++) {
                if (i == j) continue;
                auto temp{(*this)[i, j]};
                (*this)[i, j] = (*this)[j, i];
                (*this)[j, i] = temp;
            }
        }
    }

    void flip(uint axis = 0) {
        if (axis) {
            for (size_t j{0}; j < n / 2; j++) {
                for (size_t i{0}; i < m; i++) {
                    auto temp{(*this)[i, j]};
                    (*this)[i, j] = (*this)[i, n - 1 - j];
                    (*this)[i, n - 1 - j] = temp;
                }
            }
        } else {
            for (size_t i{0}; i < m / 2; i++) {
                for (size_t j{0}; j < n; j++) {
                    auto temp{(*this)[i, j]};
                    (*this)[i, j] = (*this)[m - 1 - i, j];
                    (*this)[m - 1 - i, j] = temp;
                }
            }
        }
    }

    void rot90() {
        (*this).transpose();
        (*this).flip(1);
    }
};

template <typename T>
string to_string(Matrix<T>& m, string sep = " ") {
    string r;
    for (size_t i{0}; i < m.m - 1; i++) {
        for (size_t j{0}; j < m.n - 1; j++) {
            r += to_string(m[i, j]) + sep;
        }
        r += to_string(m[i, m.n - 1]) + "\n";
    }
    for (size_t j{0}; j < m.n - 1; j++) {
        r += to_string(m[m.m - 1, j]) + sep;
    }
    r += to_string(m[m.m - 1, m.n - 1]);
    return r;
}

template <typename T>
string encode(Matrix<T>& m) {
    return string{from_range,
                  m.v | vw::transform([](auto& a) { return to_string(a); }) |
                      vw::join | vw::common};
}
string encode(Matrix<string>& m) { return string{from_range, m.v | vw::join}; }
string encode(Matrix<char>& m) {
    return string{from_range, m.v | vw::transform([](auto& a) {
                                  return to_string(a);
                              }) | vw::join};
}

template <typename T>
class Grid {
   public:
    Grid() = default;

    T& operator[](string key) { return grid_map[key]; }
    T& operator[](int i, int j) {
        string key = std::vformat("{},{}", std::make_format_args(i, j));
        return grid_map[key];
    }
    bool check_key(string key) { return grid_map.contains(key); }
    bool check_key(int i, int j) {
        string key = std::vformat("{},{}", std::make_format_args(i, j));
        return grid_map.contains(key);
    }
    auto& get_map() { return grid_map; }

   private:
    unordered_map<string, T> grid_map;
};

template <typename T>
void print_grid(Grid<T>& grid, size_t sz_i, size_t sz_j) {
    for (size_t i = 0; i < sz_i; i++) {
        for (size_t j = 0; j < sz_j; j++) {
            print("{}", grid[i, j]);
        }
        println();
    }
}

void update_canvas(Grid<string>& base_canvas,
                   unordered_map<string, string>& rules, size_t& size,
                   size_t i_base, size_t o_base) {
    size_t n{size / i_base};
    Grid<string> new_canvas;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            string pat;
            for (size_t k = 0; k < i_base; k++) {
                for (size_t l = 0; l < i_base; l++) {
                    pat += base_canvas[i * i_base + k, j * i_base + l];
                }
            }
            if (!rules.contains(pat)) {
                println("Padrão {} não encontrado nas regras!", pat);
                exit(1);
            }
            string out = rules[pat];
            for (size_t k = 0; k < o_base; k++) {
                for (size_t l = 0; l < o_base; l++) {
                    new_canvas[i * o_base + k, j * o_base + l] =
                        out[k * o_base + l];
                }
            }
        }
    }
    base_canvas = new_canvas;
    size = n * o_base;
}

void parte_1(vector<string>& lines) {
    unordered_map<string, string> rules;
    auto create_matrix = [](vector<string>& vs, string iv = " ") {
        size_t sz{vs.size()};
        Matrix<string> m{sz, sz, iv};
        for (size_t i = 0; i < sz; i++) {
            for (size_t j = 0; j < sz; j++) {
                m[i, j] = vs[i][j];
            }
        }
        return m;
    };
    regex pat{R"(^(.*) => (.*)$)"};
    for (auto line : lines) {
        smatch m;
        regex_match(line, m, pat);
        vector<string> input = split(m[1].str(), "/");
        vector<string> output = split(m[2].str(), "/");
        Matrix<string> im = create_matrix(input);
        Matrix<string> om = create_matrix(output);
        string encoded_output = encode(om);
        rules[encode(im)] = encoded_output;
        im.flip(0);
        string f0_ei{encode(im)};
        if (!rules.contains(f0_ei)) rules[f0_ei] = encoded_output;
        im.flip(0);
        im.flip(1);
        string f1_ei{encode(im)};
        if (!rules.contains(f1_ei)) rules[f1_ei] = encoded_output;
        im.flip(1);
        for (size_t k = 0; k < 3; k++) {
            im.rot90();
            string rot_ei{encode(im)};
            if (!rules.contains(rot_ei)) rules[rot_ei] = encoded_output;
            im.flip(0);
            string f0_ei{encode(im)};
            if (!rules.contains(f0_ei)) rules[f0_ei] = encoded_output;
            im.flip(0);
            im.flip(1);
            string f1_ei{encode(im)};
            if (!rules.contains(f1_ei)) rules[f1_ei] = encoded_output;
            im.flip(1);
        }
    }

    Grid<string> canvas;
    canvas[0, 0] = ".";
    canvas[0, 1] = "#";
    canvas[0, 2] = ".";
    canvas[1, 0] = ".";
    canvas[1, 1] = ".";
    canvas[1, 2] = "#";
    canvas[2, 0] = "#";
    canvas[2, 1] = "#";
    canvas[2, 2] = "#";
    size_t size{3};

    uint iters{5};
    while (iters--) {
        if (size % 2 == 0) {
            update_canvas(canvas, rules, size, 2, 3);
        } else {
            update_canvas(canvas, rules, size, 3, 4);
        }
    }
    // print_grid(canvas, size, size);

    ulong count{0};
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            if (canvas[i, j] == "#") count++;
        }
    }

    println("Resultado da parte 1: {}", count);
}

void parte_2(vector<string>& lines) {
    unordered_map<string, string> rules;
    auto create_matrix = [](vector<string>& vs, string iv = " ") {
        size_t sz{vs.size()};
        Matrix<string> m{sz, sz, iv};
        for (size_t i = 0; i < sz; i++) {
            for (size_t j = 0; j < sz; j++) {
                m[i, j] = vs[i][j];
            }
        }
        return m;
    };
    regex pat{R"(^(.*) => (.*)$)"};
    for (auto line : lines) {
        smatch m;
        regex_match(line, m, pat);
        vector<string> input = split(m[1].str(), "/");
        vector<string> output = split(m[2].str(), "/");
        Matrix<string> im = create_matrix(input);
        Matrix<string> om = create_matrix(output);
        string encoded_output = encode(om);
        rules[encode(im)] = encoded_output;
        im.flip(0);
        string f0_ei{encode(im)};
        if (!rules.contains(f0_ei)) rules[f0_ei] = encoded_output;
        im.flip(0);
        im.flip(1);
        string f1_ei{encode(im)};
        if (!rules.contains(f1_ei)) rules[f1_ei] = encoded_output;
        im.flip(1);
        for (size_t k = 0; k < 3; k++) {
            im.rot90();
            string rot_ei{encode(im)};
            if (!rules.contains(rot_ei)) rules[rot_ei] = encoded_output;
            im.flip(0);
            string f0_ei{encode(im)};
            if (!rules.contains(f0_ei)) rules[f0_ei] = encoded_output;
            im.flip(0);
            im.flip(1);
            string f1_ei{encode(im)};
            if (!rules.contains(f1_ei)) rules[f1_ei] = encoded_output;
            im.flip(1);
        }
    }

    Grid<string> canvas;
    canvas[0, 0] = ".";
    canvas[0, 1] = "#";
    canvas[0, 2] = ".";
    canvas[1, 0] = ".";
    canvas[1, 1] = ".";
    canvas[1, 2] = "#";
    canvas[2, 0] = "#";
    canvas[2, 1] = "#";
    canvas[2, 2] = "#";
    size_t size{3};

    uint iters{18};
    while (iters--) {
        if (size % 2 == 0) {
            update_canvas(canvas, rules, size, 2, 3);
        } else {
            update_canvas(canvas, rules, size, 3, 4);
        }
    }
    // print_grid(canvas, size, size);

    ulong count{0};
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            if (canvas[i, j] == "#") count++;
        }
    }

    println("Resultado da parte 2: {}", count);
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
