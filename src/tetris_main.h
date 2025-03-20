#ifndef TETRIS_MAIN_H
#define TETRIS_MAIN_H

#define FIELD_W 10
#define FIELD_H 20

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef enum { GameStart, GameON, GamePaused, GameFinished } PauseState_t;

#include "brick_game/tetris/backend.h"
#include "gui/cli/frontend.h"

void gameLoop();

#endif