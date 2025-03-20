#include "backend.h"

void checkFullLines() {
  GameState_t *state = getCurrentState();
  int full_lines = 0;

  for (int i = FIELD_H - 1; i > 0; i--) {
    int count_blocks = 0;
    for (int j = 0; j < FIELD_W; j++) {
      count_blocks += state->field[i][j];
    }

    if (count_blocks == FIELD_W) {
      for (int z = i; z > 0; z--) {
        for (int j = 0; j < FIELD_W; j++) {
          state->field[z][j] = state->field[z - 1][j];
        }
      }
      i++;
      full_lines++;
    }
  }
  updateScore(full_lines);
}

bool isFigureAtTopLine() {
  GameState_t *state = getCurrentState();
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (state->figure[i][j] == 1) {
        int new_y = state->fig_y + i;
        if (new_y <= 0) {
          return true;
        }
      }
    }
  }
  return false;
}

void updateScore(int removed_lines) {
  GameState_t *state = getCurrentState();

  switch (removed_lines) {
    case 1:
      state->score += 100;
      break;
    case 2:
      state->score += 300;
      break;
    case 3:
      state->score += 700;
      break;
    case 4:
      state->score += 1500;
      break;
    default:
      break;
  }

  if (state->score > state->high_score) {
    state->high_score = state->score;
    saveHighScore(state->high_score);
  }
  updateLevelAndSpeed();
}

void updateLevelAndSpeed() {
  GameState_t *state = getCurrentState();

  int new_level = state->score / MAX_SCORE_IN_LEVEL + 1;
  if (new_level > MAX_LEVEL) new_level = MAX_LEVEL;

  state->level = new_level;
  state->speed = INIT_SPEED - (state->level - 1) * SPEED_STEP;
}

int readHighScore() {
  int high_score = 0;
  FILE *file = fopen("high_score.txt", "r");
  if (file) {
    fscanf(file, "%d", &high_score);
    fclose(file);
  }
  return high_score;
}

void saveHighScore(int score) {
  FILE *file = fopen("high_score.txt", "w");
  if (file) {
    fprintf(file, "%d", score);
    fclose(file);
  }
}

unsigned long long getCurrentTime() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

bool timer(int delay) {
  GameState_t *state = getCurrentState();
  bool result = false;
  unsigned long long time = getCurrentTime();

  if (time - state->time >= (unsigned long long)delay) {
    state->time = time;
    result = true;
  }
  return result;
}

int **createMatrix(int rows, int cols) {
  int **matrix = (int **)calloc(rows, sizeof(int *));
  for (int i = 0; i < rows; i++) {
    matrix[i] = (int *)calloc(cols, sizeof(int));
  }
  return matrix;
}

void copyMatrix(int **dest, int **src, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      dest[i][j] = src[i][j];
    }
  }
}

void freeMatrix(int **matrix, int size) {
  if (matrix != NULL) {
    for (int i = 0; i < size; i++) {
      free(matrix[i]);
    }
    free(matrix);
  }
  matrix = NULL;
}

void freeMemory() {
  GameState_t *state = getCurrentState();

  freeMatrix(state->field, FIELD_H);
  freeMatrix(state->figure, FIGURE_SIZE);
  freeMatrix(state->next_figure, FIGURE_SIZE);
}

void freeGameInfo(GameInfo_t *info) {
  freeMatrix(info->field, FIELD_H);
  freeMatrix(info->next, FIGURE_SIZE);
}