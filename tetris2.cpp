#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>

std::vector<std::vector<char>> box(int width, int height) {

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

void draw(int y, int x, int side, std::vector<std::vector<std::vector<int>>>& shape, char ch) {
    for (int j = 0; j < shape[side].size(); ++j) {
        for (int k = 0; k < shape[side][j].size(); ++k) {
            if (shape[side][j][k])
                mvaddch(y + j, x + k, ch);
        }
    }
    refresh();   
}

void move(int y, int x, std::vector<std::vector<std::vector<int>>>& shape, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) {
    int i = 0;
    int size = shape.size();
    int time = 1;
    
    while(true) {
        
        int symb = getch();
        if (symb == KEY_UP)
            if (i < size - 1)
                i++;
            else
                i = 0;
        else if (symb == KEY_DOWN) 
            time = 0;
        else if (symb == KEY_LEFT && x > 2) {
            x -= 2;
        }
        else if (symb == KEY_RIGHT && x + shape[i][0].size() < 20) {
            x += 2;
        }
        draw(y, x, i, shape, '@');
        sleep(time);

        for (int z1 = 0; z1 < shape[i].size(); ++z1) {
            for (int z2 = 0; z2 < shape[i][z1].size(); ++z2) {
                if ((gamebox[y + z1 + 1][x + z2] == '@' && shape[i][z1][z2]) || y + shape[i].size() > gamebox.size() - 2) {
                    for (int j = 0; j < shape[i].size(); ++j) {
                        for (int k = 0; k < shape[i][j].size(); ++k) {
                            if (shape[i][j][k]) {
                                gamebox[y + j][x + k] = '@';
                                count[y + j]++;
                                if (count[y + j] == 20) {
                                    for (int len = 1; len < 21; ++len) {
                                        gamebox[y + j][len] = ' ';
                                        mvaddch(y + j, len, ' ');
                                    }
                                    mvprintw(2,2, "%d", count[y + j]);
                                    count[y + j] = 0;
                                }
                                
                            }
                        }
                    }
                    for (int i  = 0; i < 21; i++) {
                        mvprintw(i, 23, "%d", count[i]);
                    }
                    return;
                }
            }
        }
        draw(y, x, i, shape, ' ');
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

    std::vector<std::vector<char>> gamebox = box(field_width, field_height);
    std::vector<int> count(20, 0);

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
        
        int random = rand() % 7;
        move(2, 9, Shapes[random], gamebox, count);
        
    }
}

// g++ tetris2.cpp -lncurses -o tet && ./tet