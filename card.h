#ifndef CARD_H
#define CARD_H

// Enum for card suits
typedef enum Suit {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
} Suit;

// Struct for a playing card
typedef struct Card {
    enum Suit suit;
    int value; // 1 (Ace) to 13 (King)
}Card;

void PrintCard(Card c);
int GetCardPoints(Card c);
Card MakeCard(int rank, enum Suit suit);

#endif