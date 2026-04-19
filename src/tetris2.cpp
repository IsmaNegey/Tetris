#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>

std::vector<std::vector<char>> drawField(int width, int height) {

    std::vector<std::vector<char>> pr(height, std::vector<char>(width + 3, ' '));

    for (int i = 0; i < width; ++i) {
        pr[0][i] = '_';
        pr[height - 1][i] = '=';
    } 

    for (int i = 1; i < height; ++i) {
        pr[i][0] = '|';
        pr[i][width - 1] = '|';
    }
    
    return pr;
}

void drawBlock(int y, int x, std::vector<std::vector<int>>& shape, char ch) {
    for (size_t col = 0; col < shape.size(); ++col) {
        for (size_t row = 0; row < shape[col].size(); ++row) {
            if (shape[col][row])
                mvaddch(y + col, x + row, ch);
        }
    }
    refresh();   
}

bool hasCollision(int y, int x, std::vector<std::vector<int>>& shape, std::vector<std::vector<char>>& gamebox) {

    for (size_t col = 0; col < shape.size(); ++col) {
        for (size_t row = 0; row < shape[col].size(); ++row) {
            if ((gamebox[y + col + 1][x + row] == '@' && shape[col][row]) || y + shape.size() > gamebox.size() - 2) {
                return true;
            }
        }
    }

    return false;
}

void placeBlock(int y, int x, std::vector<std::vector<int>>& shape, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) {
    for (size_t j = 0; j < shape.size(); ++j) {
        for (size_t k = 0; k < shape[j].size(); ++k) {
            if (shape[j][k]) {
                gamebox[y + j][x + k] = '@';
                count[y + j]++;
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

void cleanLines(std::vector<std::vector<char>>& gamebox, const std::vector<int>& lines, std::vector<int>& count) {
    for (auto line : lines) {
        for (size_t len = 1; len < 21; ++len) {
            gamebox[line][len] = ' ';
            mvaddch(line, len, ' ');
        }
        count[line] = 0;
    }
}

void handleBlockInput(const int& symb, int& x, const std::vector<std::vector<std::vector<int>>>& shape, int& side, const int& size, int& time) {
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
    else if (symb == KEY_RIGHT && x + shape[side][0].size() < 20) {
        x += 2;
    }
}

void wrappingLine(int y, std::vector<std::vector<char>>& gamebox) {
    gamebox[y + 1] = gamebox[y];
    for (int i = 1; i < 21; ++i) {
        gamebox[y][i] = ' ';
        mvaddch(y, i, ' ');
        mvaddch(y + 1, i, gamebox[y + 1][i]);
    }
}

void wrapLines(int line, std::vector<std::vector<char>>& gamebox) {
    for (int i = line - 1; i > 0; --i) {
        wrappingLine(i, gamebox);
    }

}


void moveBlock(int y, int x, std::vector<std::vector<std::vector<int>>>& shape, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) {
    int side = 0;
    int size = shape.size();
    int time = 1;
    
    while(true) {
        
        int symb = getch();
        handleBlockInput(symb, x, shape, side, size, time);

        drawBlock(y, x, shape[side], '@');

        if (hasCollision(y, x, shape[side], gamebox)) {
            
            placeBlock(y, x, shape[side], gamebox, count);

            std::vector<int> lines;
            lines = checkLines(count);

            cleanLines(gamebox, lines, count);
            for (auto i : lines) {
                wrapLines(i, gamebox);
            }
            return;
        }

        for (int i  = 0; i < 21; i++) {
            mvprintw(i, 23, "%d", count[i]);
        }
        
        sleep(time);
        
        drawBlock(y, x, shape[side], ' ');
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
    cbreak();

    std::vector<std::vector<char>> gamebox = drawField(field_width, field_height);
    std::vector<int> count(21, 0);

    for (int i = 0; i < field_height; ++i) {
        for (int j = 0; j < field_width; ++j) {
            mvaddch(i, j, gamebox[i][j]);
        }
    }
    refresh();

    using Shape = std::vector<std::vector<std::vector<int>>>;

    Shape T {
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

    Shape I {
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

    Shape O {
        {
        {1,1,1,1},
        {1,1,1,1}
        } 
    };

    Shape L1 {
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

    Shape L2 {
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

    Shape Z1 {
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

    Shape Z2 {
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

    std::vector<Shape> Shapes= {T, I, O, L1, L2, Z1, Z2};

    srand(time(0));
    while (true) {
        
        int random = rand() % 5;
        moveBlock(2, 9, Shapes[random], gamebox, count);
    }
}