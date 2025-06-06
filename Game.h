#pragma once
#include "Deck.h"
#include "Hand.h"
#include <vector>

enum class HandStatus {
    PLAYING,
    STOOD,
    BUSTED,
    BLACKJACK,
    SURRENDERED,
    DOUBLED
};

struct PlayerHand {
    Hand hand;
    int bet;
    HandStatus status;
    bool isDoubled;
    
    PlayerHand(int betAmount) : bet(betAmount), status(HandStatus::PLAYING), isDoubled(false) {}
};

class Game {
private:
    Deck deck;
    std::vector<PlayerHand> playerHands;
    Hand dealerHand;
    int playerMoney;
    int baseBet;
    int currentHandIndex;
    bool canSurrender;
    bool canDoubleDown;

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
    
    // Advanced features
    void handleAdvancedOptions();
    bool splitHand();
    bool doubleDown();
    bool surrender();
    void playHand(int handIndex);
    void displayHandStatus(int handIndex) const;
};
