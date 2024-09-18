#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <locale.h>
#include "draw.h"
#include "poker.h"

#define PADDING_X 15
#define PADDING_Y 1

void initNCurses() {
    setlocale(LC_CTYPE, ""); 
    initscr();
    start_color();
    cbreak(); 
    noecho(); 
	curs_set(0);
    keypad(stdscr, TRUE);
}

int getCol(int x, int y) {

	chtype cell_content = mvwinch(stdscr, y, x);
	return PAIR_NUMBER(cell_content & A_COLOR);

}

void updateCursor(int x, int y) {

	move(y, x);

}

void initColorPairs() {

	// Text
	init_pair(1, COLOR_WHITE, COLOR_RED);
	init_pair(2, COLOR_WHITE, COLOR_YELLOW);
	init_pair(3, COLOR_WHITE, COLOR_GREEN);
	init_pair(4, COLOR_WHITE, COLOR_CYAN);
	init_pair(5, COLOR_WHITE, COLOR_BLUE);
	init_pair(6, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(7, COLOR_WHITE, COLOR_BLACK);
	init_pair(8, COLOR_BLACK, COLOR_WHITE);
	init_pair(9, COLOR_RED,   COLOR_WHITE);

}

int main() {


	initNCurses();
	initColorPairs();
	srand(time(NULL));

	int rows, cols;
    getmaxyx(stdscr, rows, cols);


	Card** deck = createDeck();

	Card** deal = fullDeal(deck);

	int card_count = 5;
	Card** table = fullDeal(deck);

	drawScreen(cols, rows, table, card_count);
	updateCursor(cols / 2, rows / 2);

	// Input loop
	char ch;
	while((ch = getch()) != 'q') {
		switch(ch) {
			case 'h':
				updateCursor(-1, 0);
				break;
			case 'j':
				updateCursor(0, -1);
				break;
			case 'k':
				updateCursor(0, 1);
				break;
			case 'l':
				updateCursor(1, 0);
				break;
		}
        refresh();
	}


    endwin();

	free(deck);


	return 0;
}
