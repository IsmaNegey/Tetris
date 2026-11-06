#pragma once
#include "shape.h"
#include "field.h"


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

void moveBlock(int y, int x, Shape& shape, Field& gamebox, Counts& count);
//void runGame();