#ifndef FRONTEND_H
#define FRONTEND_H

#include <ncurses.h>

#include "../../tetris_main.h"

#define WINDOW_H (FIELD_H + 2)
#define WINDOW_W (FIELD_W * 3 + 2)
#define WINDOW_START_Y 1
#define WINDOW_START_X 1

void initNcurses();
void setColors();
void renderGame(GameInfo_t info);
WINDOW *drawGameWindow(GameInfo_t info);
WINDOW *drawInfoWindow(GameInfo_t info);
void drawStartPanel();
void drawPausePanel();
void drawGameoverPanel();

#endif