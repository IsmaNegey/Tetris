#include "render.h"

void drawBlock(int y, int x, const std::vector<std::vector<int>>& shape, char ch, int color) {
    for (size_t col = 0; col < shape.size(); ++col) {
        for (size_t row = 0; row < shape[col].size(); ++row) {
            if (shape[col][row]) {
                attron(COLOR_PAIR(color));
                mvaddch(y + col, x + row, ch);
                attroff(COLOR_PAIR(color));
            }
        }
    }
    refresh();   
}

void renderField(const Field& gamebox) {
    for (int i = 0; i < cfg::kScreenHeight; ++i) {
        for (int j = 0; j < cfg::kScreenWidth; ++j) {
            const Cell& cell = gamebox[i][j];

            if (cell.filled == '@') {
                mvaddch(i, j, ' ' | COLOR_PAIR(cell.color));
            } else {
                mvaddch(i, j, cell.filled);
            }
        }
    }
    refresh();
}