#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <ctime>

struct Shape {
    short corner = 0;
    short time = 1;
    virtual void draw (int y, int x, char ch) = 0;
    virtual void move_right(const int& y, int& x, std::vector<std::vector<char>>& gamebox) = 0;
    virtual void move_left(const int& y, int& x, std::vector<std::vector<char>>& gamebox) = 0;
    virtual void frame (int y, int x) {
        draw(y, x, '@');
        refresh();
        sleep(time);
        draw(y, x, ' ');
        refresh();
    };
    virtual bool check_the_touch(int y, int x, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) = 0;
    virtual void transfer(int y, int x, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) = 0;
    virtual void turn() { 
        corner += 90;
        if (corner == 360)
            corner = 0;
    }
    virtual void boost() {
        if (time == 1)
            time = 0;
        else
            time = 1;
    }
    virtual ~Shape() = default;
};

struct Shape_one: public Shape {
    void draw(int y, int x, char ch) override {
        mvaddch(y, x, ch);
        mvaddch(y, x + 1, ch);
        mvaddch(y, x + 2, ch);
        mvaddch(y, x + 3, ch);
        mvaddch(y + 1, x, ch);
        mvaddch(y + 1, x + 1, ch);
        mvaddch(y + 1, x + 2, ch);
        mvaddch(y + 1, x + 3, ch); 
    }

    void move_left(const int& y, int& x, std::vector<std::vector<char>>& gamebox) override {
        if (gamebox[y][x - 1] != '|' && gamebox[y][x - 1] != '@' && gamebox[y + 1][x - 1] != '@') 
            x -= 2;
        else
            mvprintw(0, 1, "Left");
    }

    void move_right(const int& y, int& x, std::vector<std::vector<char>>& gamebox) override {
        if (gamebox[y][x + 4] != '|' && gamebox[y][x + 4] != '@' && gamebox[y + 1][x + 4] != '@') 
            x += 2;
        else
            mvprintw(0, 1, "Right");
    }

    void transfer(int y, int x, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) override {
        gamebox[y][x] = '@';
        gamebox[y][x + 1] = '@';
        gamebox[y][x + 2] = '@';
        gamebox[y][x + 3] = '@';
        gamebox[y + 1][x] = '@';
        gamebox[y + 1][x + 1] = '@';
        gamebox[y + 1][x + 2] = '@';
        gamebox[y + 1][x + 3] = '@';

        count[y] += 2;
        count[y + 1] += 2;
    }

    bool check_the_touch(int y, int x, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) override {
        if (gamebox[y + 2][x] == '=' || gamebox[y + 2][x] == '@' || gamebox[y + 2][x + 1] == '@' || gamebox[y + 2][x + 2] == '@' || gamebox[y + 2][x + 3] == '@') {
            this->transfer(y, x, gamebox, count);
            return true; 
        }
        return false;
    }

};

struct Shape_two: public Shape {
    void draw(int y, int x, char ch) override {
        
        if (corner == 0 || corner == 180){
            mvaddch(y, x, ch);
            mvaddch(y, x + 1, ch);
            mvaddch(y, x + 2, ch);
            mvaddch(y, x + 3, ch);
            mvaddch(y + 1, x + 2, ch);
            mvaddch(y + 1, x + 3, ch);
            mvaddch(y + 1, x + 4, ch);
            mvaddch(y + 1, x + 5, ch);
        }

        if (corner == 90 || corner == 270) {
            mvaddch(y, x, ch);
            mvaddch(y, x + 1, ch);
            mvaddch(y, x + 2, ch);
            mvaddch(y, x + 3, ch);
            mvaddch(y + 1, x, ch);
            mvaddch(y + 1, x + 1, ch);
            mvaddch(y - 1, x + 2, ch);
            mvaddch(y - 1, x + 3, ch);
        }
    }

    void move_left(const int& y, int& x, std::vector<std::vector<char>>& gamebox) override {
        
        if (corner == 0 || corner == 180) {
            if (gamebox[y][x - 1] != '|' && gamebox[y][x - 1] != '@' && gamebox[y + 1][x + 1] != '@') 
                x -= 2;
        }
        if (corner == 90 || corner == 270) {
            if (gamebox[y][x - 1] != '|' && gamebox[y][x - 1] != '@' && gamebox[y + 1][x - 1] != '@' && gamebox[y + 2][x + 1] != '|') 
                x -= 2;
        }
    }

    void move_right(const int& y, int& x, std::vector<std::vector<char>>& gamebox) override {
        if (corner == 0 || corner == 180) {
            if (gamebox[y + 1][x + 6] != '|' && gamebox[y + 1][x + 6] != '@' && gamebox[y][x + 4] != '@') 
                x += 2;
        }

        if (corner == 90 || corner == 270) {
            if (gamebox[y][x + 4] != '|' && gamebox[y][x + 4] != '@' && gamebox[y - 1][x + 4] != '@' && gamebox[y + 1][x + 2] != '@') 
                x += 2;
        }
    }

    void transfer(int y, int x, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) override {
        if (corner == 0 || corner == 180) {
            gamebox[y][x] = '@';
            gamebox[y][x + 1] = '@';
            gamebox[y][x + 2] = '@';
            gamebox[y][x + 3] = '@';
            gamebox[y + 1][x + 2] = '@';
            gamebox[y + 1][x + 3] = '@';
            gamebox[y + 1][x + 4] = '@';
            gamebox[y + 1][x + 5] = '@';

            count[y] += 2;
            count[y + 1] += 2;
        }
        if (corner == 90 || corner == 270) {
            gamebox[y] [x] = '@';
            gamebox[y] [x + 1] = '@';
            gamebox[y] [x + 2] = '@';
            gamebox[y] [x + 3] = '@';
            gamebox[y + 1] [x] = '@';
            gamebox[y + 1] [x + 1] = '@';
            gamebox[y - 1] [x + 2] = '@';
            gamebox[y - 1] [x + 3] = '@';

            count[y] += 2;
            count[y + 1] += 1;
            count[y - 1] += 1;
        }
    }

    bool check_the_touch(int y, int x, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) override {
        if (corner == 0 || corner == 180) {
        
            if (gamebox[y + 2][x + 2] == '=' || gamebox[y + 1][x] == '@' || gamebox[y + 1][x + 1] == '@' || gamebox[y + 2][x + 2] == '@' || gamebox[y + 2][x + 3] == '@' || gamebox[y + 2][x + 4] == '@' || gamebox[y + 2][x + 5] == '@') {
                this->transfer(y, x, gamebox, count); //поменять условие - одного же хватит (?)
                return true; 
            }
            return false;
        }

        else { //if (corner == 90 || corner == 270)

            if (gamebox[y + 2][x] == '=' || gamebox[y + 1][x + 2] == '@' || gamebox[y + 1][x + 3] == '@' || gamebox[y + 2][x] == '@' || gamebox[y + 2][x + 1] == '@') {
                this->transfer(y, x, gamebox, count);
                return true; 
            }
            
            return false;
        }
    }
};

struct Shape_three: public Shape {
    void draw(int y, int x, char ch) override {
        if (corner == 0 || corner == 180) {
            mvaddch(y, x, ch);
            mvaddch(y, x + 1, ch);
            mvaddch(y, x + 2, ch);
            mvaddch(y, x + 3, ch);
            mvaddch(y + 1, x, ch);
            mvaddch(y + 1, x + 1, ch);
            mvaddch(y + 1, x - 1, ch);
            mvaddch(y + 1, x - 2, ch);
        }

        if (corner == 90 || corner == 270) {
            mvaddch(y, x, ch);
            mvaddch(y, x + 1, ch);
            mvaddch(y + 1, x, ch);
            mvaddch(y + 1, x + 1, ch);
            mvaddch(y + 1, x + 2, ch);
            mvaddch(y + 1, x + 3, ch);
            mvaddch(y + 2, x + 2, ch);
            mvaddch(y + 2, x + 3, ch);
        }
    }

    void move_left(const int& y, int& x, std::vector<std::vector<char>>& gamebox) override {
        if (corner == 0 || corner == 180) {
            if (gamebox[y + 1][x - 3] != '|' && gamebox[y + 1][x - 3] != '@' && gamebox[y][x - 1] != '@') 
                x -= 2;
        }
        if (corner == 90 || corner == 270) {
            if (gamebox[y][x - 1] != '|' && gamebox[y][x - 1] != '@' && gamebox[y + 1][x - 1] != '@' && gamebox[y - 1][x + 1] != '|') 
                x -= 2;
        }
    }

    void move_right(const int& y, int& x, std::vector<std::vector<char>>& gamebox) override {
        if (corner == 0 || corner == 180) {
            if (gamebox[y][x + 4] != '|' && gamebox[y][x + 4] != '@' && gamebox[y + 1][x + 2] != '@') 
                x += 2;
        }

        if (corner == 90 || corner == 270) {
            if (gamebox[y - 1][x + 4] != '|' && gamebox[y - 1][x + 4] != '@' && gamebox[y - 2][x + 4] != '@' && gamebox[y][x + 2] != '@') 
                x += 2;
        }
    }

    void transfer(int y, int x, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) override {
        if (corner == 0 || corner == 180) {
            gamebox[y][x] = '@';
            gamebox[y][x + 1] = '@';
            gamebox[y][x + 2] = '@';
            gamebox[y][x + 3] = '@';
            gamebox[y + 1][x] = '@';
            gamebox[y + 1][x + 1] = '@';
            gamebox[y + 1][x - 1] = '@';
            gamebox[y + 1][x - 2] = '@';

            count[y] += 2;
            count[y + 1] += 2;
        }
        if (corner == 90 || corner == 270) {
            gamebox[y][x] = '@';
            gamebox[y][x + 1] = '@';
            gamebox[y + 1][x] = '@';
            gamebox[y + 1][x + 1] = '@';
            gamebox[y + 1][x + 2] = '@';
            gamebox[y + 1][x + 3] = '@';
            gamebox[y + 2][x + 2] = '@';
            gamebox[y + 2][x + 3] = '@';

            count[y] += 1;
            count[y + 1] += 2;
            count[y + 2] += 1;
        }
    }
    
    bool check_the_touch(int y, int x, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) override {
        if (corner == 0 || corner == 180) {
        
            if (gamebox[y + 2][x] == '=' || gamebox[y + 2][x - 2] == '@' || gamebox[y + 2][x - 1] == '@' || gamebox[y + 2][x] == '@' || gamebox[y + 2][x + 1] == '@' || gamebox[y][x + 2] == '@' || gamebox[y][x + 3] == '@') {
                this->transfer(y, x, gamebox, count);
                return true; 
            }
            return false;
        }

        else { //if (corner == 90 || corner == 270)

            if (gamebox[y + 3][x + 2] == '=' || gamebox[y + 2][x] == '@' || gamebox[y + 2][x + 1] == '@' || gamebox[y + 3][x + 2] == '@' || gamebox[y + 3][x + 3] == '@') {
                this->transfer(y, x, gamebox, count);
                return true; 
            }
            return false;
        }
    }
};

struct Shape_four: public Shape {
    void draw(int y, int x, char ch) override {
        if (corner == 0) {
            mvaddch(y, x, ch);
            mvaddch(y, x + 1, ch);
            mvaddch(y + 1, x, ch);
            mvaddch(y + 1, x + 1, ch);
            mvaddch(y + 2, x, ch);
            mvaddch(y + 2, x + 1, ch);
            mvaddch(y + 2, x - 1, ch);
            mvaddch(y + 2, x - 2, ch);
        }
        else if (corner == 90) {
            mvaddch(y, x, ch);
            mvaddch(y, x + 1, ch);
            mvaddch(y + 1, x, ch);
            mvaddch(y + 1, x + 1, ch);
            mvaddch(y + 1, x + 2, ch);
            mvaddch(y + 1, x + 3, ch);
            mvaddch(y + 1, x + 4, ch);
            mvaddch(y + 1, x + 5, ch);
        }
        else if (corner == 180) {
            mvaddch(y, x, ch);
            mvaddch(y, x + 1, ch);
            mvaddch(y, x + 2, ch);
            mvaddch(y, x + 3, ch);
            mvaddch(y + 1, x, ch);
            mvaddch(y + 1, x + 1, ch);
            mvaddch(y + 2, x, ch);
            mvaddch(y + 2, x + 1, ch);  
        }
        else if (corner == 270) {
            mvaddch(y, x, ch);
            mvaddch(y, x + 1, ch);
            mvaddch(y, x + 2, ch);
            mvaddch(y, x + 3, ch);
            mvaddch(y, x + 4, ch);
            mvaddch(y, x + 5, ch);
            mvaddch(y + 1, x + 4, ch);
            mvaddch(y + 1, x + 5, ch);
        }
    }

    void move_left(const int& y, int& x, std::vector<std::vector<char>>& gamebox) override {
        
        if (corner == 0) {
            if (gamebox[y - 2][x - 3] != '|' && gamebox[y - 2][x - 3] != '@' && gamebox[y][x - 1] != '@' && gamebox[y + 1][x - 1] != '@') 
                x -= 2;
        }
        if (corner == 90) {
            if (gamebox[y][x - 1] != '|' && gamebox[y][x - 1] != '@' && gamebox[y + 1][x - 1] != '@') 
                x -= 2;
        }
        if (corner == 180) {
            if (gamebox[y][x - 1] != '|' && gamebox[y][x - 1] != '@' && gamebox[y + 1][x - 1] != '@' && gamebox[y + 2][x - 1] != '@') 
                x -= 2;
        }
        if (corner == 270) {
            if (gamebox[y][x - 1] != '|' && gamebox[y][x - 1] != '@' && gamebox[y + 1][x + 3] != '@') 
                x -= 2;
        }
    }

    void move_right(const int& y, int& x, std::vector<std::vector<char>>& gamebox) override {
        if (corner == 0) {
            if (gamebox[y][x + 2] != '|' && gamebox[y][x + 2] != '@' && gamebox[y + 1][x + 2] != '@' && gamebox[y + 2][x + 2] != '@') 
                x += 2;
        }
        else if (corner == 90) {
            if (gamebox[y + 1][x + 6] != '|' && gamebox[y + 1][x + 6] != '@' && gamebox[y][x + 2] != '@') 
                x += 2;
        }
        else if (corner == 180) {
            if (gamebox[y][x + 4] != '|' && gamebox[y][x + 4] != '@' && gamebox[y + 1][x + 2] != '@' && gamebox[y + 2][x + 2] != '@') 
                x += 2;
        }
        else if (corner == 270) {
            if (gamebox[y][x + 6] != '|' && gamebox[y][x + 6] != '@' && gamebox[y + 1][x + 6] != '@') 
                x += 2;
        }
    }
    
    void transfer(int y, int x, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) override {
        if (corner == 0) {
            gamebox[y][x] = '@';
            gamebox[y][x + 1] = '@';
            gamebox[y + 1][x] = '@';
            gamebox[y + 1][x + 1] = '@';
            gamebox[y + 2][x] = '@';
            gamebox[y + 2][x + 1] = '@';
            gamebox[y + 2][x - 1] = '@';
            gamebox[y + 2][x - 2] = '@';

            count[y] += 1;
            count[y + 1] += 1;
            count[y + 2] += 2;

        }
        else if (corner == 90) {
            gamebox[y][x] = '@';
            gamebox[y][x + 1] = '@';
            gamebox[y + 1][x] = '@';
            gamebox[y + 1][x + 1] = '@';
            gamebox[y + 1][x + 2] = '@';
            gamebox[y + 1][x + 3] = '@';
            gamebox[y + 1][x + 4] = '@';
            gamebox[y + 1][x + 5] = '@';

            count[y] += 1;
            count[y + 1] += 3;
        }
        else if (corner == 180) {
            gamebox[y][x] = '@';
            gamebox[y][x + 1] = '@';
            gamebox[y][x + 2] = '@';
            gamebox[y][x + 3] = '@';
            gamebox[y + 1][x] = '@';
            gamebox[y + 1][x + 1] = '@';
            gamebox[y + 2][x] = '@';
            gamebox[y + 2][x + 1] = '@';

            count[y] += 2;
            count[y + 1] += 1;
            count[y + 2] += 1;
        }
        else if (corner == 270) {
            gamebox[y][x] = '@';
            gamebox[y][x + 1] = '@';
            gamebox[y][x + 2] = '@';
            gamebox[y][x + 3] = '@';
            gamebox[y][x + 4] = '@';
            gamebox[y][x + 5] = '@';
            gamebox[y + 1][x + 4] = '@';
            gamebox[y + 1][x + 5] = '@';

            count[y] += 3;
            count[y + 1] += 1;
        }
    }

    bool check_the_touch(int y, int x, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) override {
        if (corner == 0) {
        
            if (gamebox[y + 3][x] == '=' || gamebox[y + 3][x] == '@' || gamebox[y + 3][x - 1] == '@') {
                this->transfer(y, x, gamebox, count);
                return true; 
            }
            return false;
        }
        else if (corner == 90){

            if (gamebox[y + 2][x] == '=' || gamebox[y + 2][x] == '@' || gamebox[y + 2][x + 2] == '@' || gamebox[y + 2][x + 4] == '@') {
                this->transfer(y, x, gamebox, count);
                return true; 
            }
            return false;
        }
        else if (corner == 180){

            if (gamebox[y + 3][x] == '=' || gamebox[y + 3][x] == '@' || gamebox[y + 1][x + 2] == '@') {
                this->transfer(y, x, gamebox, count);
                return true; 
            }
            return false;
        }
        else{ //corner == 270

            if (gamebox[y + 2][x + 4] == '=' || gamebox[y + 2][x + 4] == '@' || gamebox[y + 1][x] == '@' || gamebox[y + 1][x + 2] == '@') {
                this->transfer(y, x, gamebox, count);
                return true; 
            }
            return false;
        }
    }
};

struct Shape_five: public Shape {
    void draw(int y, int x, char ch) override {
        if (corner == 0) {
            mvaddch(y, x, ch);
            mvaddch(y, x + 1, ch);
            mvaddch(y + 1, x, ch);
            mvaddch(y + 1, x + 1, ch);
            mvaddch(y + 2, x, ch);
            mvaddch(y + 2, x + 1, ch);
            mvaddch(y + 2, x + 2, ch);
            mvaddch(y + 2, x + 3, ch);
        }
        else if (corner == 90) {
            mvaddch(y, x, ch);
            mvaddch(y, x + 1, ch);
            mvaddch(y + 1, x, ch);
            mvaddch(y + 1, x + 1, ch);
            mvaddch(y, x + 2, ch);
            mvaddch(y, x + 3, ch);
            mvaddch(y, x + 4, ch);
            mvaddch(y, x + 5, ch);
        }
        else if (corner == 180) {
            mvaddch(y, x, ch);
            mvaddch(y, x + 1, ch);
            mvaddch(y, x - 1, ch);
            mvaddch(y, x - 2, ch);
            mvaddch(y + 1, x, ch);
            mvaddch(y + 1, x + 1, ch);
            mvaddch(y + 2, x, ch);
            mvaddch(y + 2, x + 1, ch);  
        }
        else if (corner == 270) {
            mvaddch(y, x, ch);
            mvaddch(y, x + 1, ch);
            mvaddch(y, x + 2, ch);
            mvaddch(y, x + 3, ch);
            mvaddch(y, x + 4, ch);
            mvaddch(y, x + 5, ch);
            mvaddch(y - 1, x + 4, ch);
            mvaddch(y - 1, x + 5, ch);
        }
    }

    void move_left(const int& y, int& x, std::vector<std::vector<char>>& gamebox) override {
        
        if (corner == 0) {
            if (gamebox[y][x - 1] != '|' && gamebox[y][x - 1] != '@' && gamebox[y - 1][x - 1] != '@' && gamebox[y - 2][x - 1] != '@') 
                x -= 2;
        }
        if (corner == 90) {
            if (gamebox[y][x - 1] != '|' && gamebox[y][x - 1] != '@' && gamebox[y + 1][x - 1] != '@') 
                x -= 2;
        }
        if (corner == 180) {
            if (gamebox[y][x - 3] != '|' && gamebox[y][x - 3] != '@' && gamebox[y + 1][x - 1] != '@' && gamebox[y + 2][x - 1] != '@') 
                x -= 2;
        }
        if (corner == 270) {
            if (gamebox[y][x - 1] != '|' && gamebox[y][x - 1] != '@' && gamebox[y + 1][x + 3] != '@') 
                x -= 2;
        }
    }

    void move_right(const int& y, int& x, std::vector<std::vector<char>>& gamebox) override {
        if (corner == 0) {
            if (gamebox[y + 2][x + 4] != '|' && gamebox[y + 2][x + 4] != '@' && gamebox[y][x + 2] != '@' && gamebox[y + 1][x + 2] != '@') 
                x += 2;
        }
        else if (corner == 90) {
            if (gamebox[y][x + 6] != '|' && gamebox[y][x + 6] != '@' && gamebox[y + 1][x + 2] != '@') 
                x += 2;
        }
        else if (corner == 180) {
            if (gamebox[y][x + 2] != '|' && gamebox[y][x + 2] != '@' && gamebox[y + 1][x + 2] != '@' && gamebox[y + 2][x + 2] != '@') 
                x += 2;
        }
        else if (corner == 270) {
            if (gamebox[y][x + 6] != '|' && gamebox[y][x + 6] != '@' && gamebox[y + 1][x + 6] != '@') 
                x += 2;
        }
    }
    
    void transfer(int y, int x, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) override {
        if (corner == 0) {
            gamebox[y][x] = '@';
            gamebox[y][x + 1] = '@';
            gamebox[y + 1][x] = '@';
            gamebox[y + 1][x + 1] = '@';
            gamebox[y + 2][x] = '@';
            gamebox[y + 2][x + 1] = '@';
            gamebox[y + 2][x + 2] = '@';
            gamebox[y + 2][x + 3] = '@';

            count[y] += 1;
            count[y + 1] += 1;
            count[y + 2] += 2;
        }
        else if (corner == 90) {
            gamebox[y][x] = '@';
            gamebox[y][x + 1] = '@';
            gamebox[y + 1][x] = '@';
            gamebox[y + 1][x + 1] = '@';
            gamebox[y][x + 2] = '@';
            gamebox[y][x + 3] = '@';
            gamebox[y][x + 4] = '@';
            gamebox[y][x + 5] = '@';

            count[y] += 3;
            count[y + 1] += 1;
        }
        else if (corner == 180) {
            gamebox[y][x] = '@';
            gamebox[y][x + 1] = '@';
            gamebox[y][x - 1] = '@';
            gamebox[y][x - 2] = '@';
            gamebox[y + 1][x] = '@';
            gamebox[y + 1][x + 1] = '@';
            gamebox[y + 2][x] = '@';
            gamebox[y + 2][x + 1] = '@';

            count[y] += 2;
            count[y + 1] += 1;
            count[y + 2] += 1;
        }
        else if (corner == 270) {
            gamebox[y][x] = '@';
            gamebox[y][x + 1] = '@';
            gamebox[y][x + 2] = '@';
            gamebox[y][x + 3] = '@';
            gamebox[y][x + 4] = '@';
            gamebox[y][x + 5] = '@';
            gamebox[y - 1][x + 4] = '@';
            gamebox[y - 1][x + 5] = '@';

            count[y + 1] += 3;
            count[y - 1] += 1;
        }
    }

    bool check_the_touch(int y, int x, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) override {
        if (corner == 0) {
        
            if (gamebox[y + 3][x] == '=' || gamebox[y + 3][x] == '@' || gamebox[y + 3][x + 2] == '@') {
                this->transfer(y, x, gamebox, count);
                return true; 
            }
        }
        else if (corner == 90){

            if (gamebox[y + 2][x] == '=' || gamebox[y + 2][x] == '@' || gamebox[y + 1][x + 2] == '@' || gamebox[y + 1][x + 4] == '@') {
                this->transfer(y, x, gamebox, count);
                return true; 
            }
        }
        else if (corner == 180){

            if (gamebox[y + 3][x] == '=' || gamebox[y + 3][x] == '@' || gamebox[y + 1][x - 1] == '@') {
                this->transfer(y, x, gamebox, count);
                return true; 
            }
        }
        else if (corner == 270) {

            if (gamebox[y + 1][x] == '=' || gamebox[y + 1][x] == '@' || gamebox[y + 1][x + 2] == '@' || gamebox[y + 1][x + 4] == '@') {
                this->transfer(y, x, gamebox, count);
                return true; 
            }           
        }
        return false;
    }
};

struct Shape_six: public Shape {
    void draw (int y, int x, char ch) override {
        if (corner == 0 || corner == 180) {
            mvaddch(y, x, ch);
            mvaddch(y, x + 1, ch);
            mvaddch(y + 1, x, ch);
            mvaddch(y + 1, x + 1, ch);
            mvaddch(y + 2, x, ch);
            mvaddch(y + 2, x + 1, ch);
            mvaddch(y + 3, x, ch);
            mvaddch(y + 3, x + 1, ch);
        }
        else if (corner == 90 || corner == 270) {
            mvaddch(y, x, ch);
            mvaddch(y, x + 1, ch);
            mvaddch(y, x - 1, ch);
            mvaddch(y, x - 2, ch);
            mvaddch(y, x + 2, ch);
            mvaddch(y, x + 3, ch);
            mvaddch(y, x + 4, ch);
            mvaddch(y, x + 5, ch);
        }
    }

    void move_left(const int& y, int& x, std::vector<std::vector<char>>& gamebox) override {
        if (corner == 0 || corner == 180) {
            if (gamebox[y][x - 1] != '|' && gamebox[y][x - 1] != '@' && gamebox[y + 1][x - 1] != '@' && gamebox[y + 2][x - 1] != '@' && gamebox[y + 3][x - 1] != '@') {
                x -= 2;
            }
        }
        else if (corner == 90 || corner == 270) {
            if (gamebox[y][x - 3] != '|' && gamebox[y][x - 3] != '@') {
                x -= 2;
            }
        }
    }

    void move_right(const int& y, int& x, std::vector<std::vector<char>>& gamebox) override {
        if (corner == 0 || corner == 180) {
            if (gamebox[y][x + 2] != '|' && gamebox[y][x + 2] != '@' && gamebox[y + 1][x + 2] != '@' && gamebox[y + 2][x + 2] != '@' && gamebox[y + 3][x + 2] != '@') {
                x += 2;
            }
        }
        else if (corner == 90 || corner == 270) {
            if (gamebox[y][x + 6] != '|' && gamebox[y][x + 6] != '@') {
                x += 2;
            }
        }
    }

    void transfer(int y, int x, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) override {
        if (corner == 0 || corner == 180) {
            gamebox[y][x] = '@';
            gamebox[y][x + 1] = '@';
            gamebox[y + 1][x] = '@';
            gamebox[y + 1][x + 1] = '@';
            gamebox[y + 2][x] = '@';
            gamebox[y + 2][x + 1] = '@';
            gamebox[y + 3][x] = '@';
            gamebox[y + 3][x + 1] = '@';

            count[y] += 1;
            count[y + 1] += 1;
            count[y + 2] += 1;
            count[y + 3] += 1;
        }
        else if (corner == 90 || corner == 270) {
            gamebox[y][x] = '@';
            gamebox[y][x + 1] = '@';
            gamebox[y][x - 1] = '@';
            gamebox[y][x - 2] = '@';
            gamebox[y][x + 2] = '@';
            gamebox[y][x + 3] = '@';
            gamebox[y][x + 4] = '@';
            gamebox[y][x + 5] = '@';

            count[y] += 4;
        } 
    }

    bool check_the_touch(int y, int x, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) override {
        if (corner == 0 || corner == 180) {
        
            if (gamebox[y + 4][x] == '=' || gamebox[y + 4][x] == '@') {
                this->transfer(y, x, gamebox, count);
                return true; 
            }
        }
        else if (corner == 90 || corner == 270){

            if (gamebox[y + 1][x] == '=' || gamebox[y + 1][x] == '@' || gamebox[y + 1][x + 2] == '@' || gamebox[y + 1][x + 4] == '@' || gamebox[y + 1][x - 1] == '@') {
                this->transfer(y, x, gamebox, count);
                return true; 
            }
        }
        return false;
    }
};

struct Shape_seven: public Shape {
    void draw (int y, int x, char ch) override {
        if (corner == 0) {
            mvaddch(y, x, ch); 
            mvaddch(y, x + 1, ch);
            mvaddch(y + 1, x, ch); 
            mvaddch(y + 1, x + 1, ch);
            mvaddch(y + 1, x + 2, ch); 
            mvaddch(y + 1, x + 3, ch);
            mvaddch(y + 1, x - 1, ch); 
            mvaddch(y + 1, x - 2, ch);
        }
        else if (corner == 90) {
            mvaddch(y, x, ch);
            mvaddch(y, x + 1, ch);
            mvaddch(y - 1, x, ch);
            mvaddch(y - 1, x + 1, ch);
            mvaddch(y + 1, x, ch);
            mvaddch(y + 1, x + 1, ch);
            mvaddch(y, x + 2, ch);
            mvaddch(y, x + 3, ch);
        }
        else if (corner == 180) {
            mvaddch(y, x, ch);
            mvaddch(y, x + 1, ch);
            mvaddch(y, x + 2, ch);
            mvaddch(y, x + 3, ch);
            mvaddch(y, x - 1, ch);
            mvaddch(y, x - 2, ch);
            mvaddch(y + 1, x, ch);
            mvaddch(y + 1, x + 1, ch);
        }
        else if (corner == 270) {
            mvaddch(y, x, ch);
            mvaddch(y, x + 1, ch);
            mvaddch(y - 1, x, ch);
            mvaddch(y - 1, x + 1, ch);
            mvaddch(y + 1, x, ch);
            mvaddch(y + 1, x + 1, ch);
            mvaddch(y, x - 1, ch);
            mvaddch(y, x - 2, ch);
        }
    }

    void move_left(const int& y, int& x, std::vector<std::vector<char>>& gamebox) override {
        if (corner == 0) {
            if (gamebox[y + 1][x - 3] != '|' && gamebox[y + 1][x - 3] != '@' && gamebox[y][x - 1] != '@') {
                x -= 2;
            }
        }
        else if (corner == 90) {
            if (gamebox[y][x - 1] != '|' && gamebox[y][x - 1] != '@' && gamebox[y + 1][x - 1] != '@' && gamebox[y - 1][x - 1] != '@') {
                x -= 2;
            }
        }
        else if (corner == 180) {
            if (gamebox[y][x - 3] != '|' && gamebox[y][x - 3] != '@' && gamebox[y + 1][x - 1] != '@') {
                x -= 2;
            }
        }
        else {
            if (gamebox[y][x - 3] != '|' && gamebox[y][x - 3] != '@' && gamebox[y + 1][x - 1] != '@' && gamebox[y - 1][x - 1] != '@') {
                x -= 2;
            }
        }
    }

    void move_right(const int& y, int& x, std::vector<std::vector<char>>& gamebox) override {
        if (corner == 0) {
            if (gamebox[y + 1][x + 4] != '|' && gamebox[y + 1][x + 4] != '@' && gamebox[y][x + 2] != '@') {
                x += 2;
            }
        }
        else if (corner == 90) {
            if (gamebox[y][x + 4] != '|' && gamebox[y][x + 4] != '@' && gamebox[y + 1][x + 2] != '@' && gamebox[y - 1][x + 2] != '@') {
                x += 2;
            }
        }
        else if (corner == 180) {
            if (gamebox[y][x + 4] != '|' && gamebox[y][x + 4] != '@' && gamebox[y + 1][x + 2] != '@') {
                x += 2;
            }
        }
        else {
            if (gamebox[y][x + 2] != '|' && gamebox[y][x + 2] != '@' && gamebox[y + 1][x + 2] != '@' && gamebox[y - 1][x + 2] != '@') {
                x += 2;
            }
        }
    }

    void transfer(int y, int x, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) override {
        if (corner == 0) {
            gamebox[y][x] = '@';
            gamebox[y][x + 1] = '@';
            gamebox[y + 1][x] = '@';
            gamebox[y + 1][x + 1] = '@';
            gamebox[y + 1][x + 2] = '@';
            gamebox[y + 1][x + 3] = '@';
            gamebox[y + 1][x - 1] = '@';
            gamebox[y + 1][x - 2] = '@';

            count[y] += 1;
            count[y + 1] += 3;
        }
        else if (corner == 90) {
            gamebox[y][x] = '@';
            gamebox[y][x + 1] = '@';
            gamebox[y + 1][x] = '@';
            gamebox[y + 1][x + 1] = '@';
            gamebox[y][x + 2] = '@';
            gamebox[y][x + 3] = '@';
            gamebox[y - 1][x] = '@';
            gamebox[y - 1][x + 1] = '@';

            count[y] += 2;
            count[y + 1] += 1;
            count[y - 1] += 1;

        }
        else if (corner == 180) {
            gamebox[y][x] = '@';
            gamebox[y][x + 1] = '@';
            gamebox[y][x - 1] = '@';
            gamebox[y][x - 2] = '@';
            gamebox[y][x + 2] = '@';
            gamebox[y][x + 3] = '@';
            gamebox[y + 1][x] = '@';
            gamebox[y + 1][x + 1] = '@';

            count[y] += 3;
            count[y + 1] += 1;
        }
        else if (corner == 270) {
            gamebox[y][x] = '@';
            gamebox[y][x + 1] = '@';
            gamebox[y][x - 1] = '@';
            gamebox[y][x - 2] = '@';
            gamebox[y + 1][x] = '@';
            gamebox[y + 1][x + 1] = '@';
            gamebox[y - 1][x] = '@';
            gamebox[y - 1][x + 1] = '@';

            count[y] += 2;
            count[y + 1] += 1;
            count[y - 1] += 1;
        }
    }

    bool check_the_touch(int y, int x, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) override {
        if (corner == 0) {
        
            if (gamebox[y + 2][x] == '=' || gamebox[y + 2][x] == '@' || gamebox[y + 2][x + 2] == '@' || gamebox[y + 2][x - 1] == '@') {
                this->transfer(y, x, gamebox, count);
                return true; 
            }

        }
        else if (corner == 90){

            if (gamebox[y + 2][x] == '=' || gamebox[y + 2][x] == '@' || gamebox[y + 1][x + 2] == '@') {
                this->transfer(y, x, gamebox, count);
                return true; 
            }
        }
        else if (corner == 180){

            if (gamebox[y + 2][x] == '=' || gamebox[y + 2][x] == '@' || gamebox[y + 1][x - 1] == '@' || gamebox[y + 1][x + 2] == '@') {
                this->transfer(y, x, gamebox, count);
                return true; 
            }
        }
        else if (corner == 270) {

            if (gamebox[y + 2][x] == '=' || gamebox[y + 2][x] == '@' || gamebox[y][x - 1] == '@') {
                this->transfer(y, x, gamebox, count);
                return true; 
            }
        }
        return false;
    }
};

std::vector<std::vector<char>> box(int width, int height);

void remove_str(int start, int y, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) {
    if (y - start == 4 || y > 21) {
        return;
    }

    if (count[y] == 10) {
        for (int i = 1; i < 21; ++i) {
            gamebox[y][i] = ' ';
            mvaddch(y, i, ' ');
        }
        count[y] = 0;
    }
    remove_str(start, y + 1, gamebox, count);
}

void brick_move(int y, int x, Shape& a, std::vector<std::vector<char>>& gamebox, std::vector<int>& count) {

    while (true) {
        int symb = getch();
        if (symb == KEY_UP)
            a.turn();
        else if (symb == KEY_DOWN)
            a.boost();
        else if (symb == KEY_LEFT) {
            a.move_left(y, x, gamebox);
        }
        else if (symb == KEY_RIGHT) {
            a.move_right(y, x, gamebox);
        }

        a.frame(y++, x);

        if (a.check_the_touch(y, x, gamebox, count)) {
            remove_str(y, y - 1, gamebox, count);
            break;
        }
    }   
}

int main()
{
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

    while(true) {
        srand(time(0));
        int random = rand() % 7;
        Shape *y;

        switch (random)
        {
        case 0:
            y = new Shape_one;
            break;
        case 1:
            y = new Shape_two;
            break;
        case 2:
            y = new Shape_three;
            break;
        case 3:
            y = new Shape_four;
            break;
        case 4:
            y = new Shape_five;
            break;
        case 5:
            y = new Shape_six;
            break;
        case 6:
            y = new Shape_seven;
            break;  
        default:
            break;
        } 
        
        brick_move(1, 7, *y, gamebox, count);

        for (int i = 0; i < field_height; ++i) {
            for (int j = 0; j < field_width; ++j) {
                mvaddch(i, j, gamebox[i][j]);
            }
            mvprintw(i, 22, "%d", count[i]);
        }

        delete y;
    }

    endwin();
}

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