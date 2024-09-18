#ifndef DRAW_H
#define DRAW_H

#include "poker.h"

void fillRect(int x, int y, int w, int h, int col);
void fillCell(int x, int y, int col);
void fillLine(int x1, int y1, int x2, int y2, int col);
void drawScreen(int cols, int rows, Card** table, int card_count); 

#endif
