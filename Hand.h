#pragma once
#include "Card.h"
#include <vector>

class Hand {
private:
    std::vector<Card> cards;

public:
    void addCard(const Card& card);
    int getValue() const;
    bool isBusted() const;
    bool isBlackjack() const;
    void clear();
    std::string toString() const;
    size_t size() const;

    // New methods for advanced features
    bool canSplit() const;

    Card getCard(int index) const;

    std::vector<Card> getCards() const;
};
