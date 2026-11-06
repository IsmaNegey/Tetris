#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include "shape.h"
#include "cell.h"
#include "shapes_factory.h"
#include "config.h"
#include "field.h"
#include "game.h"
#include "input.h"
#include "render.h"
#include "collision.h"

static void initNcurses() {
    initscr();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    cbreak();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_BLACK, COLOR_BLUE);
    init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(6, COLOR_BLACK, COLOR_CYAN);
    init_pair(7, COLOR_BLACK, COLOR_WHITE);
}

int main() {

    initNcurses();
    std::vector<std::vector<Cell>> gamebox = drawField(cfg::kScreenWidth, cfg::kScreenHeight);
    std::vector<int> count(21, 0);

    renderField(gamebox);

    std::vector<Shape> Shapes = makeShapes();

    srand(time(0));
    while (true) {
        int random = rand() % 7;
        moveBlock(cfg::kStartY, cfg::kStartX, Shapes[random], gamebox, count);
    }
}