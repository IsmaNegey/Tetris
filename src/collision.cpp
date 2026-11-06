#include "collision.h"

bool hasCollision(int y, int x,
                  const std::vector<std::vector<int>>& shape,
                  const Field& gamebox) {
    const int H = static_cast<int>(gamebox.size());
    const int W = static_cast<int>(gamebox[0].size());

    for (size_t col = 0; col < shape.size(); ++col) {
        for (size_t row = 0; row < shape[col].size(); ++row) {
            if (!shape[col][row]) continue;

            int ny = y + static_cast<int>(col);
            int nx = x + static_cast<int>(row);

            if (ny < 0 || ny >= H) return true;
            if (nx < 0 || nx >= W) return true;
            char c = gamebox[ny][nx].filled;
            if (c == '=' || c == '_' || c == '|') return true;
            if (c == '@') return true;
        }
    }
    return false;
}

void placeBlock(int y, int x, int side, const Shape& shape, std::vector<std::vector<Cell>>& gamebox, std::vector<int>& count) {
    for (size_t j = 0; j < shape.block[side].size(); ++j) {
        for (size_t k = 0; k < shape.block[side][j].size(); ++k) {
            if (shape.block[side][j][k]) {
                gamebox[y + j][x + k].filled = '@';
                gamebox[y + j][x + k].color = shape.color;
                count[y + j]++;                
            }   
        }
    }
}
