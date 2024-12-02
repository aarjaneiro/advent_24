// (c) 2024 Aaron Janeiro Stone

#include <string>
#include <fstream>
#include <iostream>

#define INPUT_LOC ADVENT_24_DIR "/day_2/input.txt"

// Flags
#define FLAG_IS_INIT (1 << 0)
#define FLAG_ORDERED (1 << 1)
#define FLAG_IS_INCREASING (1 << 2)
#define FLAG_IS_DECREASING (1 << 3)
#define FLAG_DAMPED (1 << 4)

#define SKIP_LINE flags = 0; break;

size_t n_spaces(const std::string &line) {
    size_t n = 0;
    for (char c: line) {
        if (c == ' ') {
            n++;
        }
    }
    return n;
}

void extractor(uint8_t &flags, size_t line_sz, const std::string &line, int skip_idx = -1) {
    int diff = 0;
    uint8_t increasing = 0;
    std::string number_buffer;
    int current = 0, last = 0;
    size_t processed = 0;
    for (size_t i = 0; i < line_sz; i++) {
        char c = line[i];
        if (c == ' ' || i == line_sz - 1) {
            if (c != ' ') {
                number_buffer.push_back(c);
            }
            if (number_buffer.empty()) {
                continue;
            }
            current = std::stoi(number_buffer);
            number_buffer.clear();
            processed++;
            if (skip_idx >= 0 && processed == (skip_idx + 1)) {
                continue;
            }

            // First number in line
            if (flags == 0 || flags == FLAG_DAMPED) {
                last = current;
                flags |= FLAG_IS_INIT;
                continue;
            }

            // Not first - we can check order and differences
            increasing = current > last ? FLAG_IS_INCREASING : FLAG_IS_DECREASING;
            if (flags == (flags | FLAG_ORDERED) && flags != (flags | increasing)) {
                SKIP_LINE;
            }

            // abs(abs(diff) - 2) should be at most 1
            diff = abs(abs(current - last) - 2);
            if (diff > 1) {
                SKIP_LINE;
            }

            // Current number passes checks
            flags |= increasing | FLAG_ORDERED;
            last = current;

        } else {
            number_buffer.push_back(c);
        }
    }
}

bool assess(uint8_t &flags, uint &n_safe, uint &n_safe_damped) {
    if (flags != 0) {
        if (flags != (flags | FLAG_DAMPED)) {
            n_safe++;
        }
        n_safe_damped++;
        return true;
    }
    return false;
}

int main() {
    std::ifstream file(INPUT_LOC);
    uint8_t flags = 0;
    uint n_safe = 0, n_safe_damped = 0;
    int skip_idx;
    size_t line_sz, n_numbers;
    std::string line, line_buffer;

    // Find number of safe lines
    while (std::getline(file, line)) {
        flags = 0;
        skip_idx = -1;
        line_sz = line.size();
        n_numbers = n_spaces(line) + 1;
        extractor(flags, line_sz, line, skip_idx);

        // Check if line is safe. If not, check if it is safe after damping.
        if (assess(flags, n_safe, n_safe_damped)) continue;

        // Definitely can be optimized...
        for (int i = 0; i < n_numbers; i++) {
            skip_idx = i;
            flags = FLAG_DAMPED;
            extractor(flags, line_sz, line, skip_idx);
            if (assess(flags, n_safe, n_safe_damped)) break;
        }
    }
    file.close();
    std::cout << "Number of safe lines (No Damper): " << n_safe << std::endl;
    std::cout << "Number of safe lines (With Damper): " << n_safe_damped << std::endl;

}
