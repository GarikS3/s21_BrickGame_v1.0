#ifndef BACKEND_H
#define BACKEND_H

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "../../tetris_main.h"

#define START_BUTTON 115
#define PAUSE_BUTTON 112
#define ACTION_BUTTON 32
#define TERMINATE_BUTTON 113

#define NUM_FIGURES 7
#define FIGURE_SIZE 4
#define MAX_LEVEL 10
#define MAX_SCORE_IN_LEVEL 600
#define INIT_SPEED 900
#define SPEED_STEP 90

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int status;
  int **field;
  int **figure;
  int **next_figure;
  int fig_x;
  int fig_y;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
  unsigned long long time;
} GameState_t;

typedef enum {
  Initial,
  Spawn,
  Moving,
  Shifting,
  Attaching,
  GameOver
} GameStatus_t;

typedef enum {
  Figure_I,
  Figure_J,
  Figure_L,
  Figure_O,
  Figure_S,
  Figure_T,
  Figure_Z
} Figure_t;

void initStartState();
GameState_t *getCurrentState();
GameInfo_t updateCurrentState();
UserAction_t processInput(int ch);
void userInput(UserAction_t action, bool hold);
void updateGameField(int **field);
void startGame();
void pauseGame();
void freeMemory();
void handleGameState();
void handleGameoverState();
int **generateNewFigure();
void writeFigure(int **figure, Figure_t figure_index);
void spawnFigure();
void moveFigureLeft();
void moveFigureRight();
void shiftFigureDown();
void rotateFigure();
bool canMove(int new_x, int new_y);
bool canRotate(int **rotated_figure);
bool isFigureAtTopLine();
void attachFigureOnField();
void checkFullLines();
void updateScore(int removed_lines);
void updateLevelAndSpeed();
int readHighScore();
void saveHighScore(int score);
void finishGame();
unsigned long long getCurrentTime();
bool timer(int delay);
int **createMatrix(int rows, int cols);
void copyMatrix(int **dest, int **src, int rows, int cols);
void freeMatrix(int **matrix, int size);
void freeGameInfo(GameInfo_t *info);

#endif