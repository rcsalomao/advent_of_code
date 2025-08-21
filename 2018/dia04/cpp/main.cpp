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

struct Message {
    int month;
    int day;
    int hour;
    int minute;
    string msg;
};

auto parte_1(vector<string>& lines) {
    regex pat{R"(^\[\d+-(\d+)-(\d+) (\d+):(\d+)\] (.*)$)", regex::optimize};
    smatch m;
    vector<Message> messages;
    for (auto line : lines) {
        regex_match(line, m, pat);
        messages.emplace_back(stoi(m[1]), stoi(m[2]), stoi(m[3]), stoi(m[4]),
                              m[5]);
    }

    rg::sort(messages, [](Message& l, Message& r) {
        return std::tie(l.month, l.day, l.hour, l.minute) <
               std::tie(r.month, r.day, r.hour, r.minute);
    });

    unordered_map<int, std::array<int, 60>> guards_minutes;
    regex guard_pat{R"(.* #(\d+) .*)"};
    int asleep_minute{-1};
    int awake_minute{-1};
    std::array<int, 60>* ap{nullptr};
    int guard_id{-1};
    for (Message& message : messages) {
        if (regex_match(message.msg, m, guard_pat)) {
            guard_id = stoi(m[1]);
            ap = &guards_minutes[guard_id];
        } else if (message.msg == "falls asleep") {
            asleep_minute = message.minute;
        } else {
            awake_minute = message.minute;
            std::for_each(ap->begin() + asleep_minute,
                          ap->begin() + awake_minute, [](int& i) { i++; });
        }
    }

    auto most_sleepy = rg::max_element(
        guards_minutes, [](decltype(guards_minutes)::value_type& a,
                           decltype(guards_minutes)::value_type& b) {
            return std::reduce(a.second.begin(), a.second.end(), 0) <
                   std::reduce(b.second.begin(), b.second.end(), 0);
        });

    return most_sleepy->first *
           std::distance(most_sleepy->second.begin(),
                         rg::max_element(most_sleepy->second));
}

auto parte_2(vector<string>& lines) {
    regex pat{R"(^\[\d+-(\d+)-(\d+) (\d+):(\d+)\] (.*)$)", regex::optimize};
    smatch m;
    vector<Message> messages;
    for (auto line : lines) {
        regex_match(line, m, pat);
        messages.emplace_back(stoi(m[1]), stoi(m[2]), stoi(m[3]), stoi(m[4]),
                              m[5]);
    }

    rg::sort(messages, [](Message& l, Message& r) {
        return std::tie(l.month, l.day, l.hour, l.minute) <
               std::tie(r.month, r.day, r.hour, r.minute);
    });

    unordered_map<int, std::array<int, 60>> guards_minutes;
    regex guard_pat{R"(.* #(\d+) .*)"};
    int asleep_minute{-1};
    int awake_minute{-1};
    std::array<int, 60>* ap{nullptr};
    int guard_id{-1};
    for (Message& message : messages) {
        if (regex_match(message.msg, m, guard_pat)) {
            guard_id = stoi(m[1]);
            ap = &guards_minutes[guard_id];
        } else if (message.msg == "falls asleep") {
            asleep_minute = message.minute;
        } else {
            awake_minute = message.minute;
            std::for_each(ap->begin() + asleep_minute,
                          ap->begin() + awake_minute, [](int& i) { i++; });
        }
    }

    unordered_map<int, unordered_map<int, int>> minutes_map;
    for (auto [k, v] : guards_minutes) {
        for (size_t i : vw::iota(0, 60)) {
            minutes_map[i][k] += v[i];
        }
    }

    int result_guard{-1};
    int result_minute{-1};
    int max_v{0};
    for (auto [minute, map] : minutes_map) {
        for (auto [id, quant] : map) {
            if (quant > max_v) {
                result_guard = id;
                result_minute = minute;
                max_v = quant;
            }
        }
    }

    return result_guard * result_minute;
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
