#include "Game.h"
#include <iostream>
#include <limits>

Game::Game() : playerMoney(1000), baseBet(0), currentHandIndex(0), canSurrender(true), canDoubleDown(true) {
}

void Game::displayWelcome() const {
    std::cout << "==========================\n";
    std::cout << "    WELCOME TO BLACKJACK   \n";
    std::cout << "==========================\n";
    std::cout << "Starting money: $" << playerMoney << "\n";
    std::cout << "Controls: W = Hit, S = Stand, D = Double, Q = Split, R = Surrender\n\n";
}

void Game::playRound() {
    if (playerMoney <= 0) {
        std::cout << "You're out of money! Game over.\n";
        return;
    }

    if (baseBet == 0) {
        baseBet = getBet();
        if (baseBet == 0) return;
    }

    playerHands.clear();
    playerHands.push_back(PlayerHand(baseBet));
    dealerHand.clear();
    currentHandIndex = 0;
    canSurrender = true;
    canDoubleDown = true;

    if (deck.isEmpty()) {
        deck = Deck();
    }

    dealInitialCards();
    displayGameState(true);

    // Check for dealer blackjack
    if (dealerHand.isBlackjack()) {
        std::cout << "Dealer has blackjack!\n";
        displayGameState(false);

        if (playerHands[0].hand.isBlackjack()) {
            std::cout << "Push! Both have blackjack.\n";
        } else {
            std::cout << "Dealer wins with blackjack.\n";
            playerMoney -= baseBet;
        }
        baseBet = 0;
        return;
    }

    // Check for player blackjack
    if (playerHands[0].hand.isBlackjack()) {
        std::cout << "BLACKJACK! You win!\n";
        playerMoney += baseBet * 1.5;
        baseBet = 0;
        return;
    }

    playerTurn();

    // Check if any hands are still playing
    bool anyHandsActive = false;
    for (const auto &hand: playerHands) {
        if (hand.status != HandStatus::BUSTED && hand.status != HandStatus::SURRENDERED) {
            anyHandsActive = true;
            break;
        }
    }

    if (anyHandsActive) {
        dealerTurn();
    }

    determineWinner();
    baseBet = 0;
}

void Game::displayGameState(bool hideDealerCard) const {
    std::cout << "\n--------------------------\n";
    std::cout << "Dealer: ";
    if (hideDealerCard && dealerHand.size() > 1) {
        std::cout << dealerHand.toString().substr(0, dealerHand.toString().find(' ')) << " [HIDDEN]";
    } else {
        std::cout << dealerHand.toString() << " (Value: " << dealerHand.getValue() << ")";
    }

    std::cout << "\n\nYour Hands:\n";
    for (size_t i = 0; i < playerHands.size(); i++) {
        std::cout << "Hand " << (i + 1) << ": " << playerHands[i].hand.toString()
                << " (Value: " << playerHands[i].hand.getValue() << ")";
        std::cout << " | Bet: $" << playerHands[i].bet;

        if (i == currentHandIndex && playerHands[i].status == HandStatus::PLAYING) {
            std::cout << " [CURRENT]";
        }

        switch (playerHands[i].status) {
            case HandStatus::STOOD: std::cout << " [STOOD]";
                break;
            case HandStatus::BUSTED: std::cout << " [BUSTED]";
                break;
            case HandStatus::BLACKJACK: std::cout << " [BLACKJACK]";
                break;
            case HandStatus::SURRENDERED: std::cout << " [SURRENDERED]";
                break;
            case HandStatus::DOUBLED: std::cout << " [DOUBLED]";
                break;
            default: break;
        }
        std::cout << "\n";
    }

    std::cout << "Money: $" << playerMoney;
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
    playerHands[0].hand.addCard(deck.deal());
    dealerHand.addCard(deck.deal());
    playerHands[0].hand.addCard(deck.deal());
    dealerHand.addCard(deck.deal());
}

void Game::playerTurn() {
    for (currentHandIndex = 0; currentHandIndex < playerHands.size(); currentHandIndex++) {
        if (playerHands[currentHandIndex].status != HandStatus::PLAYING) {
            continue;
        }

        playHand(currentHandIndex);
    }
}

void Game::playHand(int handIndex) {
    currentHandIndex = handIndex;
    PlayerHand &currentHand = playerHands[handIndex];

    while (currentHand.status == HandStatus::PLAYING) {
        displayGameState(true);

        if (playerHands.size() > 1) {
            std::cout << "\nPlaying Hand " << (handIndex + 1) << ":\n";
        }

        handleAdvancedOptions();

        if (currentHand.status != HandStatus::PLAYING) {
            break;
        }

        char choice;
        std::cout << "\nYour turn. (W)hit, (S)tand";

        // Show additional options if available
        if (canDoubleDown && currentHand.hand.size() == 2 && playerMoney >= currentHand.bet) {
            std::cout << ", (D)ouble";
        }
        if (canSurrender && handIndex == 0 && playerHands.size() == 1 && currentHand.hand.size() == 2) {
            std::cout << ", (R)ender";
        }
        if (currentHand.hand.canSplit() && playerMoney >= currentHand.bet && playerHands.size() < 4) {
            std::cout << ", (Q)split";
        }

        std::cout << "? ";
        std::cin >> choice;
        choice = tolower(choice);

        if (choice == 'w') {
            currentHand.hand.addCard(deck.deal());
            canSurrender = false;
            canDoubleDown = false;

            if (currentHand.hand.isBusted()) {
                currentHand.status = HandStatus::BUSTED;
                std::cout << "Hand " << (handIndex + 1) << " busted!\n";
            }
        } else if (choice == 's') {
            currentHand.status = HandStatus::STOOD;
        } else if (choice == 'd' && canDoubleDown && currentHand.hand.size() == 2 && playerMoney >= currentHand.bet) {
            if (doubleDown()) {
                currentHand.hand.addCard(deck.deal());
                currentHand.status = HandStatus::DOUBLED;
                currentHand.isDoubled = true;
                currentHand.bet *= 2;

                if (currentHand.hand.isBusted()) {
                    currentHand.status = HandStatus::BUSTED;
                    std::cout << "Doubled down and busted!\n";
                } else {
                    std::cout << "Doubled down and stand.\n";
                }
            }
        } else if (choice == 'r' && canSurrender && handIndex == 0 && playerHands.size() == 1 && currentHand.hand.size()
                   == 2) {
            if (surrender()) {
                currentHand.status = HandStatus::SURRENDERED;
                std::cout << "Hand surrendered.\n";
            }
        } else if (choice == 'q' && currentHand.hand.canSplit() && playerMoney >= currentHand.bet && playerHands.size()
                   < 4) {
            if (splitHand()) {
                // After splitting, continue playing the current hand
                continue;
            }
        } else {
            std::cout << "Invalid choice.\n";
        }
    }
}

void Game::handleAdvancedOptions() {
    // This method is for future expansion of advanced options
    // Currently, options are handled in playHand
}

bool Game::splitHand() {
    PlayerHand &currentHand = playerHands[currentHandIndex];

    if (!currentHand.hand.canSplit() || playerMoney < currentHand.bet || playerHands.size() >= 4) {
        return false;
    }

    // Create new hand with second card
    PlayerHand newHand(currentHand.bet);
    newHand.hand.addCard(currentHand.hand.getCard(1));

    // Keep first card in current hand
    Hand tempHand;
    tempHand.addCard(currentHand.hand.getCard(0));
    currentHand.hand = tempHand;

    // Deal new cards to both hands
    currentHand.hand.addCard(deck.deal());
    newHand.hand.addCard(deck.deal());

    // Add new hand to the vector
    playerHands.insert(playerHands.begin() + currentHandIndex + 1, newHand);

    std::cout << "Hand split! You now have " << playerHands.size() << " hands.\n";
    canSurrender = false;

    return true;
}

bool Game::doubleDown() {
    PlayerHand &currentHand = playerHands[currentHandIndex];

    if (playerMoney < currentHand.bet || currentHand.hand.size() != 2) {
        return false;
    }

    return true; // The actual doubling is handled in playHand
}

bool Game::surrender() {
    if (!canSurrender || playerHands.size() > 1 || playerHands[0].hand.size() != 2) {
        return false;
    }

    return true; // The actual surrender is handled in playHand
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
    int dealerValue = dealerHand.getValue();
    bool dealerBusted = dealerHand.isBusted();

    std::cout << "\n========== RESULTS ==========\n";

    int totalWinnings = 0;
    int totalLosses = 0;

    for (size_t i = 0; i < playerHands.size(); i++) {
        PlayerHand &hand = playerHands[i];
        int playerValue = hand.hand.getValue();

        std::cout << "Hand " << (i + 1) << ": ";

        if (hand.status == HandStatus::SURRENDERED) {
            std::cout << "Surrendered - lose half bet ($" << (hand.bet / 2) << ")\n";
            totalLosses += hand.bet / 2;
        } else if (hand.status == HandStatus::BUSTED) {
            std::cout << "Busted - lose $" << hand.bet << "\n";
            totalLosses += hand.bet;
        } else if (dealerBusted) {
            std::cout << "Dealer busts - win $" << hand.bet << "\n";
            totalWinnings += hand.bet;
        } else if (playerValue > dealerValue) {
            std::cout << "Win $" << hand.bet << "\n";
            totalWinnings += hand.bet;
        } else if (playerValue < dealerValue) {
            std::cout << "Lose $" << hand.bet << "\n";
            totalLosses += hand.bet;
        } else {
            std::cout << "Push - bet returned\n";
        }
    }

    playerMoney += totalWinnings - totalLosses;

    if (totalWinnings > totalLosses) {
        std::cout << "Net win: $" << (totalWinnings - totalLosses) << "\n";
    } else if (totalLosses > totalWinnings) {
        std::cout << "Net loss: $" << (totalLosses - totalWinnings) << "\n";
    } else {
        std::cout << "Break even\n";
    }
    
    std::cout << "New balance: $" << playerMoney << "\n";
}

void Game::displayHandStatus(int handIndex) const {
    // This method can be used for detailed hand status display
    // Currently integrated into displayGameState
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

    baseBet = bet;
    return true;
}
