// (c) 2024 Aaron Janeiro Stone

#include <string>
#include <fstream>
#include <iostream>

#define INPUT_LOC ADVENT_24_DIR "/day_3/input.txt"


#define FLAG_IN_MUL (1 << 0) // true if in mul function call
#define FLAG_L_FILLED (1 << 1) // true if l was filled
#define FLAG_DONT (1 << 2) // true if don't was called
#define FLAG_DO_LOAD (1 << 3) // function name in buffer
#define FLAG_DONT_LOAD (1 << 4) // function name in buffer

// Only keep the FLAG_DONT flag if it was set
#define RESET_PROG buffer.erase(); flags = flags & FLAG_DONT

bool is_digit(char c){
    return c >= '0' && c <= '9';
}

int main() {
    std::ifstream input(INPUT_LOC);
    std::string buffer, line;
    uint8_t flags;
    int l, r, out = 0, out_switched = 0;

    while (std::getline(input, line)) {
        RESET_PROG;
        for (char c : line) {
            switch (c){
                case '(':
                    if (buffer == "mul"){
                        flags |= FLAG_IN_MUL;
                    }
                    else if (buffer == "do"){
                        flags |= FLAG_DO_LOAD;
                    }
                    else if (buffer == "don't"){
                        flags |= FLAG_DONT_LOAD;
                    }
                    buffer.erase();
                    break;

                case ',':
                    if (flags & FLAG_IN_MUL && flags & ~FLAG_L_FILLED){
                        l = std::stoi(buffer);
                    }
                    buffer.erase();
                    flags |= FLAG_L_FILLED;
                    break;

                case ')':
                    if (flags & FLAG_IN_MUL && flags & FLAG_L_FILLED){
                        r = std::stoi(buffer);
                        out += (l * r);

                        // only add to switched if don't flag is not set
                        if (!(flags & FLAG_DONT)){
                            out_switched += (l * r);
                        }
                    }
                    else if (flags & FLAG_DO_LOAD){
                        // turn off DON'T flag
                        flags = 0;
                    }
                    else if (flags & FLAG_DONT_LOAD){
                        // turn on DON'T flag
                        flags = FLAG_DONT;
                    }
                    RESET_PROG;
                    break;
                case 'm':
                    RESET_PROG;
                    buffer += c;
                    break;
                case 'u':
                    if (buffer == "m"){
                        buffer += c;
                    } else {
                        RESET_PROG;
                    }
                    break;
                case 'l':
                    if (buffer == "mu"){
                        buffer += c;
                    } else {
                        RESET_PROG;
                    }
                    break;

                case 'd':
                    RESET_PROG;
                    buffer = c;
                    break;
                case 'o':
                    if (buffer == "d"){
                        buffer += c;
                    } else {
                        RESET_PROG;
                    }
                    break;
                case 'n':
                    if (buffer == "do"){
                        buffer += c;
                    } else {
                        RESET_PROG;
                    }
                    break;
                case '\'':
                    if (buffer == "don"){
                        buffer += c;
                    } else {
                        RESET_PROG;
                    }
                    break;

                case 't':
                    if (buffer == "don'"){
                        buffer += c;
                    } else {
                        RESET_PROG;
                    }
                    break;
                default:
                    if (flags & FLAG_IN_MUL && is_digit(c)){
                        buffer += c;
                    }
                    else{
                        RESET_PROG;
                    }
                    break;
            }

        }
    }
    input.close();

    std::cout << "Without switch: " << out << std::endl;
    std::cout << "With switch: " << out_switched << std::endl;
}