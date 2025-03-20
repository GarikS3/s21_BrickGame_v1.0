CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g
LDFLAGS = -lcheck -lm
GCOV_FLAGS = --coverage -fprofile-arcs -ftest-coverage
SRC = tetris_main.c $(wildcard brick_game/tetris/*.c) $(wildcard gui/cli/*.c)
DIST_DIR = s21_Brick_game

OS_NAME = $(shell uname)

ifeq ($(OS_NAME),Darwin)
    OPEN = open
    LDFLAGS += -L/usr/local/opt/ncurses/lib -lncurses
    CFLAGS += -I/usr/local/opt/ncurses/include
endif

ifeq ($(OS_NAME),Linux)
    OPEN = xdg-open
    LDFLAGS += -lsubunit -lncurses
endif

all: clean install

install: clean
	mkdir -p build
	$(CC) $(CFLAGS) -c $(SRC)
	ar rcs build/libtetris.a *.o
	rm -f *.o
	echo 0 > build/high_score.txt
	$(CC) $(CFLAGS) tetris_main.c -Lbuild -ltetris $(LDFLAGS) -o build/Tetris

run:
	cd build && ./Tetris

uninstall:
	rm -rf build

clean:
	rm -rf build
	rm -f tests/*.gcda tests/*.gcno *.gcov
	rm -rf coverage_report
	rm -f *.info
	rm -f tests/test tests/*.txt
	rm -rf tests/*.dSYM
	rm -rf Tetris.tar.gz

dist:
	mkdir -p $(DIST_DIR)
	cp -a brick_game $(DIST_DIR)
	cp -a gui $(DIST_DIR)
	cp -a dvi $(DIST_DIR)
	cp -a tests $(DIST_DIR)
	cp -a *.c $(DIST_DIR)
	cp -a *.h $(DIST_DIR)
	cp -a tetris.png $(DIST_DIR)
	tar -czf Tetris.tar.gz $(DIST_DIR)
	rm -rf $(DIST_DIR)

test: install
	echo 0 > tests/high_score.txt
	$(CC) $(CFLAGS) $(wildcard tests/*.c) -Lbuild -ltetris $(LDFLAGS) -o tests/test
	cd tests && ./test

gcov_report: install
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(wildcard brick_game/tetris/*.c) $(wildcard gui/cli/*.c) $(wildcard tests/*.c) -Lbuild -ltetris $(LDFLAGS) -o tests/test
	cd tests && ./test
	lcov -t "Tetris Coverage" -o coverage.info -c -d .
	genhtml -o coverage_report coverage.info
	$(OPEN) coverage_report/index.html

clang_check:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -n *.c *.h
	clang-format -n brick_game/tetris/*.c brick_game/tetris/*.h
	clang-format -n gui/cli/*.c gui/cli/*.h
	clang-format -n tests/*.c
	rm -f .clang-format

clang_format:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i *.c *.h
	clang-format -i brick_game/tetris/*.c brick_game/tetris/*.h
	clang-format -i gui/cli/*.c gui/cli/*.h
	clang-format -i tests/*.c
	rm -f .clang-format

valgrind: test
	cd tests && valgrind --tool=memcheck --leak-check=yes ./test

leaks: test
	cd tests && leaks --atExit -- ./test

rebuild: clean all