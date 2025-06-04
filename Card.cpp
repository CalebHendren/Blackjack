#include "Card.h"

Card::Card(Suit s, int v) : suit(s), value(v) {}

int Card::getValue() const {
    return value;
}

std::string Card::toString() const {
    std::string rank;
    if (value == 1) rank = "A";
    else if (value == 11) rank = "J";
    else if (value == 12) rank = "Q";
    else if (value == 13) rank = "K";
    else rank = std::to_string(value);

    std::string suitStr;
    switch (suit) {
        case Suit::HEARTS: suitStr = "H";
            break;
        case Suit::DIAMONDS: suitStr = "D";
            break;
        case Suit::CLUBS: suitStr = "C";
            break;
        case Suit::SPADES: suitStr = "S";
            break;
    }
    
    return rank + suitStr;
}

bool Card::isAce() const {
    return value == 1;
}
