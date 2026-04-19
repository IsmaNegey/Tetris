#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include "shape_struct.h"
#include "cell_struct.h"

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

void drawBlock(int y, int x, std::vector<std::vector<int>>& shape, char ch, int color) {
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

bool hasCollision(int y, int x, std::vector<std::vector<int>>& shape, std::vector<std::vector<Cell>>& gamebox) {

    for (size_t col = 0; col < shape.size(); ++col) {
        for (size_t row = 0; row < shape[col].size(); ++row) {
            if ((gamebox[y + col + 1][x + row].filled == '@' && shape[col][row]) || y + shape.size() > gamebox.size() - 2) {
                return true;
            }
        }
    }

    return false;
}

void placeBlock(int y, int x, int side, Shape& shape, std::vector<std::vector<Cell>>& gamebox, std::vector<int>& count) {
    for (size_t j = 0; j < shape.block[side].size(); ++j) {
        for (size_t k = 0; k < shape.block[side][j].size(); ++k) {
            if (shape.block[side][j][k]) {
                gamebox[y + j][x + k].filled = '@';
                gamebox[y + j][x + k].color = shape.color;
                count[y + j]++;
                mvaddch(3,3,'f');
                
            }   
        }
    }
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

void handleBlockInput(const int& symb, int& x, const Shape& shape, int& side, const int& size, int& time) {
    if (symb == KEY_UP)
        if (side < size - 1)
            side++;
        else
            side = 0;
    else if (symb == KEY_DOWN) 
        time = 0;
    else if (symb == KEY_LEFT && x > 2) {
        x -= 2;
    }
    else if (symb == KEY_RIGHT && x + shape.block[side][0].size() < 20) {
        x += 2;
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


void moveBlock(int y, int x, Shape& shape, std::vector<std::vector<Cell>>& gamebox, std::vector<int>& count) {
    int side = 0;
    int size = shape.block.size();
    int time = 1;
    
    while(true) {
        
        int symb = getch();
        handleBlockInput(symb, x, shape, side, size, time);

        drawBlock(y, x, shape.block[side], ' ', shape.color);

        if (hasCollision(y, x, shape.block[side], gamebox)) {
            
            placeBlock(y, x, side, shape, gamebox, count);

            std::vector<int> lines;
            lines = checkLines(count);

            cleanLines(gamebox, lines, count);
            for (auto i : lines) {
                wrapLines(i, gamebox, count);
            }
            return;
        }

        for (int i  = 0; i < 21; i++) {
            mvprintw(i, 23, "%d", count[i]);
        }
        
        sleep(time);
        
        drawBlock(y, x, shape.block[side], ' ', -1);
        y++;  
    }
}

int main() {
    constexpr int cell_w = 2;
    constexpr int cell_h = 1;
    constexpr int walls = 2;
    constexpr int top_bottom = 2;
    constexpr int field_width = 10 * cell_w + walls;
    constexpr int field_height = 20 * cell_h + top_bottom;
    initscr();
    nodelay(stdscr,TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    start_color();
    init_pair(1,COLOR_BLACK, COLOR_RED);
    init_pair(2,COLOR_BLACK, COLOR_GREEN);
    init_pair(3,COLOR_BLACK, COLOR_YELLOW);
    init_pair(4,COLOR_BLACK, COLOR_BLUE);
    init_pair(5,COLOR_BLACK, COLOR_MAGENTA);
    init_pair(6,COLOR_BLACK, COLOR_CYAN);
    init_pair(7,COLOR_BLACK, COLOR_WHITE);
    cbreak();

    

    std::vector<std::vector<Cell>> gamebox = drawField(field_width, field_height);
    std::vector<int> count(21, 0);

    for (int i = 0; i < field_height; ++i) {
        for (int j = 0; j < field_width; ++j) {
            mvaddch(i, j, gamebox[i][j].filled);
        }
    }
    refresh();

    Shape T;
    T.block = {
        {
        {0,0,1,1,0,0},
        {1,1,1,1,1,1}
        }, 
        {
        {1,1,0,0},
        {1,1,1,1},
        {1,1,0,0}
        }, 
        {
        {1,1,1,1,1,1}, 
        {0,0,1,1,0,0}
        },
        {
        {0,0,1,1},
        {1,1,1,1},
        {0,0,1,1}
        }

    };
    T.color = COLOR_MAGENTA;

    Shape I;
    I.block = {
        {
        {1,1,1,1,1,1,1,1}
        }, 
        {
        {1,1},
        {1,1},
        {1,1},
        {1,1}
        } 
    };
    I.color = COLOR_CYAN;

    Shape O;
    O.block = {
        {
        {1,1,1,1},
        {1,1,1,1}
        } 
    };
    O.color = COLOR_YELLOW;

    Shape L1;
    L1.block = {
        {
        {1,1,0,0},
        {1,1,0,0},
        {1,1,1,1}
        }, 
        {
        {1,1,1,1,1,1},
        {1,1,0,0,0,0}
        },
        {
        {1,1,1,1},
        {0,0,1,1},
        {0,0,1,1}
        },
        {
        {0,0,0,0,1,1},
        {1,1,1,1,1,1}
        }
    };
    L1.color = COLOR_WHITE;

    Shape L2;
    L2.block = {
        {
        {0,0,1,1},
        {0,0,1,1},
        {1,1,1,1}
        }, 
        {
        {1,1,1,1,1,1},
        {0,0,0,0,1,1}
        },
        {
        {1,1,1,1},
        {1,1,0,0},
        {1,1,0,0}
        },
        {
        {1,1,0,0,0,0},
        {1,1,1,1,1,1}
        }
    };
    L2.color = COLOR_BLUE;

    Shape Z1;
    Z1.block = {
        {
        {1,1,1,1,0,0},
        {0,0,1,1,1,1}
        },
        {
        {0,0,1,1},
        {1,1,1,1},
        {1,1,0,0} 
        }
    };
    Z1.color = COLOR_RED;

    Shape Z2;
    Z2.block = {
        {
        {0,0,1,1,1,1},
        {1,1,1,1,0,0}
        },
        {
        {1,1,0,0},
        {1,1,1,1},
        {0,0,1,1} 
        }
    };
    Z2.color = COLOR_GREEN;

    std::vector<Shape> Shapes= {T, I, O, L1, L2, Z1, Z2};

    srand(time(0));
    while (true) {
        int random = rand() % 5;
        moveBlock(2, 9, Shapes[random], gamebox, count);
    }
}
