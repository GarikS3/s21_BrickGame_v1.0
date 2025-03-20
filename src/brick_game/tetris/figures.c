#include "backend.h"

void spawnFigure() {
  GameState_t *state = getCurrentState();

  freeMatrix(state->figure, FIGURE_SIZE);
  state->figure = state->next_figure;

  state->next_figure = generateNewFigure();
  state->fig_x = FIELD_W / 2 - FIGURE_SIZE / 2;
  state->fig_y = -1;
  state->time = getCurrentTime();

  state->status = Moving;
}

void attachFigureOnField() {
  GameState_t *state = getCurrentState();

  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      int new_x = state->fig_x + j;
      int new_y = state->fig_y + i;
      if ((new_x >= 0) && (new_x < FIELD_W) && (new_y >= 0) &&
          (new_y < FIELD_H) && (state->figure[i][j] == 1)) {
        state->field[new_y][new_x] = 1;
      }
    }
  }

  if (!canMove(state->fig_x, state->fig_y + 1) && isFigureAtTopLine()) {
    state->status = GameOver;
    state->pause = GameFinished;
  } else {
    checkFullLines();
    state->status = Spawn;
  }
}

int **generateNewFigure() {
  int **figure = createMatrix(FIGURE_SIZE, FIGURE_SIZE);
  int figure_index = rand() % NUM_FIGURES;
  writeFigure(figure, figure_index);

  return figure;
}

void writeFigure(int **figure, Figure_t figure_index) {
  switch (figure_index) {
    case Figure_I:
      figure[1][0] = 1;
      figure[1][1] = 1;
      figure[1][2] = 1;
      figure[1][3] = 1;
      break;
    case Figure_J:
      figure[1][1] = 1;
      figure[2][1] = 1;
      figure[2][2] = 1;
      figure[2][3] = 1;
      break;
    case Figure_L:
      figure[1][2] = 1;
      figure[2][0] = 1;
      figure[2][1] = 1;
      figure[2][2] = 1;
      break;
    case Figure_O:
      figure[1][1] = 1;
      figure[1][2] = 1;
      figure[2][1] = 1;
      figure[2][2] = 1;
      break;
    case Figure_S:
      figure[1][1] = 1;
      figure[1][2] = 1;
      figure[2][0] = 1;
      figure[2][1] = 1;
      break;
    case Figure_T:
      figure[1][1] = 1;
      figure[2][0] = 1;
      figure[2][1] = 1;
      figure[2][2] = 1;
      break;
    case Figure_Z:
      figure[1][1] = 1;
      figure[1][2] = 1;
      figure[2][2] = 1;
      figure[2][3] = 1;
      break;
    default:
      break;
  }
}