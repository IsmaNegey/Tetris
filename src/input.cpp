#include "input.h"

void handleBlockInput(int symb, int& x, const Shape& shape, int& side, int& time) {
    const int size = static_cast<int>(shape.block.size());

    if (symb == KEY_UP) {
        if (side < size - 1)
            side++;
        else
            side = 0;
    }
    else if (symb == KEY_DOWN) {
        time = 0;
    }
    else if (symb == KEY_LEFT && x > 1) {
        x -= 2;
    }
    else if (symb == KEY_RIGHT && x + static_cast<int>(shape.block[side][0].size()) < cfg::kScreenWidth - 1) {
        x += 2;
    }
}