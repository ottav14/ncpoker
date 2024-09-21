#ifndef DRAW_H
#define DRAW_H

#include "poker.h"

void drawMessage(char* message);
void drawRect(int x, int y, int w, int h, int col);
void drawBox(int x, int y, int w, int h, int col);
void drawCard(int x, int y, int w, int h, Card* card);
void drawHand(Hand hand);
void drawBoard(Table* table);
void drawDeck(Deck* deck);
void drawOpponent(int x, int y, Hand hand);
void drawOpponents(int player_count, Table* table);
void drawScreen(Table* table, int pot); 
void drawBetMenu();
void drawHelp();
void eraseHelp(Table* table, char* msg);
void drawPotDisplay(); 

#endif
