#include <check.h>

#include "../brick_game/tetris/backend.h"
#include "../tetris_main.h"

START_TEST(test_init_state) {
  initStartState();
  GameState_t *state = getCurrentState();
  ck_assert_int_eq(state->status, Initial);
  ck_assert_int_eq(state->score, 0);
  ck_assert_int_eq(state->level, 1);
  ck_assert_int_eq(state->speed, INIT_SPEED);
  ck_assert_int_eq(state->pause, GameStart);
  freeMemory();
}
END_TEST

START_TEST(test_update_state) {
  initStartState();
  GameState_t *state = getCurrentState();
  GameInfo_t info = updateCurrentState();

  ck_assert_int_eq(state->score, info.score);
  ck_assert_int_eq(state->high_score, info.high_score);
  ck_assert_int_eq(state->level, info.level);
  ck_assert_int_eq(state->speed, info.speed);
  ck_assert_int_eq(state->pause, info.pause);
  freeMemory();
  freeMatrix(info.field, FIELD_H);
  freeMatrix(info.next, FIGURE_SIZE);
}
END_TEST

START_TEST(test_userInput_start) {
  initStartState();
  GameState_t *state = getCurrentState();

  UserAction_t action = processInput(START_BUTTON);
  ck_assert_int_eq(action, Start);
  userInput(action, false);
  ck_assert_int_eq(state->status, Spawn);
  freeMemory();
}
END_TEST

START_TEST(test_userInput_left) {
  initStartState();
  GameState_t *state = getCurrentState();

  startGame();
  spawnFigure();
  state->fig_x = 5;
  UserAction_t action = processInput(KEY_LEFT);
  ck_assert_int_eq(action, Left);
  userInput(action, false);
  ck_assert_int_eq(state->fig_x, 4);
  ck_assert_int_eq(state->status, Moving);
  freeMemory();
}
END_TEST

START_TEST(test_userInput_right) {
  initStartState();
  GameState_t *state = getCurrentState();

  startGame();
  spawnFigure();
  state->fig_x = 5;
  UserAction_t action = processInput(KEY_RIGHT);
  ck_assert_int_eq(action, Right);
  userInput(action, false);
  ck_assert_int_eq(state->fig_x, 6);
  ck_assert_int_eq(state->status, Moving);
  freeMemory();
}
END_TEST

START_TEST(test_userInput_down) {
  initStartState();
  GameState_t *state = getCurrentState();

  startGame();
  spawnFigure();
  state->fig_y = 5;
  UserAction_t action = processInput(KEY_DOWN);
  ck_assert_int_eq(action, Down);
  userInput(action, false);
  ck_assert_int_eq(state->fig_y, 6);
  ck_assert_int_eq(state->status, Moving);
  freeMemory();
}
END_TEST

START_TEST(test_userInput_pause) {
  initStartState();
  GameState_t *state = getCurrentState();
  startGame();

  ck_assert_int_eq(state->pause, GameON);
  UserAction_t action = processInput(PAUSE_BUTTON);
  ck_assert_int_eq(action, Pause);
  userInput(action, false);
  ck_assert_int_eq(state->pause, GamePaused);
  pauseGame();
  ck_assert_int_eq(state->pause, GameON);
  freeMemory();
}
END_TEST

START_TEST(test_userInput_action) {
  initStartState();
  GameState_t *state = getCurrentState();

  startGame();
  spawnFigure();
  state->fig_y = 5;
  int fig[FIGURE_SIZE][FIGURE_SIZE] = {
      {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}};
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      state->figure[i][j] = fig[i][j];
    }
  }
  UserAction_t action = processInput(ACTION_BUTTON);
  ck_assert_int_eq(action, Action);
  userInput(action, false);
  int rotated_fig[FIGURE_SIZE][FIGURE_SIZE] = {
      {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(state->figure[i][j], rotated_fig[i][j]);
    }
  }
  ck_assert_int_eq(state->status, Moving);
  freeMemory();
}
END_TEST

START_TEST(test_userInput_terminate) {
  UserAction_t action = processInput(TERMINATE_BUTTON);
  ck_assert_int_eq(action, Terminate);
}
END_TEST

START_TEST(test_write_figure) {
  int **figure = createMatrix(FIGURE_SIZE, FIGURE_SIZE);
  writeFigure(figure, Figure_J);
  ck_assert_int_eq(figure[1][1], 1);
  ck_assert_int_eq(figure[2][1], 1);
  ck_assert_int_eq(figure[2][2], 1);
  ck_assert_int_eq(figure[2][3], 1);

  freeMatrix(figure, FIGURE_SIZE);
}
END_TEST

START_TEST(test_spawn_figure) {
  initStartState();
  GameState_t *state = getCurrentState();
  spawnFigure();
  ck_assert_int_eq(state->fig_y, -1);
  ck_assert_int_eq(state->status, Moving);
  freeMemory();
}
END_TEST

START_TEST(test_attaching) {
  initStartState();
  GameState_t *state = getCurrentState();

  startGame();
  spawnFigure();
  state->fig_x = 5;
  state->fig_y = FIELD_H - 1;
  attachFigureOnField();
  ck_assert_int_eq(state->status, Spawn);
  freeMemory();
}
END_TEST

START_TEST(test_can_move) {
  initStartState();
  GameState_t *state = getCurrentState();

  startGame();
  spawnFigure();
  state->fig_x = 5;
  state->fig_y = 5;
  shiftFigureDown();
  ck_assert_int_eq(canMove(state->fig_x + 1, state->fig_y + 1), true);
  ck_assert_int_eq(state->status, Moving);
  freeMemory();
}
END_TEST

START_TEST(test_can_move_2) {
  initStartState();
  GameState_t *state = getCurrentState();

  startGame();
  spawnFigure();
  state->fig_x = 5;
  state->fig_y = FIELD_H - 1;
  shiftFigureDown();
  ck_assert_int_eq(canMove(state->fig_x, state->fig_y + 1), false);
  ck_assert_int_eq(state->status, Attaching);
  freeMemory();
}
END_TEST

START_TEST(test_can_move_3) {
  initStartState();
  GameState_t *state = getCurrentState();

  startGame();
  spawnFigure();
  state->fig_x = 0;
  state->fig_y = 5;
  state->status = Moving;
  moveFigureLeft();
  ck_assert_int_eq(canMove(state->fig_x - 1, state->fig_y), false);
  ck_assert_int_eq(state->status, Moving);
  freeMemory();
}
END_TEST

START_TEST(test_can_move_4) {
  initStartState();
  GameState_t *state = getCurrentState();

  startGame();
  spawnFigure();
  state->fig_x = FIELD_W - 1;
  state->fig_y = 5;
  state->status = Moving;
  moveFigureRight();
  ck_assert_int_eq(canMove(state->fig_x + 1, state->fig_y), false);
  ck_assert_int_eq(state->status, Moving);
  freeMemory();
}
END_TEST

START_TEST(test_can_rotate) {
  initStartState();
  GameState_t *state = getCurrentState();

  startGame();
  spawnFigure();
  state->fig_x = 5;
  state->fig_y = 5;
  int **new_figure = generateNewFigure();
  ck_assert_int_eq(canRotate(new_figure), true);
  freeMemory();
}
END_TEST

START_TEST(test_full_field) {
  initStartState();
  GameState_t *state = getCurrentState();

  for (int i = 0; i < FIELD_H; i++) {
    for (int j = 0; j < FIELD_W; j++) {
      state->field[i][j] = 1;
    }
  }
  spawnFigure();
  attachFigureOnField();
  ck_assert(isFigureAtTopLine() == true);
  ck_assert_int_eq(state->status, GameOver);
  freeMemory();
}
END_TEST

START_TEST(test_delete_full_lines) {
  initStartState();
  GameState_t *state = getCurrentState();

  for (int i = 0; i < FIELD_W; i++) {
    state->field[FIELD_H - 1][i] = 1;
  }
  checkFullLines();
  ck_assert_int_eq(state->field[FIELD_H - 1][0], 0);
  freeMemory();
}
END_TEST

START_TEST(test_score_for_1_line) {
  initStartState();
  GameState_t *state = getCurrentState();
  for (int j = 0; j < FIELD_W; j++) {
    state->field[FIELD_H - 1][j] = 1;
  }
  checkFullLines();
  ck_assert_int_eq(state->score, 100);
  freeMemory();
}
END_TEST

START_TEST(test_score_for_2_lines) {
  initStartState();
  GameState_t *state = getCurrentState();
  for (int i = FIELD_H - 1; i > FIELD_H - 3; i--) {
    for (int j = 0; j < FIELD_W; j++) {
      state->field[i][j] = 1;
    }
  }
  checkFullLines();
  ck_assert_int_eq(state->score, 300);
  freeMemory();
}
END_TEST

START_TEST(test_score_for_3_lines) {
  initStartState();
  GameState_t *state = getCurrentState();
  for (int i = FIELD_H - 1; i > FIELD_H - 4; i--) {
    for (int j = 0; j < FIELD_W; j++) {
      state->field[i][j] = 1;
    }
  }
  checkFullLines();
  ck_assert_int_eq(state->score, 700);
  freeMemory();
}
END_TEST

START_TEST(test_score_for_4_lines) {
  initStartState();
  GameState_t *state = getCurrentState();
  for (int i = FIELD_H - 1; i > FIELD_H - 5; i--) {
    for (int j = 0; j < FIELD_W; j++) {
      state->field[i][j] = 1;
    }
  }
  checkFullLines();
  ck_assert_int_eq(state->score, 1500);
  freeMemory();
}
END_TEST

START_TEST(test_update_level_and_speed) {
  initStartState();
  GameState_t *state = getCurrentState();

  state->score = 700;
  updateLevelAndSpeed();
  ck_assert_int_eq(state->level, 2);
  ck_assert_int_eq(state->speed, 810);

  state->score = 2000;
  updateLevelAndSpeed();
  ck_assert_int_eq(state->level, 4);
  ck_assert_int_eq(state->speed, 630);
  freeMemory();
}
END_TEST

START_TEST(test_read_and_save_high_score) {
  initStartState();
  GameState_t *state = getCurrentState();

  int points = 2000;
  state->score = points;
  saveHighScore(points);
  state->high_score = state->score;
  ck_assert_int_eq(state->high_score, state->score);

  int score = readHighScore();
  ck_assert_int_eq(score, 2000);
  freeMemory();
}
END_TEST

START_TEST(test_create_matrix) {
  int rows = 10;
  int cols = 10;
  int **matr = createMatrix(rows, cols);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      ck_assert_int_eq(matr[i][j], 0);
    }
  }
  freeMatrix(matr, rows);
}
END_TEST

START_TEST(test_copy_matrix) {
  int rows = 10;
  int cols = 10;
  int **matrix_1 = createMatrix(rows, cols);
  int **matrix_2 = createMatrix(rows, cols);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      matrix_1[i][j] = 1;
    }
  }
  copyMatrix(matrix_2, matrix_1, rows, cols);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      ck_assert_int_eq(matrix_1[i][j], matrix_2[i][j]);
    }
  }
  freeMatrix(matrix_1, rows);
  freeMatrix(matrix_2, rows);
}
END_TEST

Suite *game_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Game");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_init_state);
  tcase_add_test(tc_core, test_update_state);
  tcase_add_test(tc_core, test_userInput_start);
  tcase_add_test(tc_core, test_userInput_left);
  tcase_add_test(tc_core, test_userInput_right);
  tcase_add_test(tc_core, test_userInput_down);
  tcase_add_test(tc_core, test_userInput_action);
  tcase_add_test(tc_core, test_userInput_pause);
  tcase_add_test(tc_core, test_userInput_terminate);
  tcase_add_test(tc_core, test_write_figure);
  tcase_add_test(tc_core, test_spawn_figure);
  tcase_add_test(tc_core, test_attaching);
  tcase_add_test(tc_core, test_can_move);
  tcase_add_test(tc_core, test_can_move_2);
  tcase_add_test(tc_core, test_can_move_3);
  tcase_add_test(tc_core, test_can_move_4);
  tcase_add_test(tc_core, test_can_rotate);
  tcase_add_test(tc_core, test_full_field);
  tcase_add_test(tc_core, test_delete_full_lines);
  tcase_add_test(tc_core, test_score_for_1_line);
  tcase_add_test(tc_core, test_score_for_2_lines);
  tcase_add_test(tc_core, test_score_for_3_lines);
  tcase_add_test(tc_core, test_score_for_4_lines);
  tcase_add_test(tc_core, test_update_level_and_speed);
  tcase_add_test(tc_core, test_read_and_save_high_score);
  tcase_add_test(tc_core, test_create_matrix);
  tcase_add_test(tc_core, test_copy_matrix);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = game_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}