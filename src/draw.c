#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "draw.h"
#include "poker.h"

#define CARD_WIDTH 10
#define CARD_HEIGHT 9
#define OPPONENT_CARD_WIDTH 5
#define OPPONENT_CARD_HEIGHT 4
#define BET_MENU_WIDTH 20
#define BET_MENU_HEIGHT 10
#define HELP_WIDTH 20
#define HELP_HEIGHT 10

void drawMessage(char* message) {
	int rows, cols;
	getmaxyx(stdscr, rows, cols);
	attron(COLOR_PAIR(3));
	mvprintw(rows/2, (cols - strlen(message))/2, message);
}

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


void drawCard(int x, int y, int w, int h, Card* card) {

	int cx, cy;
	getyx(stdscr, cy, cx);

	if(card->face_up) {
		drawRect(x, y, w, h, 8);

		if(strcmp(card->suit, HEARTS) || strcmp(card->suit, DIAMONDS))
			attron(COLOR_PAIR(9));
		else
			attron(COLOR_PAIR(8));

		if(card->value[0] != '0') {
			mvprintw(y, x, "%s%s", card->value, card->suit);
			mvprintw(y+h-1, x+w-3, "%s %s", card->suit, card->value);
		}
		else {
			mvprintw(y, x, "%s10", card->suit);
			mvprintw(y+h-1, x+w-4, "%s 10", card->suit);
		}
	}
	else {
		drawBox(x, y, w, h, 5);
	}

	move(cy, cx);


}

void drawHand(Hand hand) {

	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	int x = cols/2 - CARD_WIDTH - 2; 
	int y = rows - CARD_HEIGHT - 2; 
	drawBox(x, y, 2*CARD_WIDTH + 3, CARD_HEIGHT + 2, 3);

	// Cards
	drawCard(x+1, y+1, CARD_WIDTH, CARD_HEIGHT, &hand[0]);
	drawCard(x + CARD_WIDTH + 2, y+1, CARD_WIDTH, CARD_HEIGHT, &hand[1]);

}

void drawBoard(Table* table) {
	
	int rows, cols;
	getmaxyx(stdscr, rows, cols);
	(void)rows;

	int x = cols/2 - 5*CARD_WIDTH/2;
	int y = 0;

	drawBox(x-4, y, 5*CARD_WIDTH+6, CARD_HEIGHT+2, 3);

	for(int i=0; i<table->board_count; i++)
		drawCard(x+i*(CARD_WIDTH+1)-3, y+1, CARD_WIDTH, CARD_HEIGHT, &table->board[i]);

}

void drawOpponent(int x, int y, Hand hand) {

	int w = 2*OPPONENT_CARD_WIDTH+3;
	int h = OPPONENT_CARD_HEIGHT+2;
	
	drawBox(x, y, w, h, 3);

	drawCard(x+1, y+1, OPPONENT_CARD_WIDTH, OPPONENT_CARD_HEIGHT, &hand[0]); 
	drawCard(x+OPPONENT_CARD_WIDTH+2, y+1, OPPONENT_CARD_WIDTH, OPPONENT_CARD_HEIGHT, &hand[1]);

}

void drawOpponents(int player_count, Table* table) {

	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	int x = 5;
	int y = 5;

	int positions[8] = {x, y, cols-x-2*OPPONENT_CARD_WIDTH-3, y, x, rows-y-OPPONENT_CARD_HEIGHT-2, cols-x-2*OPPONENT_CARD_WIDTH-3, rows-y-OPPONENT_CARD_HEIGHT-2};
	if(player_count > 4)
		return;
	
	for(int i=0; i<2*player_count; i+=2)
		drawOpponent(positions[i], positions[i+1], table->opponents_hands[i/2]);

}

void drawPotDisplay(int pot) {
	
	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	attron(COLOR_PAIR(3));
	mvprintw(rows/2, cols/2-3, "Pot: %d", pot);

}

void drawScreen(Table* table, int pot) { 

	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	// Background
	drawRect(0, 0, cols, rows, 3); 


	drawBoard(table);
	drawHand(table->hand);
	drawOpponents(4, table);
	drawPotDisplay(pot);


}

void drawBetMenu() {
	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	int x = (cols-BET_MENU_WIDTH)/2;
	int y = (rows-BET_MENU_HEIGHT)/2;

	drawBox(x, y, BET_MENU_WIDTH, BET_MENU_HEIGHT, 8);
	drawBox(cols/2-3, rows/2-1, 6, 3, 8);

	attron(COLOR_PAIR(8));
	char* msg = "Bet size:";
	mvprintw(y+2, x+BET_MENU_WIDTH/2-strlen(msg)/2, msg);


}

void drawHelp() {
	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	int x = (cols-HELP_WIDTH)/2;
	int y = (rows-HELP_HEIGHT)/2;

	drawBox(x, y, HELP_WIDTH, HELP_HEIGHT, 8);

	attron(COLOR_PAIR(8));
	mvprintw(y+1, x+2, "Space.......Check");
	mvprintw(y+2, x+2, "B.............Bet");
	mvprintw(y+3, x+2, "F............Fold");

}

void eraseHelp(Table* table, char* msg) {
	int rows, cols;
	getmaxyx(stdscr, rows, cols);
	 
	int x = (cols-HELP_WIDTH)/2;
	int y = (rows-HELP_HEIGHT)/2;

	drawRect(x, y, HELP_WIDTH, HELP_HEIGHT, 3);

	drawBoard(table);
	drawHand(table->hand);
	drawMessage(msg);
	
}

