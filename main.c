#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "deck.h"
#include "card.h"
#include "hand.h"

//contains all the game information
typedef struct Game{
    Hand playerHand;
    Hand dealerHand;
    int playerChips;
    int bet;
    Deck deck;
}Game;

//functions you must complete
int RunGame(Game* r, int bet);
void SaveGameToFile(Game game);
Game LoadGameFromFile();

void SaveHandToFile(Hand hand, FILE *file) {
    for (int i = 0; i < HANDSIZE; ++i) {
        fprintf(file, "%d %d\n", hand.cards[i].value, hand.cards[i].suit);
    }
}
void SaveDeckToFile(Deck deck, FILE *file) {
    for (int i = 0; i < DECK_SIZE; ++i) {
        fprintf(file, "%d %d\n", deck.cards[i].value, deck.cards[i].suit);
    }
}
Hand LoadHandFromFile(FILE *file) {
    Hand hand = MakeHand();
    for (int i = 0; i < HANDSIZE; ++i) {
        fscanf(file, "%d %d\n", &hand.cards[i].value, &hand.cards[i].suit);
    }
    return hand;
}
Deck LoadDeckFromFile(FILE *file) {
    Deck deck = MakeDeck();
    for (int i = 0; i < DECK_SIZE; ++i) {
        fscanf(file, "%d %d\n", &deck.cards[i].value, &deck.cards[i].suit);
    }
    return deck;
}

int main(int argc, char** argv){
    
    Game game;
    game.playerHand = MakeHand();
    game.dealerHand = MakeHand();
    game.playerChips = 1000;
    game.deck = MakeDeck();

    int loaded = 0;

    if (argc > 1 && strcmp(argv[1], "load") == 0) {
        loaded = 1;
    }

    if (loaded == 1) {
        game = LoadGameFromFile();
    }

  while(game.playerChips > 0){

    if(loaded == 0){
        printf("\nYou have %d chips\n",game.playerChips);
        printf("How much would you like to bet?\n");
        scanf("%d",&game.bet);
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { } //clear input buffer to avoid issues
        if(game.bet == 0){
            break; //we're done
        }
        
        //this is how we deal cards
        AddCard(&game.playerHand, DrawCard(&game.deck));
        AddCard(&game.dealerHand, DrawCard(&game.deck));

        AddCard(&game.playerHand, DrawCard(&game.deck));
        AddCard(&game.dealerHand, DrawCard(&game.deck));
    }else{
        loaded = 0;
    }


    if(CalculatePoints(&game.playerHand) == 21){
        printf("You were dealt a 21!");
        PrintHand(game.playerHand);
        game.playerChips += (int)(double)game.bet/2.0;
    }

    game.playerChips += RunGame(&game, game.bet);
    game.playerHand = MakeHand();
    game.dealerHand = MakeHand();
    game.deck = MakeDeck();
  }

  printf("Game over.\n");
  printf("You left with %d chips.\n", game.playerChips);
}

int RunGame(Game* r, int bet){ //returns true on a win, false on a loss

    printf("\n"); //just to make things pretty

    //get the player points using CalculatePoints function, if greater than 21 print out u busted and return -bet
    int playerPts = CalculatePoints(&r->playerHand);
    if (playerPts > 21) {
        printf("YOU BUSTED!\n");
        return -bet;
    }

    //print out info on dealer hand and player hand. Remember not to reveal dealer's second card!
    int dealerPts = CalculatePoints(&r->dealerHand);
    printf("Dealer Score: %d + ???\n", dealerPts);
    printf("Dealer Hand: ");
    PrintCard(r->dealerHand.cards[0]);
    printf("??? of ???\n");
    printf("Player Score: %d\n", playerPts);
    printf("Player Hand: ");
    PrintHand(r->playerHand);

    //prompt user to hit 'h' stay 's' or take a break 'b'
    printf("Will you hit 'h', stay 's', or take a break 'b'?\n");
    //read character from terminal (don't forget to clear input buffer see above)
    char c;
    scanf(" %c", &c);
    int buffer;

    while ((buffer = getchar()) != '\n' && buffer != EOF) {} 
    switch (c) {
        case 'h': // hit
            AddCard(&r->playerHand, DrawCard(&r->deck));
            return RunGame(r, bet); // recursive call!
        case 's': // stay
            while (CalculatePoints(&r->dealerHand) < 16) {
                AddCard(&r->dealerHand, DrawCard(&r->deck));
            }
            dealerPts = CalculatePoints(&r->dealerHand);
            if ((dealerPts > 21) || (playerPts > dealerPts)) {
                printf("\nYOU WIN!\n");
                printf("Dealer Score: %d\n", dealerPts);
                printf("Dealer Hand: ");
                PrintHand(r->dealerHand);
                printf("Player Score: %d\n", playerPts);
                printf("Player Hand: ");
                PrintHand(r->playerHand);
                printf("\n");
                return bet; // player wins
            }
            else if (dealerPts > playerPts) {
                printf("\nYOU LOST.\n");
                printf("Dealer Score: %d\n", dealerPts);
                printf("Dealer Hand: ");
                PrintHand(r->dealerHand);
                printf("Player Score: %d\n", playerPts);
                printf("Player Hand: ");
                PrintHand(r->playerHand);
                printf("\n");
                return -bet; // player loses
            }
            else {
                printf("\nIT'S A TIE!\n");
                printf("Dealer Score: %d\n", dealerPts);
                printf("Dealer Hand: ");
                PrintHand(r->dealerHand);
                printf("Player Score: %d\n", playerPts);
                printf("Player Hand: ");
                PrintHand(r->playerHand);
                printf("\n");
                return 0; // dealer and player tie
            }
            break;
        case 'b': // take a break
            printf("\nSaving game...\n");
            SaveGameToFile(*r); // saves game
             exit(0);
        default:
            printf("\nInvalid choice. Please choose 'h', 's', or 'b'.\n");
            return RunGame(r, bet);
        }
        return 0;
    }

    
void SaveGameToFile(Game game){
    FILE *file = NULL;
    file = fopen("save.txt", "w"); 
    if (file == NULL) {
        printf("Couldn't open file.");
        return;
    }
    fprintf(file, "%d %d\n", game.playerChips, game.bet);
    SaveHandToFile(game.playerHand, file); // saves player's hand
    SaveHandToFile(game.dealerHand, file); // saves dealer's hand
    SaveDeckToFile(game.deck, file); // saves deck
    fclose(file);
}

Game LoadGameFromFile() {
    Game game;
    FILE *file = NULL;
    file = fopen("save.txt", "r");
    if (file == NULL) {
        printf("Couldn't open file.\n");
        return game;
    }
    fscanf(file, "%d %d\n", &game.playerChips, &game.bet);
    game.playerHand = LoadHandFromFile(file); // loads player's hand
    game.dealerHand = LoadHandFromFile(file); // loads dealer's hand
    game.deck = LoadDeckFromFile(file); // loads deck
    fclose(file);
    return game;
}
