#pragma once
#include "shape.h"
#include "field.h"

void drawBlock(int y, int x, const std::vector<std::vector<int>>& shape, char ch, int color);
void renderField(const Field& gamebox);   // вынести цикл из main