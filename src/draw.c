#include<ncurses.h>
#include<string.h>
#include "draw.h"
#include "poker.h"

#define CARD_WIDTH 10
#define CARD_HEIGHT 9

void drawRect(int x, int y, int w, int h, int col) {

	int cx, cy;
	getyx(stdscr, cy, cx);

	attron(COLOR_PAIR(col));

	for(int i=0; i<h; i++) {
		move(y+i, x);
		for(int j=0; j<w; j++)
			addch(' ');
	}

	attroff(COLOR_PAIR(col));
	move(cy, cx);
	
}

void drawBox(int x, int y, int w, int h, int col) {

	int cx, cy;
	getyx(stdscr, cy, cx);

	attron(COLOR_PAIR(col));

	mvprintw(y, x, "┌");
	for(int i=1; i<w-1; i++)
		printw("─");
	printw("┐");

	for(int i=1; i<h-1; i++) {
		mvprintw(y+i, x, "│"); 
		mvprintw(y+i, x+w-1, "│");
	}

	mvprintw(y+h-1, x, "└");
	for(int i=1; i<w-1; i++)
		printw("─");
	printw("┘");

	drawRect(x+1, y+1, w-2, h-2, col);

	attroff(COLOR_PAIR(col));
	move(cy, cx);
	
}


void drawCard(int x, int y, Card* card) {

	int cx, cy;
	getyx(stdscr, cy, cx);

	drawRect(x, y, CARD_WIDTH, CARD_HEIGHT, 8);

	if(strcmp(card->suit, HEARTS) || strcmp(card->suit, DIAMONDS))
		attron(COLOR_PAIR(9));
	else
		attron(COLOR_PAIR(8));

	mvprintw(y, x, "%s%s", card->value, card->suit);
	mvprintw(y+CARD_HEIGHT-1, x+CARD_WIDTH-3, "%s %s", card->suit, card->value);

	move(cy, cx);


}


void drawScreen(int cols, int rows, Card** table, int card_count) { 

	
	// Background
	drawRect(0, 0, cols, rows, 3); 

	int board_x = cols/2 - 5*CARD_WIDTH/2;
	int board_y = 3;

	// Table
	drawBox(board_x-4, board_y, 5*CARD_WIDTH+6, CARD_HEIGHT+2, 3);


	// Cards
	for(int i=0; i<card_count; i++)
		drawCard(board_x+i*(CARD_WIDTH+1)-3, board_y+1, table[i]);
	

	// Hand




}

