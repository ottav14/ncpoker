#ifndef POKER_H
#define POKER_H

#define HEARTS "\u2665"
#define DIAMONDS "\u2666"
#define CLUBS "\u2663"
#define SPADES "\u2660"

typedef struct Card {
	char* suit;
	char* value;
} Card;

Card** createDeck();
Card* pullCard(Card** deck);
Card** fullDeal(Card** deck);

#endif
