#pragma once
#include "shape.h"
#include "field.h"

bool hasCollision(int y, int x, const std::vector<std::vector<int>>& shape, const Field& gamebox);
void placeBlock(int y, int x, int side, const Shape& shape, Field& gamebox, Counts& count);