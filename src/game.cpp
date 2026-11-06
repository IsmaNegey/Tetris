#include "game.h"

void moveBlock(int y, int x, Shape& shape, Field& gamebox, Counts& count) {
    int side = 0;
    int fallTimer = 0;
    constexpr int kFallDelay = 60;

    timeout(16);

    while (true) {
        int symb = getch();
        if (symb != ERR) {
            if (symb == KEY_UP) {
                side++;

                if (side >= static_cast<int>(shape.block.size()))
                    side = 0;
            }
            else if (symb == KEY_DOWN)
                fallTimer = kFallDelay;
            else if (symb == KEY_LEFT && x > 1)
                x -= 2;
            else if (symb == KEY_RIGHT &&
                     x + static_cast<int>(shape.block[side][0].size()) < cfg::kScreenWidth - 1)
                x += 2;
        }
        fallTimer++;
        bool needFall = (fallTimer >= kFallDelay);
        int nextY = y + (needFall ? 1 : 0);
        if (hasCollision(nextY, x, shape.block[side], gamebox)) {
            placeBlock(y, x, side, shape, gamebox, count);

            auto lines = checkLines(count);
            cleanLines(gamebox, lines, count);
            for (auto i : lines)
                wrapLines(i, gamebox, count);
            return;
        }

        if (needFall) {
            y = nextY;
            fallTimer = 0;
        }
        erase();
        renderField(gamebox);

        drawBlock(y, x, shape.block[side], ' ', shape.color);
        refresh();
    }
}
