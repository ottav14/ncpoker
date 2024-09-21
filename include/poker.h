#ifndef POKER_H
#define POKER_H

#define HEARTS "\u2665"
#define DIAMONDS "\u2666"
#define CLUBS "\u2663"
#define SPADES "\u2660"

typedef struct Card {
	char* suit;
	char* value;
	int face_up;
} Card;

typedef struct Deck {
	Card** cards;
	int count;
} Deck;

typedef Card Hand[2];

typedef struct Table {
	Card* board;
	Hand hand;
	Hand* opponents_hands;
	int board_count;
	int opponent_count;
} Table;

Deck* createDeck();
Card* pullCard(Deck* deck);
Table* fullDeal(Deck* deck);
void placeBet(int* pot, Table* table);
void revealCard(Table* table, int ix, int pot);
int hashCardValue(Card card);
int checkHand(Table* table, int hand_ix);
int decideWinner(Table* table);

#endif
