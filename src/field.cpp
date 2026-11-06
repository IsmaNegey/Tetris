#include "field.h"

std::vector<std::vector<Cell>> drawField(int width, int height) {

    std::vector<std::vector<Cell>> pr(height, std::vector<Cell>(width + 3, {' ', 0}));

    for (int i = 0; i < width; ++i) {
        pr[0][i].filled = '_';
        pr[height - 1][i].filled = '=';
    } 

    for (int i = 1; i < height; ++i) {
        pr[i][0].filled = '|';
        pr[i][width - 1].filled = '|';
    }
    
    return pr;
}

std::vector<int> checkLines(const std::vector<int>& count) {
    
    std::vector<int> lines;
    for (size_t i = 0; i < count.size(); ++i) {
        if (count[i] == 20) {
            lines.push_back(i);
        }
    }
    return lines;
}

void cleanLines(std::vector<std::vector<Cell>>& gamebox, const std::vector<int>& lines, std::vector<int>& count) {
    for (auto line : lines) {
        for (size_t len = 1; len < 21; ++len) {
            gamebox[line][len].filled = ' ';
            gamebox[line][len].color = 0;
            mvaddch(line, len, ' ');
        }
        count[line] = 0;
    }
}

void wrappingLine(int y, std::vector<std::vector<Cell>>& gamebox, std::vector<int>& count) {
    gamebox[y + 1] = gamebox[y];
    count[y + 1] = count[y];
    for (int i = 1; i < 21; ++i) {

        gamebox[y][i].filled = ' ';
        gamebox[y][i].color = 0;
        mvaddch(y, i, ' ');

        if (gamebox[y + 1][i].filled){
            attron(COLOR_PAIR(gamebox[y + 1][i].color));
            mvaddch(y + 1, i, ' ');
            attroff(COLOR_PAIR(gamebox[y + 1][i].color));
        }
    }
}

void wrapLines(int line, std::vector<std::vector<Cell>>& gamebox, std::vector<int>& count) {
    for (int i = line - 1; i > 0; --i) {
        wrappingLine(i, gamebox, count);
    }
}