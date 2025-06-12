#include <Eigen/Dense>
#include <algorithm>
#include <boost/functional/hash.hpp>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <json.hpp>
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
#include <vector>

namespace rg = std::ranges;
namespace vw = std::views;
using namespace std::string_view_literals;
using std::map;
using std::print;
using std::println;
using std::string;
using std::string_view;
using std::to_string;
using std::tuple;
using std::unordered_map;
using std::vector;

using nlohmann::json;

vector<string> split(const string& str, string_view delim) {
    return vector<string>{std::from_range,
                          str | vw::split(delim) | vw::transform([](auto a) {
                              return string{std::from_range, a};
                          })};
}

enum class Estado { correndo, descansando };

class Rena {
    string _nome;
    int _velocidade_media;
    int _tempo_corrida;
    int _tempo_descanso;
    Estado _estado{Estado::correndo};
    int _tempo_correndo{0};
    int _tempo_descansando{0};
    int _distancia_total{0};
    int _pontuacao{0};

   public:
    Rena(string nome, int velocidade_media, int tempo_corrida,
         int tempo_descanso)
        : _nome(nome),
          _velocidade_media(velocidade_media),
          _tempo_corrida(tempo_corrida),
          _tempo_descanso(tempo_descanso) {}
    void corra_dt() {
        if (_estado == Estado::correndo) {
            _distancia_total += _velocidade_media;
            _tempo_correndo++;
            if (_tempo_correndo >= _tempo_corrida) {
                _estado = Estado::descansando;
                _tempo_descansando = 0;
            }
        }
        if (_estado == Estado::descansando) {
            _tempo_descansando++;
            if (_tempo_descansando > _tempo_descanso) {
                _estado = Estado::correndo;
                _tempo_correndo = 0;
            }
        }
    }
    string nome() { return _nome; }
    int pontuacao() { return _pontuacao; }
    int distancia_total() { return _distancia_total; }
    void dar_pontuacao() { _pontuacao++; }
};

void parte_1(std::vector<std::string>& lines) {
    int tempo_total{2503};
    // int tempo_total{1000};
    vector<Rena> renas{
        std::from_range,
        lines | vw::transform([](string line) {
            std::smatch m;
            std::regex_match(
                line, m,
                std::regex{
                    R"((\w+) can fly (\d+) km/s for (\d+) seconds, .* (\d+) seconds.)"});
            return Rena{m[1], std::stoi(m[2]), std::stoi(m[3]),
                        std::stoi(m[4])};
        })};
    while (tempo_total) {
        for (auto& rena : renas) {
            rena.corra_dt();
        }
        tempo_total--;
    }
    int i{0}, max_dist{0};
    for (auto [j, rena] : vw::enumerate(renas)) {
        if (rena.distancia_total() > max_dist) {
            i = j;
            max_dist = rena.distancia_total();
        }
    }
    println("Resultado da parte 1: Rena={} Distância máxima={}",
            renas[i].nome(), renas[i].distancia_total());
};

void parte_2(std::vector<std::string>& lines) {
    int tempo_total{2503};
    // int tempo_total{1000};
    vector<Rena> renas{
        std::from_range,
        lines | vw::transform([](string line) {
            std::smatch m;
            std::regex_match(
                line, m,
                std::regex{
                    R"((\w+) can fly (\d+) km/s for (\d+) seconds, .* (\d+) seconds.)"});
            return Rena{m[1], std::stoi(m[2]), std::stoi(m[3]),
                        std::stoi(m[4])};
        })};
    while (tempo_total) {
        for (auto& rena : renas) {
            rena.corra_dt();
        }
        rg::sort(renas, std::greater<>{}, &Rena::distancia_total);
        int leader_dist = renas[0].distancia_total();
        for (auto& rena : renas) {
            if (rena.distancia_total() < leader_dist) break;
            rena.dar_pontuacao();
        }
        tempo_total--;
    }
    rg::sort(renas, std::greater<>{}, &Rena::pontuacao);
    println("Resultado da parte 2: Rena={} Pontuação máxima={}",
            renas[0].nome(), renas[0].pontuacao());
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
