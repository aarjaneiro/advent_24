// (c) 2024 Aaron Janeiro Stone

// TODO: Finish part 2 (output is too high)

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>

#define INPUT_LOC ADVENT_24_DIR "/day_6/input.txt"


#define FLAG_FWD (1 << 0)
#define FLAG_BWD (1 << 1)
#define FLAG_LEFT (1 << 2)
#define FLAG_RIGHT (1 << 3)

static std::vector<bool> grid{};

// finds if there exists a size-2 subsequence that repeats
bool is_repeating_subseq(const std::vector<uint> &numbers) {
    for (uint i = 0; i < numbers.size() - 1; i++) {
        for (uint j = i + 1; j < numbers.size() - 1; j++) {
            if (numbers[i] == numbers[j] && numbers[i + 1] == numbers[j + 1]) {
                return true;
            }
        }
    }
    return false;
}


template<typename VisitCtr = std::set<uint>>
static bool
move_guard(uint &guard_x, uint &guard_y, uint8_t &direction, uint &next_x, uint &next_y, VisitCtr &visited,
           uint row_sz, uint max_sz) {
    switch (direction) {
        case FLAG_FWD:
            next_x = guard_x;
            next_y = guard_y - 1;
            break;
        case FLAG_BWD:
            next_x = guard_x;
            next_y = guard_y + 1;
            break;
        case FLAG_LEFT:
            next_x = guard_x - 1;
            next_y = guard_y;
            break;
        case FLAG_RIGHT:
            next_x = guard_x + 1;
            next_y = guard_y;
            break;
    }

    if (next_y * row_sz + next_x >= max_sz) {
        // last step
        if constexpr (std::is_same_v<VisitCtr, std::set<uint>>) {
            visited.insert(guard_y * row_sz + guard_x);
        } else {
            visited.push_front(guard_y * row_sz + guard_x);
        }
        return false;
    }

    if (grid[next_y * row_sz + next_x]) {
        // no obstacle
        guard_x = next_x;
        guard_y = next_y;
        if constexpr (std::is_same_v<VisitCtr, std::set<uint>>) {
            visited.insert(guard_y * row_sz + guard_x);
        } else {
            visited.push_front(guard_y * row_sz + guard_x);
        }
    } else {
        // face 90 degrees to right
        switch (direction) {
            case FLAG_FWD:
                direction = FLAG_RIGHT;
                break;
            case FLAG_BWD:
                direction = FLAG_LEFT;
                break;
            case FLAG_LEFT:
                direction = FLAG_FWD;
                break;
            case FLAG_RIGHT:
                direction = FLAG_BWD;
                break;
        }
    }
    return true;
}


int main() {
    std::fstream input_file(INPUT_LOC);
    std::string line;
    std::set<uint> visited{};
    std::list<uint> visited_mod{};
    size_t steps = 0;
    std::getline(input_file, line);
    uint guard_x, guard_y, next_x, next_y, start_x, start_y, max_sz;
    uint row = 0, n_loops = 0;
    uint8_t direction = FLAG_FWD;
    uint row_sz = line.size();
    do {
        for (uint i = 0; i < row_sz; i++) {
            if (line[i] == '.') {
                grid.push_back(true); // open
            } else {
                if (line[i] == '^') {
                    start_x = i;
                    start_y = row;
                    grid.push_back(true); // starting pos
                } else {
                    grid.push_back(false); // obstacle
                }
            }
        }
        row++;
    } while (std::getline(input_file, line));
    input_file.close();
    visited.insert(start_y * row_sz + start_x);
    max_sz = row * row_sz;

    // Save these for part 2
    guard_x = start_x;
    guard_y = start_y;

    while (move_guard(guard_x, guard_y, direction, next_x, next_y, visited, row_sz, max_sz)) {
        // Part 1
    }

    std::cout << "Number of rots: " << visited.size() << std::endl;

    // Now we test if any loops are made if we add a new obstacle along the visited nodes
    auto rc = visited.erase(visited.find(start_y * row_sz + start_x)); // cannot place obstacle at start
    if (rc == visited.end()) {
        std::cerr << "Warning: could not erase start position from visited nodes" << std::endl;
    }
    for (uint new_block: visited) {
        grid[new_block] = false;
        guard_x = start_x;
        guard_y = start_y;

        // reset direction
        direction = FLAG_FWD;

        while (move_guard(guard_x, guard_y, direction, next_x, next_y, visited_mod, row_sz, max_sz)) {
            steps += 1;

            if (steps % 10000 == 0) {
                if (is_repeating_subseq(std::vector<uint>(visited_mod.begin(), visited_mod.end()))) {
                    n_loops++;
                    break;
                }
                visited_mod.clear();
            }

        }

        // cleanup
        visited_mod.clear();
        grid[new_block] = true; // reset
        steps = 0;
    }

    std::cout << "Number of loops: " << n_loops << std::endl;
}