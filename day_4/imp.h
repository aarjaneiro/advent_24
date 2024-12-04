// (c) 2024 Aaron Janeiro Stone

#ifndef ADVENT_24_IMP_H
#define ADVENT_24_IMP_H

// Implementations of scanning functions for Advent of Code 2024 Day 4

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>

struct CharPos {
    int x, y;
};

enum MatchDir {
    NA = 0, N, NE, E, SE, S, SW, W, NW
};


void adj_match_p1(int &count, const std::vector<std::vector<char>> &grid, const CharPos pos, const CharPos &maximal,
                  char to_match = 'M',
                  MatchDir restrict_dir = NA) {
    // Check for restriction on direction
    if (restrict_dir == MatchDir::NA) {
        for (int i = MatchDir::N; i <= MatchDir::NW; i++) {
            adj_match_p1(count, grid, pos, maximal, to_match, (MatchDir) i); // Recurse
        }
        return;
    }

    // Determine next character to match
    char next_match;
    switch (to_match) {
        case 'M':
            next_match = 'A';
            break;
        case 'A':
            next_match = 'S';
            break;
        case 'S': // Terminal (exit recursion)
            break;
        default:
            return;
    }

    // Check if next position is within bounds
    CharPos next_pos = pos;
    switch (restrict_dir) {
        case MatchDir::N:
            next_pos.y--;
            break;
        case MatchDir::NE:
            next_pos.y--;
            next_pos.x++;
            break;
        case MatchDir::E:
            next_pos.x++;
            break;
        case MatchDir::SE:
            next_pos.y++;
            next_pos.x++;
            break;
        case MatchDir::S:
            next_pos.y++;
            break;
        case MatchDir::SW:
            next_pos.y++;
            next_pos.x--;
            break;
        case MatchDir::W:
            next_pos.x--;
            break;
        case MatchDir::NW:
            next_pos.y--;
            next_pos.x--;
            break;
        case MatchDir::NA: // Should not happen
        default:
            return;
    }
    if (next_pos.x < 0 || next_pos.y < 0 || next_pos.x > maximal.x || next_pos.y > maximal.y) {
        return;
    }
    if (grid[next_pos.y][next_pos.x] != to_match) {
        return;
    }
    if (to_match == 'S') {
        count++;
        return; // Terminal
    }
    adj_match_p1(count, grid, next_pos, maximal, next_match, restrict_dir);
}


void adj_match_p2(int &count, const std::vector<std::vector<char>> &grid, const CharPos pos, const CharPos &maximal) {
    if (pos.y >= maximal.y) {
        return; // need one row below
    }
    char nw, ne, sw, se;
    nw = grid[pos.y - 1][pos.x - 1];
    se = grid[pos.y + 1][pos.x + 1];
    if (!((nw == 'M' && se == 'S') || nw == 'S' && se == 'M')) {
        return;
    }
    ne = grid[pos.y - 1][pos.x + 1];
    sw = grid[pos.y + 1][pos.x - 1];
    if (!((ne == 'M' && sw == 'S') || ne == 'S' && sw == 'M')) {
        return;
    }
    count++;
}

#endif //ADVENT_24_IMP_H
