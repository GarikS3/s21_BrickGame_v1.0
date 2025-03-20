#include "backend.h"

void shiftFigureDown() {
  GameState_t *state = getCurrentState();

  if (canMove(state->fig_x, state->fig_y + 1)) {
    state->fig_y++;
    state->status = Moving;
  } else {
    state->status = Attaching;
  }
}

void moveFigureLeft() {
  GameState_t *state = getCurrentState();

  if (canMove(state->fig_x - 1, state->fig_y)) state->fig_x--;
}

void moveFigureRight() {
  GameState_t *state = getCurrentState();

  if (canMove(state->fig_x + 1, state->fig_y)) state->fig_x++;
}

void rotateFigure() {
  GameState_t *state = getCurrentState();

  int **rotated_figure = createMatrix(FIGURE_SIZE, FIGURE_SIZE);
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      rotated_figure[j][FIGURE_SIZE - 1 - i] = state->figure[i][j];
    }
  }
  if (canRotate(rotated_figure)) {
    freeMatrix(state->figure, FIGURE_SIZE);
    state->figure = rotated_figure;
  } else {
    freeMatrix(rotated_figure, FIGURE_SIZE);
  }
}

bool canMove(int new_x, int new_y) {
  GameState_t *state = getCurrentState();
  bool can_move = true;

  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (state->figure[i][j] == 1) {
        int field_x = new_x + j;
        int field_y = new_y + i;
        if (field_x < 0 || field_x >= FIELD_W || field_y >= FIELD_H ||
            state->field[field_y][field_x]) {
          can_move = false;
        }
      }
    }
  }
  return can_move;
}

bool canRotate(int **rotatedFigure) {
  GameState_t *state = getCurrentState();

  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (rotatedFigure[i][j]) {
        int field_x = state->fig_x + j;
        int field_y = state->fig_y + i;
        if (field_x < 0 || field_x >= FIELD_W || field_y < 0 ||
            field_y >= FIELD_H || state->field[field_y][field_x]) {
          return false;
        }
      }
    }
  }
  return true;
}