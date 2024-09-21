#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <locale.h>
#include "draw.h"
#include "poker.h"

void initNCurses() {
    setlocale(LC_CTYPE, ""); 
    initscr();
    start_color();
    cbreak(); 
    noecho(); 
	curs_set(0);
    keypad(stdscr, TRUE);
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

void toggleHelp(int* flag, Table* table, char* msg, int pot) {

	*flag = !(*flag);
	
	if(*flag)
		drawHelp();
	else {
		drawScreen(table, pot);
		drawMessage(msg);
	}
		

}

int main() {

	srand(time(NULL));
	initNCurses();
	initColorPairs();

	int help_mode = false;
	int pot = 0;
	int gamestate = 0;
	char* msg = "Press 'Space' to reveal your cards, press '?' for help.";


	Deck* deck = createDeck();

	Table* table = fullDeal(deck);

	drawScreen(table, pot);
	drawMessage(msg);

	refresh();



	// Input loop
	char ch;
	while((ch = getch()) != 'q') {
		switch(ch) {
			case ' ':
				switch(gamestate) {
					case 0:
						drawScreen(table, pot);
						revealCard(table, 0, pot);
						revealCard(table, 1, pot);
						gamestate++;
						break;
					case 1:
						revealCard(table, 2, pot);
						revealCard(table, 3, pot);
						revealCard(table, 4, pot);
						gamestate++;
						break;
					case 2:
						revealCard(table, 5, pot);
						gamestate++;
						break;
					case 3:
						revealCard(table, 6, pot);
						gamestate++;
						break;
					case 4:
						for(int i=0; i<8; i++)
							revealCard(table, 7+i, pot);
						
						int winner = 1;
						if(!winner)
							msg = "You won!";
						else
							msg = "You lost :(";
						drawMessage(msg);
						gamestate++;
						break;
				}
				break;
			case '?':
				toggleHelp(&help_mode, table, msg, pot);
				break;
			case 'b':
				if(gamestate > 0)
					placeBet(&pot, table);
				break;
			case 'r':
				if(gamestate == 5) {
					gamestate = 0;
					deck = createDeck();
					table = fullDeal(deck);
					pot = 0;
					drawScreen(table, pot);
				}
				break;
		}
        refresh();
	}


    endwin();

	free(deck);


	return 0;
}
