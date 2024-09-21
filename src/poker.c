#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "draw.h"
#include "poker.h"

#define PLAYER_COUNT 4
#define MAX_BET_SIZE 4


Deck* createDeck() {

	char* suits[4] = { HEARTS, CLUBS, DIAMONDS, SPADES };
	char* values[13] = { "2", "3", "4", "5", "6", "7", "8", "9", "0", "J", "Q", "K", "A" }; 

	Deck* deck = (Deck*)malloc(sizeof(Deck));
	deck->count = 52;
	deck->cards = (Card**)malloc(deck->count*sizeof(Card*));

	int ix = 0;

	for(int i=0; i<4; i++) {
		for(int j=0; j<13; j++) {
			Card* card = (Card*)malloc(sizeof(Card));
			card->suit = suits[i];
			card->value = values[j];
			card->face_up = 0;
			deck->cards[ix] = card;
			ix++;
		}
	}

	return deck;


}

Card* pullCard(Deck* deck) {

	
	int ix = rand() % deck->count;
	while(!deck->cards[ix])
		ix = rand() % deck->count;
	
	Card* card = deck->cards[ix];
	deck->cards[ix] = NULL;
	deck->count--;

	return card;

}


Table* fullDeal(Deck* deck) {

	Table* table = (Table*)malloc(sizeof(Table));
	table->hand[0] = *pullCard(deck);
	table->hand[1] = *pullCard(deck);
	table->opponent_count = PLAYER_COUNT;
	table->opponents_hands = (Hand*)malloc(PLAYER_COUNT*sizeof(Hand));

	
	for(int i=0; i<PLAYER_COUNT; i++) {
		table->opponents_hands[i][0] = *pullCard(deck);
		table->opponents_hands[i][1] = *pullCard(deck);
	}

	table->board_count = 5;
	table->board = (Card*)malloc(table->board_count*sizeof(Card));

	for(int i=0; i<table->board_count; i++)
		table->board[i] = *pullCard(deck);


	return table;

}

void placeBet(int* pot, Table* table) {
	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	drawBetMenu();
	refresh();

	move(rows/2, cols/2-2);
	char* bet = (char*)malloc(MAX_BET_SIZE+1);
	int ibet = 0;
	int ix = 0;

	char ch = 'x';;
	while(ix<MAX_BET_SIZE && ch != '\n') {
		ch = getchar();
		if(0 <= ch - '0' && ch - '0' <= 9) {
			addch(ch);
			refresh();
			bet[ix] = ch;
			ix++;
			ibet = 10*ibet + ch - '0';
		}
	}

	*pot += ibet;
	drawScreen(table, *pot);

}

void revealCard(Table* table, int ix, int pot) {

	if(ix < 2)
		table->hand[ix].face_up = true;
	else if(ix < 7)
		table->board[ix-2].face_up = true;
	else
		table->opponents_hands[(ix-7)/2][(ix-7)%2].face_up = true;
	
	drawScreen(table, pot);
}

int hashCardValue(Card card) {
	if(2 <= card.value[0] - '0' && card.value[0] - '0' <= 9) return card.value[0] - '0';
	switch(card.value[0]) {
		case '0': return 10;
		case 'J': return 11;
		case 'Q': return 12;
		case 'K': return 13;
		case 'A': return 14;
	}
	return -1;
}

int checkHand(Table* table, int hand_ix) {

	Hand* hand;
	if(hand_ix == 0)
		hand = &table->hand;
	else
		hand = &table->opponents_hands[hand_ix-1];

	int h0 = hashCardValue(*hand[0]);
	int h1 = hashCardValue(*hand[1]);
	int result = h0 > h1 ? h0 : h1;


	// Check for pairs
	for(int i=0; i<2; i++) { 
		for(int j=0; j<table->board_count; j++) {
			if(strcmp(table->board[j].value, hand[i]->value)) {
				int val = 100 + hashCardValue(*hand[i]);
				result = val > result ? val : result;
			}
		}
	}



	
	return h0 > h1 ? h1 : h0;

}

int decideWinner(Table* table) {

	int highest_val = checkHand(table, 0);
	int highest_ix = 0;
	for(int i=0; i<table->opponent_count; i++) {
		int current = checkHand(table, i);
		if(current > highest_val) {
			highest_val = current;
			highest_ix = i;
		}
	}

	return highest_ix;


}

