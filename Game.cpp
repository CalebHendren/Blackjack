#include "Game.h"
#include <iostream>
#include <limits>

Game::Game() : playerMoney(1000), currentBet(0) {}

void Game::displayWelcome() const {
    std::cout << "==========================\n";
    std::cout << "    WELCOME TO BLACKJACK   \n";
    std::cout << "==========================\n";
    std::cout << "Starting money: $" << playerMoney << "\n";
    std::cout << "Controls: W = Hit, S = Stand\n\n";
}

void Game::playRound() {
    if (playerMoney <= 0) {
        std::cout << "You're out of money! Game over.\n";
        return;
    }

    if (currentBet == 0) {
        currentBet = getBet();
        if (currentBet == 0) return;
    }

    playerHand.clear();
    dealerHand.clear();

    if (deck.isEmpty()) {
        deck = Deck();
    }

    dealInitialCards();
    displayGameState(true);

    if (playerHand.isBlackjack()) {
        std::cout << "BLACKJACK! You win!\n";
        playerMoney += currentBet * 1.5;
        currentBet = 0;
        return;
    }

    playerTurn();
    if (!playerHand.isBusted()) {
        dealerTurn();
        determineWinner();
    } else {
        std::cout << "BUST! You lose $" << currentBet << "\n";
        playerMoney -= currentBet;
    }

    currentBet = 0;
}

void Game::displayGameState(bool hideDealerCard) const {
    std::cout << "\n--------------------------\n";
    std::cout << "Dealer: ";
    if (hideDealerCard && dealerHand.size() > 1) {
        std::cout << dealerHand.toString().substr(0, dealerHand.toString().find(' ')) << " [HIDDEN]";
    } else {
        std::cout << dealerHand.toString() << " (Value: " << dealerHand.getValue() << ")";
    }
    std::cout << "\nPlayer: " << playerHand.toString() << " (Value: " << playerHand.getValue() << ")";
    std::cout << "\nMoney: $" << playerMoney;
    if (currentBet > 0) std::cout << " | Bet: $" << currentBet;
    std::cout << "\n--------------------------\n";
}

int Game::getBet() {
    int bet;
    std::cout << "Enter your bet (1-" << playerMoney << ") or 0 to quit: $";
    
    while (!(std::cin >> bet) || bet < 0 || bet > playerMoney) {
        if (bet == 0) return 0;
        std::cout << "Invalid bet. Enter amount between 1 and " << playerMoney << ": $";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    return bet;
}

void Game::dealInitialCards() {
    playerHand.addCard(deck.deal());
    dealerHand.addCard(deck.deal());
    playerHand.addCard(deck.deal());
    dealerHand.addCard(deck.deal());
}

void Game::playerTurn() {
    char choice;
    while (true) {
        std::cout << "\nYour turn. (W)Hit or (S)Stand? ";
        std::cin >> choice;
        choice = tolower(choice);
        
        if (choice == 'w') {
            playerHand.addCard(deck.deal());
            displayGameState(true);
            
            if (playerHand.isBusted()) {
                break;
            }
        } else if (choice == 's') {
            break;
        } else {
            std::cout << "Invalid choice. Use W for Hit or S for Stand.\n";
        }
    }
}

void Game::dealerTurn() {
    std::cout << "\nDealer's turn:\n";
    displayGameState(false);
    
    while (dealerHand.getValue() < 17) {
        std::cout << "Dealer hits.\n";
        dealerHand.addCard(deck.deal());
        displayGameState(false);
    }
    
    if (dealerHand.getValue() >= 17) {
        std::cout << "Dealer stands.\n";
    }
}

void Game::determineWinner() {
    int playerValue = playerHand.getValue();
    int dealerValue = dealerHand.getValue();
    
    std::cout << "\n========== RESULTS ==========\n";
    
    if (dealerHand.isBusted()) {
        std::cout << "Dealer busts! You win $" << currentBet << "!\n";
        playerMoney += currentBet;
    } else if (playerValue > dealerValue) {
        std::cout << "You win $" << currentBet << "!\n";
        playerMoney += currentBet;
    } else if (playerValue < dealerValue) {
        std::cout << "Dealer wins. You lose $" << currentBet << ".\n";
        playerMoney -= currentBet;
    } else {
        std::cout << "Push! Your bet is returned.\n";
    }
    
    std::cout << "New balance: $" << playerMoney << "\n";
}

bool Game::playAgain() {
    if (playerMoney <= 0) {
        std::cout << "You're out of money! Thanks for playing!\n";
        return false;
    }

    int bet;
    std::cout << "\nPlace your bet for next round (1-" << playerMoney << ") or 0 to quit: $";

    while (!(std::cin >> bet) || bet < 0 || bet > playerMoney) {
        std::cout << "Invalid bet. Enter amount between 0 and " << playerMoney << ": $";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (bet == 0) {
        return false;
    }

    currentBet = bet;
    return true;
}
