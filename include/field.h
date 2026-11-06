#pragma once
#include <vector>
#include <ncurses.h>
#include "config.h"
#include "cell.h"

using Field = std::vector<std::vector<Cell>>;
using Counts = std::vector<int>;

Field drawField(int width, int height);
std::vector<int> checkLines(const Counts& count);
void cleanLines(Field& gamebox, const std::vector<int>& lines, Counts& count);
void wrappingLine(int y, Field& gamebox, Counts& count);
void wrapLines(int line, Field& gamebox, Counts& count);