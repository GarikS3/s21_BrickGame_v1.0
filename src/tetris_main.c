#include "tetris_main.h"

int main() {
  initNcurses();
  initStartState();
  gameLoop();
  endwin();

  return 0;
}

void gameLoop() {
  GameInfo_t info;

  while (true) {
    int ch = getch();
    UserAction_t action = processInput(ch);
    timeout(10);
    userInput(action, false);
    handleGameState();

    info = updateCurrentState();
    renderGame(info);

    freeGameInfo(&info);
  }
}
