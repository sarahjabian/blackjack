#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <stdio.h>

#define DECK_SIZE 52

typedef struct Deck {
    Card cards[DECK_SIZE];
    int top; // Index of the top card
} Deck;

Deck MakeDeck();
void ShuffleDeck(Deck* deck);
void SaveDeckToFile(Deck deck, FILE *file);
Deck LoadDeckFromFile(FILE *file);
Card DrawCard(Deck* deck);


#endif