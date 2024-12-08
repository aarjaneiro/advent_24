// (c) 2024 Aaron Janeiro Stone

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>

#define INPUT_LOC ADVENT_24_DIR "/day_5/input.txt"

// Originally misinterpreted this to be a transitive relation...
//
// Note to anyone trying this in the future:
// The example data will still pass if you treat it as transitive!
// Your actual input data, however, probably will not.

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

struct PageOrderInfo {
    std::set<uint> smaller_than{};
};

static std::map<uint, PageOrderInfo> page_number_to_info{};


bool is_valid(uint prev_n, uint next) {
    if ((prev_n != 0 && page_number_to_info.find(prev_n) == page_number_to_info.end()) ||
        page_number_to_info.find(next) == page_number_to_info.end()) {
        return false;
    }
    PageOrderInfo &prev = page_number_to_info[prev_n];
    if (prev_n == 0 || prev.smaller_than.find(next) != prev.smaller_than.end()) {
        return true;
    }
    return false;
}

// Topological DFS to find an ordering that works
// https://en.wikipedia.org/wiki/Topological_sorting#Depth-first_search

void visit(uint n, std::set<uint> &unmarked, std::list<uint> &ordered) {
    if (unmarked.find(n) == unmarked.end()) {
        return;
    }
    unmarked.erase(n);
    for (uint m: page_number_to_info[n].smaller_than) {
        visit(m, unmarked, ordered);
    }
    ordered.push_front(n);
}


bool try_rearrange(std::vector<uint> &numbers) {
    std::set<uint> unmarked = std::set<uint>(numbers.begin(), numbers.end());
    std::list<uint> ordered;
    while (!unmarked.empty()) {
        uint n = *unmarked.begin();
        visit(n, unmarked, ordered);
    }
    if (ordered.size() != numbers.size()) {
        return false;
    }
    numbers = std::vector<uint>(ordered.begin(), ordered.end());
    return true;
}


int main() {
    std::fstream input_file(INPUT_LOC);
    std::string line;
    std::string int_buffer;
    std::vector<uint> numbers;
    bool skip = false;
    uint last_rule = 0;
    uint middle_sum = 0; // added only if valid
    uint rearranged_middle_sum = 0; // added if invalid until rearranged

    // Consume rules
    while (std::getline(input_file, line)) {
        if (line.empty()) {
            break; // move to next section
        }
        numbers.clear();
        for (char c: line) {
            if (is_digit(c)) {
                int_buffer += c;
            } else {
                numbers.push_back(std::stoi(int_buffer));
                int_buffer.clear();
            }
        }
        numbers.push_back(std::stoi(int_buffer));
        int_buffer.clear();

        // Add to map
        for (uint8_t n = 0; n <= 1; n++) {
            uint i = numbers[n];
            if (page_number_to_info.find(i) == page_number_to_info.end()) {
                page_number_to_info[i] = PageOrderInfo{};
            }
        }

        // Add to relation data
        if (page_number_to_info[numbers[0]].smaller_than.find(numbers[1]) ==
            page_number_to_info[numbers[0]].smaller_than.end()) {
            page_number_to_info[numbers[0]].smaller_than.insert(numbers[1]);
        }
    }


    // Apply rules
    while (std::getline(input_file, line)) {
        numbers.clear();
        for (char c: line) {
            if (is_digit(c)) {
                int_buffer += c;
            } else {
                numbers.push_back(std::stoi(int_buffer));
                int_buffer.clear();
            }
        }
        numbers.push_back(std::stoi(int_buffer));
        int_buffer.clear();

        // Check if valid
        last_rule = 0;
        for (uint v: numbers) {
            if (is_valid(last_rule, v)) {
                last_rule = v;
            } else {
                skip = true;

                // Try to rearrange
                if (try_rearrange(numbers)) {
                    rearranged_middle_sum += numbers[(numbers.size() - 1) / 2];
                }

                break;
            }
        }
        if (skip) {
            skip = false;
            continue;
        }

        // Add middle number to sum
        middle_sum += numbers[(numbers.size() - 1) / 2];

    }
    input_file.close();

    std::cout << "Middle sum: " << middle_sum << std::endl;
    std::cout << "Rearranged middle sum: " << rearranged_middle_sum << std::endl;

}