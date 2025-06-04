#pragma once
#include <string>

enum class Suit { HEARTS, DIAMONDS, CLUBS, SPADES };

class Card {
private:
    Suit suit;
    int value;

public:
    Card(Suit s, int v);
    int getValue() const;
    std::string toString() const;
    bool isAce() const;
};