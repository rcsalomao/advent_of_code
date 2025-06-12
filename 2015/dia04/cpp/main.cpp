#include <algorithm>
#include <fstream>
#include <functional>
#include <numeric>
#include <print>
#include <ranges>
#include <unordered_set>
#include <vector>

#include "boost/algorithm/hex.hpp"
#include "boost/uuid/detail/md5.hpp"
#include "openssl/md5.h"

namespace rg = std::ranges;
namespace vw = std::views;

using boost::uuids::detail::md5;

using std::to_string;

std::string to_string(const md5::digest_type& digest) {
    const auto char_digest = reinterpret_cast<const char*>(&digest);
    std::string result;
    boost::algorithm::hex(char_digest, char_digest + sizeof(md5::digest_type),
                          std::back_inserter(result));
    return result;
}

void parte_1_boost(std::vector<std::string>& lines) {
    std::string input = lines[0];

    md5 hash;
    md5::digest_type digest;

    std::string zeros = "00000";
    int max_tries{10000000};
    // int max_tries{10};
    for (int i : vw::iota(0, max_tries)) {
        std::string _try = input + to_string(i);
        hash.process_bytes(_try.data(), _try.size());
        hash.get_digest(digest);
        // std::print("{}\n", _try);
        // std::print("{}\n", to_string(digest));
        if (to_string(digest).substr(0, 5) == zeros) {
            std::print("{}\n", i);
            std::print("{}\n", _try);
            std::print("{}\n", to_string(digest));
            break;
        };
    };
};

std::string openssl_md5(const std::string& str) {
    unsigned char hash[MD5_DIGEST_LENGTH];

    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, str.c_str(), str.size());
    MD5_Final(hash, &md5);

    std::stringstream ss;

    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(hash[i]);
    }
    return ss.str();
};

void parte_1_openssl(std::vector<std::string>& lines) {
    std::string input = lines[0];

    std::string zeros = "00000";
    int max_tries{10000000};
    // int max_tries{10};
    for (int i : vw::iota(0, max_tries)) {
        std::string _try = input + to_string(i);
        auto hex_try = openssl_md5(_try);
        // std::print("{}\n", _try);
        // std::print("{}\n", to_string(digest));
        if (hex_try.substr(0, 5) == zeros) {
            std::print("{}\n", i);
            std::print("{}\n", _try);
            std::print("{}\n", hex_try);
            break;
        };
    };
};

void parte_2(std::vector<std::string>& lines) {
    std::string input = lines[0];

    std::string zeros = "000000";
    int max_tries{10000000};
    // int max_tries{10};
    for (int i : vw::iota(0, max_tries)) {
        std::string _try = input + to_string(i);
        auto hex_try = openssl_md5(_try);
        // std::print("{}\n", _try);
        // std::print("{}\n", to_string(digest));
        if (hex_try.substr(0, 6) == zeros) {
            std::print("{}\n", i);
            std::print("{}\n", _try);
            std::print("{}\n", hex_try);
            break;
        };
    };
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

    // parte_1_boost(lines);  // Não dá certo. Aparentemente há alguma alteração
    //                        // no algoritmo da 'boost'
    parte_1_openssl(lines);

    std::print("\n");

    parte_2(lines);

    return 0;
}
