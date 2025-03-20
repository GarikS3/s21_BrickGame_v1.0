#include "frontend.h"

void initNcurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, 1);
  nodelay(stdscr, 1);
  curs_set(0);
  setColors();
}

void setColors() {
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
}

void renderGame(GameInfo_t info) {
  refresh();

  WINDOW *game_window = NULL;
  if (info.pause != GameStart && info.pause != GamePaused) {
    game_window = drawGameWindow(info);
    wrefresh(game_window);
  }

  WINDOW *info_window = drawInfoWindow(info);
  wrefresh(info_window);

  switch (info.pause) {
    case GameStart:
      drawStartPanel();
      break;
    case GamePaused:
      drawPausePanel();
      break;
    case GameFinished:
      drawGameoverPanel();
      break;
  }

  if (game_window != NULL) delwin(game_window);
  delwin(info_window);
}

WINDOW *drawGameWindow(GameInfo_t info) {
  WINDOW *game_window =
      newwin(WINDOW_H, WINDOW_W, WINDOW_START_Y, WINDOW_START_X);
  wattron(game_window, COLOR_PAIR(1));
  box(game_window, 0, 0);

  for (int i = 0; i < FIELD_H; i++) {
    for (int j = 0; j < FIELD_W; j++) {
      if (info.field[i][j] == 1) {
        wattron(game_window, COLOR_PAIR(1));
        mvwprintw(game_window, i + 1, 3 * j + 1, "[ ]");
      } else {
        wattron(game_window, COLOR_PAIR(1));
        mvwprintw(game_window, i + 1, 3 * j + 1, "   ");
      }
    }
  }
  return game_window;
}

WINDOW *drawInfoWindow(GameInfo_t info) {
  WINDOW *info_window =
      newwin(WINDOW_H, WINDOW_W, WINDOW_START_Y, WINDOW_START_X + WINDOW_W + 2);
  wattron(info_window, COLOR_PAIR(1));
  box(info_window, 0, 0);

  wattron(info_window, COLOR_PAIR(1));
  mvwprintw(info_window, 2, 5, "SCORE:  %d", info.score);
  mvwprintw(info_window, 5, 5, "RECORD:  %d", info.high_score);
  mvwprintw(info_window, 8, 5, "LEVEL:  %d", info.level);
  mvwprintw(info_window, 11, 5, "NEXT:");

  if (info.pause != GameStart) {
    for (int i = 0; i < FIGURE_SIZE; i++) {
      for (int j = 0; j < FIGURE_SIZE; j++) {
        if (info.next[i][j] == 1) {
          wattron(info_window, COLOR_PAIR(3));
          mvwprintw(info_window, 10 + i, 13 + 3 * j, "[ ]");
        }
      }
    }
  }
  wattron(info_window, COLOR_PAIR(2));
  mvwprintw(info_window, 17, 3, "Control: <-, v, ->, SPACE");
  mvwprintw(info_window, 18, 3, "Pause: 'p'");
  mvwprintw(info_window, 19, 3, "Exit game: 'q'");

  return info_window;
}

void drawStartPanel() {
  WINDOW *start_window =
      newwin(WINDOW_H, WINDOW_W, WINDOW_START_Y, WINDOW_START_X);
  wattron(start_window, COLOR_PAIR(1));
  box(start_window, 0, 0);
  wattron(start_window, COLOR_PAIR(2));
  mvwprintw(start_window, 6, 6, "____________________");
  mvwprintw(start_window, 9, 13, "TETRIS");
  mvwprintw(start_window, 11, 6, "____________________");
  mvwprintw(start_window, 17, 7, "Press 's' to start");
  wrefresh(start_window);
  delwin(start_window);
}

void drawPausePanel() {
  WINDOW *pause_window = newwin(5, 15, 9, 10);
  wattron(pause_window, COLOR_PAIR(2));
  box(pause_window, 0, 0);
  mvwprintw(pause_window, 2, 5, "PAUSE");
  wrefresh(pause_window);
  delwin(pause_window);
}

void drawGameoverPanel() {
  WINDOW *gameover_window = newwin(6, 22, 8, 6);
  wattron(gameover_window, COLOR_PAIR(2));
  box(gameover_window, 0, 0);
  mvwprintw(gameover_window, 1, 7, "GAME OVER");
  mvwprintw(gameover_window, 3, 4, "'s' - new game");
  mvwprintw(gameover_window, 4, 4, "'q' - quit");
  wrefresh(gameover_window);
  delwin(gameover_window);
}