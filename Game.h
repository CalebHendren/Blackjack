#pragma once
#include "Deck.h"
#include "Hand.h"

class Game {
private:
    Deck deck;
    Hand playerHand;
    Hand dealerHand;
    int playerMoney;
    int currentBet;

public:
    Game();
    void playRound();
    void displayGameState(bool hideDealerCard = false) const;
    int getBet();
    void dealInitialCards();
    void playerTurn();
    void dealerTurn();
    void determineWinner();
    bool playAgain();
    void displayWelcome() const;
};