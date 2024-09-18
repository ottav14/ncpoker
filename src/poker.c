#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "draw.h"
#include "poker.h"

Card** createDeck() {

	char* suits[4] = { "♥", "♦", "♣", "♠" };
	char* values[13] = { "2", "3", "4", "5", "6", "7", "8", "9", "0", "J", "Q", "K", "A" };
	int deck_ptr = 1;


	Card** deck = (Card**)malloc(53*sizeof(Card));

	for(int i=0; i<4; i++) {
		for(int j=0; j<13; j++) {

			Card* card = (Card*)malloc(sizeof(Card));
			card->suit = suits[i];
			card->value = values[j];
			deck[deck_ptr] = card;
			deck_ptr += 1;

		}
	}

	deck[0] = (Card*)52;

	return deck;


}

Card* pullCard(Card** deck) {
	int card_count = (int) deck[0];
	int ix = (int)(card_count * (float)rand() / (float)RAND_MAX);
	Card* card = deck[ix+1];
	deck[ix+1] = deck[card_count-1];
	deck[card_count-1] = NULL;
	deck[0] = (Card*)(card_count-1);
	return card;
}

Card** fullDeal(Card** deck) {

	Card** result = (Card**)malloc(5*sizeof(Card));

	for(int i=0; i<5; i++)
		result[i] = pullCard(deck);

	return result;

}
