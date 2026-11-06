#pragma once
#include "shape.h"
#include "config.h"
#include <ncurses.h>

void handleBlockInput(int symb, int& x, const Shape& shape, int& side, int& time);