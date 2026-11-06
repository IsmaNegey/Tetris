#pragma once

namespace cfg {
    // поле (в клетках)
    inline constexpr int kFieldCols    = 10;
    inline constexpr int kFieldRows    = 20;

    // размеры одной клетки в символах терминала
    inline constexpr int kCellWidth    = 2;
    inline constexpr int kCellHeight   = 1;

    // рамки
    inline constexpr int kWalls        = 2;   // левая + правая
    inline constexpr int kTopBottom    = 2;   // верх + низ

    inline constexpr int kScreenWidth  = kFieldCols * kCellWidth + kWalls;
    inline constexpr int kScreenHeight = kFieldRows * kCellHeight + kTopBottom;

    // игровые
    inline constexpr int kStartX       = 9;
    inline constexpr int kStartY       = 2;
    inline constexpr int kNumColors    = 8;
    inline constexpr char kBlockChar   = '@';
}