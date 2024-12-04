// (c) 2024 Aaron Janeiro Stone

#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <iostream>
#include "imp.h"

#define INPUT_LOC ADVENT_24_DIR "/day_4/input.txt"

typedef std::vector<std::vector<char>> grid_t;



int main() {
    std::ifstream file(INPUT_LOC);
    std::queue<CharPos> work_p1; // Positions of X's
    std::queue<CharPos> work_p2; // Positions of A's
    std::string line;
    grid_t grid;
    char c;
    int max_y = 0;
    int max_x = 0;
    int out_1 = 0;
    int out_2 = 0;


    // Build grid
    std::getline(file, line);
    max_x = line.size();
    do {
        std::vector<char> row;
        for (int i = 0; i < max_x; i++) {
            c = line[i];
            row.push_back(c);
            if (c == 'X') {
                work_p1.push({i, max_y});
            }
            else if (c == 'A' && i > 0 && i < max_x - 1 && max_y > 0) {
                work_p2.push({i, max_y});
            }
        }
        grid.push_back(row);
        max_y++;
    } while (std::getline(file, line));
    file.close();

    CharPos maximal = {max_x - 1, max_y - 1};

    while (!work_p1.empty()) {
        CharPos pos = work_p1.front();
        work_p1.pop();
        adj_match_p1(out_1, grid, pos, maximal);
    }

    std::cout << "Part 1:" << out_1 << std::endl;

    while (!work_p2.empty()) {
        CharPos pos = work_p2.front();
        work_p2.pop();
        adj_match_p2(out_2, grid, pos, maximal);
    }

    std::cout << "Part 2:" << out_2 << std::endl;
}