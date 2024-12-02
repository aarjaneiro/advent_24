// (c) 2024 Aaron Janeiro Stone

#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <map>

#define INPUT_LOC ADVENT_24_DIR "/day_1/input.txt"

static std::multiset<int> l_num;
static std::multiset<int> r_num;
static std::map<int, int> sim_cache;
static std::string number_buffer;

int main() {
    std::string line;
    std::ifstream file(INPUT_LOC);
    size_t sz = 0;
    int diff_out = 0;

    // Setup
    while (std::getline(file, line)) {
        number_buffer.clear();
        for (char c: line) {
            if (c == ' ') {
                if (number_buffer.empty()) {
                    continue;
                }
                l_num.insert(std::stoi(number_buffer));
                number_buffer.clear();
            } else {
                number_buffer.push_back(c);
            }
        }

        r_num.insert(std::stoi(number_buffer));
        sz++;
    }

    // Part 1
    auto l_start = l_num.begin();
    auto r_start = r_num.begin();
    for (int i = 0; i < sz; i++) {
        int diff = *l_start - *r_start;
        diff_out += diff > 0 ? diff : -diff;
        l_start++;
        r_start++;
    }
    std::cout << "Difference score: " << diff_out << std::endl;


    // Part 2
    int similarity = 0;
    for(int i : l_num){
        if (sim_cache.find(i) == sim_cache.end()) {
            sim_cache[i] = (int) r_num.count(i) * i;
        }
        similarity += sim_cache[i];
    }

    std::cout << "Similarity score: " << similarity << std::endl;
}


